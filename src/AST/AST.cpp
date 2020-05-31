#include "AST/AST.h"

//the entry of code generation
llvm::Value *MainAST::codegen(CodeGen &context) {
	// PassManager manages ModulePassManagers
	llvm::legacy::PassManager pm;
	//Create a pass that writes the module to the raw_ostream llvm::outs(), add it into pm
	pm.add(llvm::createPrintModulePass(llvm::outs()));
	//Initialization
	llvm::InitializeAllTargetInfos();
	llvm::InitializeAllTargets();
	llvm::InitializeAllTargetMCs();
	llvm::InitializeAllAsmParsers();
	llvm::InitializeAllAsmPrinters();
	//get the default target triple
	std::string targetTriple = llvm::sys::getDefaultTargetTriple();
	//set the target triple.
	context.module->setTargetTriple(targetTriple);
	std::string err;
	//Lookup a target based on the target triple
	auto target = llvm::TargetRegistry::lookupTarget(targetTriple, err);
	if (!target) {
		llvm::errs() << err;
		return nullptr;
	}
	//Get the LLVM name for the host CPU
	auto cpu = llvm::sys::getHostCPUName();
	llvm::TargetOptions option;
	auto RM = llvm::Optional<llvm::Reloc::Model>();
	//Create a target specific machine implementation for the specified triple.
	auto targetMachine = target->createTargetMachine(targetTriple, cpu, "", option, RM);
	//Create a DataLayout and et the data layout
	context.module->setDataLayout(targetMachine->createDataLayout());

	std::vector<llvm::Type *> params;
	//package the whole real expression in a main function, return type "int"
	llvm::FunctionType *mainFuncTy = llvm::FunctionType::get(llvm::Type::getInt64Ty(context.context), llvm::makeArrayRef(params), false);
	llvm::Function *mainFun = llvm::Function::Create(mainFuncTy, llvm::GlobalValue::ExternalLinkage, "main", context.module.get());
	//staticLink used for child function to access vars in parent function
	context.staticLink.push_front(llvm::StructType::create(context.context, "main"));
	//Create a new BasicBlock, here is the entry block
	llvm::BasicBlock *basicBlock = llvm::BasicBlock::Create(context.context, "entry", mainFun);
	//set the builtin type
	context.types["int"] = context.intType;
	context.types["string"] = context.stringType;
	//set the standard function library
	context.builtin();
	//type check and generate
	traverse(mainVarTable, context);
	if (context.errorOccurs) {
		return nullptr;
	}
	//clear var declaration table and function declaraion table, prepare for code generation
	context.varDecls.resetTable();
	context.funcDecls.resetTable();
	context.builder.SetInsertPoint(basicBlock);
	//vars declared in main function, store the llvm::Type
	std::vector<llvm::Type *> localVars;
	for (VarDeclAST* &var : mainVarTable) {
		localVars.push_back(var->getType());
		context.varDecls.push(var->getId(), var);
	}
	//staticLink used for child function to access vars in parent function, now push the local vars in
	context.staticLink.front()->setBody(localVars);
	context.currentFrame = context.getEntryBlockAllocate(mainFun, context.staticLink.front(), "mainFrame");
	//set the function level
	context.currentLevel = 0;

	//the real expression in main function, start code generation
	_mainExpr->codegen(context);

	//return 0
	context.builder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt64Ty(context.context), llvm::APInt(64, 0)));
	//Check the main function for errors
	if (llvm::verifyFunction(*mainFun, &llvm::errs())) {
		return context.errorValueMsg("codegen() fails.");
	}
	std::cout << "Code generation finishes." << std::endl;

	//notice we may need to link some built in function for the execution
	std::string fileName = "output.o";
	std::error_code ec;
	//Create a raw_ostream that writes to "output.o"
	llvm::raw_fd_ostream dest(fileName, ec, llvm::sys::fs::F_None);
	if (ec) {
		llvm::errs() << "Fail to open the file: " << ec.message();
		return nullptr;
	}
	//Add passes to get the specified emitted.
	if (targetMachine->addPassesToEmitFile(pm, dest, nullptr, llvm::CGFT_ObjectFile)) {
		llvm::errs() << "TargetMachine fails to add passes to emit the file.";
		return nullptr;
	}
	//Execute passes
	pm.run(*context.module);
	dest.flush();
	llvm::outs() << "Write to " << fileName << "\n";

	return nullptr;
}

//===----------------------------------------------------------------------===//
// Variable
//===----------------------------------------------------------------------===//

// name
// example: student
// _name = "student"
llvm::Value *SimpleVarAST::codegen(CodeGen &context) {
	//look for the varaible in var declarations
	VarDeclAST *var = context.varDecls[_name];
	//if found, read the llvm::Value in
	if (var)
		return var->read(context);
	else
		return context.errorValueMsg("Variable " + _name + " unknown");
}

// var.field
// Example: student.age
// _var = student, _field = "age"
llvm::Value *FieldVarAST::codegen(CodeGen &context) {
	llvm::Value *value = _var->codegen(context);
	if (!value)
		return nullptr;
	//the _var should be pointer type and the fields should be struct type
	value = context.builder.CreateLoad(value, "struct_ptr");
	//the index of current _field
	llvm::Value *idx = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context.context), llvm::APInt(64, index));
	return context.builder.CreateGEP(type_, value, idx, "ptr");
}

// var[index]
// Example: array[0]
// _var = array _index = 0
llvm::Value *IndexVarAST::codegen(CodeGen &context) {
	//the _var should be pointer type
	llvm::Value *varValue = _var->codegen(context);
	if (!varValue)
		return nullptr;
	llvm::Value *indexValue = _index->codegen(context);
	varValue = context.builder.CreateLoad(varValue, "array_ptr");
	return context.builder.CreateGEP(type_, varValue, indexValue, "ptr");
}

//===----------------------------------------------------------------------===//
// Declaration
//===----------------------------------------------------------------------===//

// var id := expr
// var id : type_id := expr
// Example: var a : int := 1
// _id = "a", _type = int, _expr = 1
llvm::Value *VarDeclAST::codegen(CodeGen &context) {
	llvm::Value *exprValue = _expr->codegen(context);
	if (!exprValue)
		return nullptr;
	//store the expr value, then store this var in var declararion table
	llvm::Value *value = context.getStore(exprValue, read(context));
	context.varDecls.push(_id, this);
	return value;
}

//to read the value of variable in
llvm::Value *VarDeclAST::read(CodeGen &context) const {
	//Corresponds to current function level
	llvm::Value *value = context.currentFrame;
	size_t curLevel = context.currentLevel;
	//find the variable in this scope and in its parent functions
	while (curLevel > level_) {
		std::vector<llvm::Value *> index(2);
		//value 0
		index[0] = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context.context), llvm::APInt(32, 0));
		index[1] = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context.context), llvm::APInt(32, 0));
		value = context.builder.CreateInBoundsGEP(value, index, "staticLink");
		value = context.builder.CreateLoad(value, "frame");
		--curLevel;
	}
	std::vector<llvm::Value *> index(2);
	//value 0
	index[0] = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context.context), llvm::APInt(32, 0));
	//value offset_
	index[1] = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context.context), llvm::APInt(32, offset_));
	return context.builder.CreateGEP(value, index, _id);
}

// type type-id = type
// Example: type intArray = array of int
// _type_id = "intArray", _type = array of int
llvm::Value *TypeDeclAST::codegen(CodeGen &context) {
	//null value
	return llvm::Constant::getNullValue(llvm::Type::getInt64Ty(context.context));
}

// name (type-fields)
// name (type-fields) : type-id
// Example: f(a: int, b:int) = printi(a + b)
// _name = "f", _params = (a: int, b:int), _returnType = void
llvm::Function *FuncTy::codegen(CodeGen &context) {
	llvm::Type *rType = resultType_;
	if (!rType)
		return nullptr;
	size_t index = 0u;
	size_t offset = 0u;
	//statucLink exists
	if (function_->getLinkage() != llvm::Function::ExternalLinkage)
		offset = 1u;
	TypeFieldsAST *typeField = _params;
	//iterating for args setName
	for (auto &arg : function_->args()) {
		if (index >= offset) {
			arg.setName(typeField->getId());
			typeField = typeField->getNext();
			++index;
		}
		else {
			//variables from parent functions store as a record variable "staticLink"
			arg.setName("staticLink");
			++index;
		}
	}
	return function_;
}

// function name (type-fields) = expr
// function name (type-fields) : type-id = expr
// Example: f(a: int, b:int) = printi(a + b)
// _name = "f", _type_fields = (a: int, b:int), expr = printi(a + b)
llvm::Value *FuncDeclAST::codegen(CodeGen &context) {
	//get the llvm::Function init of this function declaration
	llvm::Function *func = _funcTy->codegen(context);
	if (context.funcDecls.find(_name))
		return context.errorValueMsg("Function " + _name + " is already defined in this scope!");
	if (!func)
		return nullptr;
	//store this function declaration
	context.funcDecls.push(_name, this);
	llvm::BasicBlock *oldBlock = context.builder.GetInsertBlock();
	//create a new basic block for this function
	llvm::BasicBlock *block = llvm::BasicBlock::Create(context.context, "entry", func);
	context.builder.SetInsertPoint(block);
	//create a new scope
	context.varDecls.enter();
	//increase current function level
	context.currentLevel += 1;
	//store the local variables in this function to staticLink
	context.staticLink.push_front(_funcTy->getFrame());
	//store llvm::Type of local variables
	std::vector<llvm::Type *> localVars;
	for (auto &var : varTable_) {
		localVars.push_back(var->getType());
	}
	_funcTy->getFrame()->setBody(localVars);
	llvm::AllocaInst *oldFrame = context.currentFrame;
	context.currentFrame = context.getEntryBlockAllocate(func, _funcTy->getFrame(), _name + "Frame");
	TypeFieldsAST *params = _funcTy->getParams();
	int index = 0;
	//iterating for args store value
	for (auto &arg : func->args()) {
		if (index == 0) {
			//read from staticLink
			context.builder.CreateStore(&arg, _funcTy->getStaticLink()->read(context));
			++index;
		}
		else {
			//if (params == nullptr)
			//	return nullptr;
			VarDeclAST *curVar = params->getVarDecl();
			context.builder.CreateStore(&arg, curVar->read(context));
			//store the specified varDecl
			context.varDecls.push(arg.getName().data(), curVar);
			params = params->getNext();
		}
	}
	//now handle the return type
	llvm::Value *returnValue = _expr->codegen(context);
	if (returnValue) {
		if (_funcTy->getResultType()->isVoidTy()) {
			context.builder.CreateRetVoid();
		}
		else {
			context.builder.CreateRet(returnValue);
		}
		//no error
		if (!llvm::verifyFunction(*func, &llvm::errs())) {
			//exit current scope
			context.varDecls.exit();
			//resume previous environment
			context.builder.SetInsertPoint(oldBlock);
			context.currentFrame = oldFrame;
			//pop current staticLink for exiting this scope
			context.staticLink.pop_front();
			//decrease function level
			context.currentLevel -= 1;
			//codegen success
			return func;
		}
	}
	//exit current scope
	context.varDecls.exit();
	//error occurs, unlink it
	func->eraseFromParent();
	//delete this funcDecl because of error
	context.funcDecls.pop(_name);
	//resume previous environment
	context.builder.SetInsertPoint(oldBlock);
	context.currentFrame = oldFrame;
	//pop current staticLink for exiting this scope
	context.staticLink.pop_front();
	//decrease function level
	context.currentLevel -= 1;
	//codegen failed
	return context.errorValueMsg("Codegen of Function " + _name + " fails!");
}

//===----------------------------------------------------------------------===//
// Expression
//===----------------------------------------------------------------------===//

// func_name(expr-list)
// Example: f() f(1, 2)
// _func_name = "f", _expr_list = (1, 2)
llvm::Value *FuncExprAST::codegen(CodeGen &context) {
	//find the function declaration
	FuncDeclAST* funcDecl = context.funcDecls[_func_name];
	llvm::Function *function;
	llvm::Function::LinkageTypes linkageType;
	size_t curLevel = 0u;
	if (funcDecl) {
		//found in function declaration table, get the llvm::Function, LinkageTypes, function level
		function = funcDecl->getFuncTy().getFunction();
		linkageType = function->getLinkage();
		curLevel = funcDecl->getLevel();
	}
	else {
		//cannnot find it in declaration table, then find it in functions table, where some builtin function is in 
		function = context.functions[_func_name];
		if (!function)
			return context.errorValueMsg("Cannot find function " + _func_name);
		linkageType = llvm::Function::ExternalLinkage;
	}
	//function parameters
	std::vector<llvm::Value *> args;
	if (linkageType == llvm::Function::InternalLinkage) {
		size_t _currentLevel = context.currentLevel;
		llvm::Value *value = context.currentFrame;
		//get the function parameters, from itself and from parent functions
		while (_currentLevel >= curLevel) {
			std::vector<llvm::Value *> index(2);
			//value 0
			index[0] = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context.context), llvm::APInt(32, 0));
			index[1] = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context.context), llvm::APInt(32, 0));
			value = context.builder.CreateInBoundsGEP(value, index, "staticLink");
			value = context.builder.CreateLoad(value, "frame");
			--_currentLevel;
		}
		args.push_back(value);
	}
	//traverse the expr-list for code generation
	ExprListAST* exprList = _expr_list;
	ExprAST* exprAST;
	while (exprList) {
		exprAST = exprList->getExpr();
		//put the result in function parameters
		args.push_back(exprAST->codegen(context));
		if (!args.back())
			return nullptr;
		exprList = exprList->getNext();
	}
	//handle the function return type
	if (function->getFunctionType()->getReturnType()->isVoidTy()) {
		return context.builder.CreateCall(function, args);
	}
	else {
		return context.builder.CreateCall(function, args, "calltmp");
	}
}

// integer-constant
// Example: 1234
// _value = 1234
llvm::Value *IntConstExprAST::codegen(CodeGen &context) {
	//handle integer _value, return a ConstantInt with the specified _value and implied Type(numBits width = 64)
	return llvm::ConstantInt::get(context.context, llvm::APInt(64, _value));
}

// string-constant
// Example: "hello"
// _value = "hello"
llvm::Value *StrConstExprAST::codegen(CodeGen &context) {
	//handle string _value, return a pointer with "i8*" type
	return context.builder.CreateGlobalStringPtr(_value, "str");
}

// nil
llvm::Value *NilExprAST::codegen(CodeGen &context) {
	//return a constant pointer value that points to null
	return llvm::ConstantPointerNull::get(context.nilType);
}

// lvalue
llvm::Value *VarExprAST::codegen(CodeGen &context) {
	//handle the lvalue, create load
	llvm::Value *value = _value->codegen(context);
	if (!value)
		return nullptr;
	return context.builder.CreateLoad(value, value->getName());
}

// - expr, '-' is the only operator
llvm::Value *UnaryExprAST::codegen(CodeGen &context) {
	//check the operator
	if (_op[0] != '-')
		return context.errorValueMsg("The op of Unary Expr must be '-'!");
	//handle the expr
	llvm::Value *V = _expr->codegen(context);
	if (!V)
		return nullptr;
	//just get (-)expr value
	return context.builder.CreateNeg(V, "negtmp");
}

// expr binary-operator expr
llvm::Value *BinaryExprAST::codegen(CodeGen &context) {
	llvm::Value *L = _expr_left->codegen(context);
	llvm::Value *R = _expr_right->codegen(context);
	if (!L || !R)
		return nullptr;
	switch (_op[0]) {
		//below 6 operators are number operatores, both L and R should be integer type 
	case '+':
		return context.builder.CreateAdd(L, R, "addtmp");
	case '-':
		return context.builder.CreateSub(L, R, "subtmp");
	case '*':
		return context.builder.CreateMul(L, R, "multmp");
	case '/':
		return context.builder.CreateFPToSI(
			context.builder.CreateFDiv(
				context.builder.CreateSIToFP(L, llvm::Type::getDoubleTy(context.context)),
				context.builder.CreateSIToFP(R, llvm::Type::getDoubleTy(context.context)),
				"divftmp"),
			llvm::Type::getInt64Ty(context.context), "divtmp");
	case '&':
		return context.builder.CreateAnd(L, R, "andtmp");
	case '|':
		return context.builder.CreateOr(L, R, "ortmp");
		//below 6 operators are compare operators, L and R should be the same type
	case '=':
		L = context.handleNil(L, R);
		R = context.handleNil(R, L);
		//for str comparison
		if (L->getType() == context.stringType) {
			L = context.strcmp(L, R);
			R = context.zero;
		}
		return context.builder.CreateZExt(context.builder.CreateICmpEQ(L, R, "cmptmp"), context.intType, "cmptmp");
	case '>':
		//it's '>='
		if (_op.length() > 1) {
			//for str comparison
			if (L->getType() == context.stringType) {
				L = context.strcmp(L, R);
				R = context.zero;
			}
			return context.builder.CreateZExt(context.builder.CreateICmpSGE(L, R, "cmptmp"), context.intType, "cmptmp");
		}
		//it's '>'
		else {
			//for str comparison
			if (L->getType() == context.stringType) {
				L = context.strcmp(L, R);
				R = context.zero;
			}
			return context.builder.CreateZExt(context.builder.CreateICmpSGT(L, R, "cmptmp"), context.intType, "cmptmp");
		}
	case '<':
		if (_op.length() > 1) {
			//it's '<='
			if (_op[1] == '=') {
				//for str comparison
				if (L->getType() == context.stringType) {
					L = context.strcmp(L, R);
					R = context.zero;
				}
				return context.builder.CreateZExt(context.builder.CreateICmpSLE(L, R, "cmptmp"), context.intType, "cmptmp");
			}
			//it's '<>'
			else {
				L = context.handleNil(L, R);
				R = context.handleNil(R, L);
				//for str comparison
				if (L->getType() == context.stringType) {
					L = context.strcmp(L, R);
					R = context.zero;
				}
				if (L->getType() == context.stringType) {
					return context.strcmp(L, R);
				}
				return context.builder.CreateZExt(context.builder.CreateICmpNE(L, R, "cmptmp"), context.intType, "cmptmp");
			}
		}
		//it's '<'
		else {
			//for str comparison
			if (L->getType() == context.stringType) {
				L = context.strcmp(L, R);
				R = context.zero;
			}
			return context.builder.CreateZExt(context.builder.CreateICmpSLT(L, R, "cmptmp"), context.intType, "cmptmp");
		}
	}

	return nullptr;
}

// lvalue := expr
// Example: a := 3
// _var = a, _expr = 3
llvm::Value *AssignExprAST::codegen(CodeGen &context) {
	llvm::Value *varValue = _var->codegen(context);
	if (!varValue)
		return nullptr;
	llvm::Value *expValue = _expr->codegen(context);
	if (!expValue)
		return nullptr;
	//store the expValue in varValue, here varValue is pointer type
	context.getStore(expValue, varValue);
	return expValue;
}

// ( expr-list ) , list can be null
// Example: (a := 1 ; b := 2)  ()
llvm::Value *SequenceExprAST::codegen(CodeGen &context) {
	//we just traverse the _expr_list
	llvm::Value *value = nullptr;
	ExprListAST* exprList = _expr_list;
	ExprAST* curExpr;
	while (exprList) {
		curExpr = exprList->getExpr();
		value = curExpr->codegen(context);
		exprList = exprList->getNext();
	}
	//the result value corresponds to the value of last expression in expression list
	return value;
}

// type-id { field-list } , list can be null
// Example:  human { age = 18, height = 170 }
// _type_id = "human" field-list = { age = 18, height = 170 }
llvm::Value *RecordDefineExprAST::codegen(CodeGen &context) {
	//the llvm::type must have generated
	if (!type_)
		return nullptr;
	//get the element type of the pointer type 'type_'
	llvm::Type *elementType = context.getType(type_);
	//the offset in bytes between successive objects of elementType
	auto size = context.module->getDataLayout().getTypeAllocSize(elementType);
	//allocate space in the heap, for element with 'size'
	llvm::Value *value = context.builder.CreateCall(context.allocateRecordFunction, llvm::ConstantInt::get(context.intType, llvm::APInt(64, size)), "alloca");
	value = context.builder.CreateBitCast(value, type_, "record");
	//index of fields
	size_t index = 0u;
	FieldListAST* fieldList = _field_list;
	ExprAST* curFieldExpr;
	llvm::Type *curType;
	while (fieldList) {
		curFieldExpr = fieldList->getExpr();
		//handle the value of current field expr
		llvm::Value *exprValue = curFieldExpr->codegen(context);
		if (!exprValue)
			return nullptr;
		//get the llvm::type of this field
		curType = fieldList->getType();
		if (!curType)
			return nullptr;
		//allocate space for this field, then store the value of field
		llvm::Value *elementPtr = context.builder.CreateGEP(curType, value, llvm::ConstantInt::get(llvm::Type::getInt64Ty(context.context), llvm::APInt(64, index)), "elementPtr");
		context.getStore(exprValue, elementPtr);
		//increase the field index by 1
		index += 1;
		fieldList = fieldList->getNext();
	}
	return value;
}

// type-id [size] of value
// Example: int[3] of 0
// _type_id = "int", size = 3, value = 0
llvm::Value *ArrayDefineExprAST::codegen(CodeGen &context) {
	llvm::Function* func = context.builder.GetInsertBlock()->getParent();
	//get the element type of array
	llvm::Type *elementType = context.getType(type_);
	//handle the number of elements in array
	llvm::Value *sizeV = _size->codegen(context);
	//handle the initial value of each element
	llvm::Value *valueV = _value->codegen(context);
	//the offset in bytes between successive objects of elementType
	auto elementSize = context.module->getDataLayout().getTypeAllocSize(elementType);
	//allocate space in the heap, for 'size' number of elements, each with elementSize
	llvm::Value *arrayPtr =
		context.builder.CreateCall(context.allocateArrayFunction, std::vector<llvm::Value *>{sizeV, llvm::ConstantInt::get(llvm::Type::getInt64Ty(context.context), llvm::APInt(64, elementSize))}, "alloca");
	arrayPtr = context.builder.CreateBitCast(arrayPtr, type_, "array");
	//index is initialized to 0
	llvm::Value *zero = llvm::ConstantInt::get(context.context, llvm::APInt(64, 0, true));
	llvm::AllocaInst *indexPtr = context.getEntryBlockAllocate(func, llvm::Type::getInt64Ty(context.context), "index");
	context.builder.CreateStore(zero, indexPtr);
	//the test condition to decide whether we have create 'size' number of elements
	llvm::BasicBlock *testBB = llvm::BasicBlock::Create(context.context, "test", func);
	//the body of loop, allocate a new element
	llvm::BasicBlock *doBB = llvm::BasicBlock::Create(context.context, "do", func);
	//just go to testBB
	llvm::BasicBlock *continueBB = llvm::BasicBlock::Create(context.context, "continue", func);
	//after we create 'size' number of elements
	llvm::BasicBlock *afterBB = llvm::BasicBlock::Create(context.context, "after", func);
	//br testBB
	context.builder.CreateBr(testBB);
	context.builder.SetInsertPoint(testBB);
	llvm::Value *index = context.builder.CreateLoad(indexPtr, "index");
	//end condition of allocating elements
	llvm::Value *EndCond = context.builder.CreateICmpSLT(index, sizeV, "loopcond");
	//br EndCond doBB afterBB
	context.builder.CreateCondBr(EndCond, doBB, afterBB);
	context.builder.SetInsertPoint(doBB);
	//allocate a new array element
	llvm::Value *elementPtr = context.builder.CreateGEP(elementType, arrayPtr, index, "elementPtr");
	context.getStore(valueV, elementPtr);
	//br continueBB, increase the index of element we are allocating
	context.builder.CreateBr(continueBB);
	context.builder.SetInsertPoint(continueBB);
	//increase the index by 1
	llvm::Value *nextVar = context.builder.CreateAdd(index, llvm::ConstantInt::get(context.context, llvm::APInt(64, 1)), "nextVar");
	context.builder.CreateStore(nextVar, indexPtr);
	//br testBB
	context.builder.CreateBr(testBB);
	//allocation finishes
	context.builder.SetInsertPoint(afterBB);
	return arrayPtr;
}

// if expr then expr
// if expr then expr else expr
llvm::Value *IfExprAST::codegen(CodeGen &context) {
	llvm::Function* func = context.builder.GetInsertBlock()->getParent();
	llvm::Value* ifV = _if_expr->codegen(context);
	if (!ifV)
		return nullptr;
	//the test condition to decide which branch to go
	ifV = context.builder.CreateICmpNE(ifV, context.zero, "ifcond");
	//if ifV true, then go to thenBB
	llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create(context.context, "then", func);
	//if ifV false, go to elseBB
	llvm::BasicBlock *ElseBB = llvm::BasicBlock::Create(context.context, "else");
	//after the 'if' expr
	llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(context.context, "ifcont");
	//create a conditional 'br ifV, ThenBB, ElseBB'
	context.builder.CreateCondBr(ifV, ThenBB, ElseBB);
	context.builder.SetInsertPoint(ThenBB);
	//handle the branch _then_expr
	llvm::Value* ThenV = _then_expr->codegen(context);
	if (!ThenV)
		return nullptr;
	//br MergeBB
	context.builder.CreateBr(MergeBB);
	ThenBB = context.builder.GetInsertBlock();
	func->getBasicBlockList().push_back(ElseBB);
	context.builder.SetInsertPoint(ElseBB);
	llvm::Value* ElseV;
	if (_else_expr) {
		//if _else_expr exists, then handle _else_expr
		ElseV = _else_expr->codegen(context);
		if (!ElseV)
			return nullptr;
	}
	//br MergeBB
	context.builder.CreateBr(MergeBB);
	ElseBB = context.builder.GetInsertBlock();
	func->getBasicBlockList().push_back(MergeBB);
	context.builder.SetInsertPoint(MergeBB);
	if (_else_expr && !ThenV->getType()->isVoidTy() && !ElseV->getType()->isVoidTy()) {
		//when _then_expr and _else_expr all exists, then handle then
		llvm::PHINode *PN = context.builder.CreatePHI(ThenV->getType(), 2, "iftmp");
		ThenV = context.handleNil(ThenV, ElseV);
		ElseV = context.handleNil(ElseV, ThenV);
		//Add then value to the end of the PHI list
		PN->addIncoming(ThenV, ThenBB);
		PN->addIncoming(ElseV, ElseBB);
		return PN;
		;
	}
	else {
		//null value
		return llvm::Constant::getNullValue(llvm::Type::getInt64Ty(context.context));
	}
}

// while expr do expr
llvm::Value *WhileExprAST::codegen(CodeGen &context) {
	llvm::Function* func = context.builder.GetInsertBlock()->getParent();
	//test condition, to test if it's still in the 'while' loop, if in, go to doBB, else go to afterBB
	llvm::BasicBlock *whileBB = llvm::BasicBlock::Create(context.context, "whileCond", func);
	//the body of 'while' loop, corresponds to the _do_expr
	llvm::BasicBlock *doBB = llvm::BasicBlock::Create(context.context, "do", func);
	//in while expr, the continueBB just do the jump to testBB
	llvm::BasicBlock *continueBB = llvm::BasicBlock::Create(context.context, "continue", func);
	//corresponds to that after the 'while' loop
	llvm::BasicBlock *afterBB = llvm::BasicBlock::Create(context.context, "after", func);
	//push the continueBB and afterBB into loopStack, in case of break exp occurs, if break, go to afterBB 
	context.loopStack.push({ continueBB, afterBB });
	//Create an br instruction, to whileBB
	context.builder.CreateBr(whileBB);
	context.builder.SetInsertPoint(whileBB);
	//handle the _while_expr condition
	llvm::Value* WhileV = _while_expr->codegen(context);
	if (!WhileV)
		return nullptr;
	//to test the end of 'while' loop
	llvm::Value* EndCond = context.builder.CreateICmpEQ(WhileV, context.zero, "loopcond");
	//create a conditional branch instraction 'br EndCond, afterBB, doBB'
	context.builder.CreateCondBr(EndCond, afterBB, doBB);
	context.builder.SetInsertPoint(doBB);
	//handle the body expr of 'while' loop
	if (!_do_expr->codegen(context))
		return nullptr;
	//br continueBB
	context.builder.CreateBr(continueBB);
	context.builder.SetInsertPoint(continueBB);
	//br whileBB
	context.builder.CreateBr(whileBB);
	context.builder.SetInsertPoint(afterBB);
	context.loopStack.pop();//
	//null value for while exp
	return llvm::Constant::getNullValue(llvm::Type::getInt64Ty(context.context));
}

// for id := expr to expr do expr
llvm::Value *ForExprAST::codegen(CodeGen &context) {
	llvm::Function* func = context.builder.GetInsertBlock()->getParent();
	//for id := _init_expr to _to_expr do _do_expr
	llvm::Value *InitV = _init_expr->codegen(context);
	if (!InitV)
		return nullptr;
	//_init_expr must be integer type
	if (!InitV->getType()->isIntegerTy())
		return context.errorValueMsg("Type of 'init' expr of 'for' expr should be integer!");
	llvm::Value *ToV = _to_expr->codegen(context);
	if (!ToV)
		return nullptr;
	//_to_expr must also be integre type
	if (!ToV->getType()->isIntegerTy())
		return context.errorValueMsg("Type of 'to' expr of 'for' expr should be integer!");
	//store the value of _init_expr
	context.builder.CreateStore(InitV, _varDecl->read(context));
	//test condition, to test if it's still in the 'for' loop, if in, go to doBB, else go to afterBB
	llvm::BasicBlock *testBB = llvm::BasicBlock::Create(context.context, "test", func);
	//the body of 'for' loop, corresponds to the _do_expr
	llvm::BasicBlock *doBB = llvm::BasicBlock::Create(context.context, "do", func);
	//in for expr, the continueBB increase the _id by 1, then go to testBB
	llvm::BasicBlock *continueBB = llvm::BasicBlock::Create(context.context, "continue", func);
	//corresponds to that after the 'for' loop
	llvm::BasicBlock *afterBB = llvm::BasicBlock::Create(context.context, "after", func);
	//push the continueBB and afterBB into loopStack, in case of break exp occurs, if break, go to afterBB 
	context.loopStack.push({ continueBB, afterBB });
	//Create an br instruction, to testBB
	context.builder.CreateBr(testBB);
	context.builder.SetInsertPoint(testBB);
	//to test the end of 'for' loop
	llvm::Value *EndCond = context.builder.CreateICmpSLE(context.builder.CreateLoad(_varDecl->read(context), _id), ToV, "loopcond");
	//create a conditional 'br EndCond, doBB, afterBB'
	context.builder.CreateCondBr(EndCond, doBB, afterBB);
	context.builder.SetInsertPoint(doBB);
	//cache declaration of _id in case of it is declared outside the for loop
	VarDeclAST *var = context.varDecls[_id];
	if (var)
		context.varDecls.pop(_id);
	//then push the _id in 'for' loop into var declaraion table
	context.varDecls.push(_id, _varDecl);
	//handle the body expression inside the 'for' loop
	if (!_do_expr->codegen(context))
		return nullptr;
	//then increase the loop '_id' by 1
	context.builder.CreateBr(continueBB);
	context.builder.SetInsertPoint(continueBB);
	llvm::Value* nextVar = context.builder.CreateAdd(context.builder.CreateLoad(_varDecl->read(context), _id), llvm::ConstantInt::get(context.context, llvm::APInt(64, 1)), "nextVar");
	//store new value of '_id'
	context.builder.CreateStore(nextVar, _varDecl->read(context));
	//go to testBB to decide if the 'for' loop ends
	context.builder.CreateBr(testBB);
	context.builder.SetInsertPoint(afterBB);
	//resume the _id declaration outside the 'for' loop if exists
	if (var)
		context.varDecls[_id] = var;
	else
		context.varDecls.pop(_id);
	context.loopStack.pop();
	//null value for 'for' expr
	return llvm::Constant::getNullValue(llvm::Type::getInt64Ty(context.context));
}

// break
llvm::Value *BreakExprAST::codegen(CodeGen &context) {
	//create an unconditional br instruction.
	//here we store the block after the loop in loopStack
	context.builder.CreateBr(std::get<1>(context.loopStack.top()));
	//null value
	return llvm::Constant::getNullValue(llvm::Type::getInt64Ty(context.context));
}

// let declaration-list in expr-list end
// Example: 
//    let 
//        var a := 1
//        var b := 2
//    in
//        a + b
//    end
llvm::Value *LetExprAST::codegen(CodeGen &context) {
	//create a new scope for new var declarations and function declarations
	context.varDecls.enter();
	context.funcDecls.enter();
	//just traverse the declaration list
	DeclListAST* declList = _decl_list;
	DeclAST* curDecl;
	while (declList) {
		curDecl = declList->getDecl();
		curDecl->codegen(context);
		declList = declList->getNext();
	}
	//just traverse the expression list
	ExprListAST* exprList = _expr_list;
	ExprAST* curExpr;
	llvm::Value *resultValue;
	while (exprList) {
		curExpr = exprList->getExpr();
		resultValue = curExpr->codegen(context);
		exprList = exprList->getNext();
	}
	//delete current scope we create at the 'let' beginning
	context.funcDecls.exit();
	context.varDecls.exit();
	//the result value corresponds to the value of last expression in expression list
	return resultValue;
}

//===----------------------------------------------------------------------===//
// CodeGen
//===----------------------------------------------------------------------===//

CodeGen::CodeGen() {}

//get the element of exp(exp is pointer type)
llvm::Type *CodeGen::getType(llvm::Type *exp) {
	return llvm::cast<llvm::PointerType>(exp)->getElementType();
}

//check if two types match
bool CodeGen::typeMatch(llvm::Type *a, llvm::Type *b) {
	if (a == b)
		return true;
	else if (!a || !b)
		return false;
	else if (a == nilType) {
		if (isRecord(b))
			return true;
		else
			return false;
	}
	else if (b == nilType) {
		if (isRecord(a))
			return true;
		else
			return false;
	}
	else if (isRecord(a) && isRecord(b)) {
		return llvm::cast<llvm::StructType>(getType(a))->isLayoutIdentical(llvm::cast<llvm::StructType>(getType(b)));
	}
	else
		return false;
}

//check if type a and type b match
bool CodeGen::isRecord(llvm::Type *exp) {
	return exp->isPointerTy() && getType(exp)->isStructTy();
}

//get the llvm::Type of name
llvm::Type *CodeGen::typeOf(std::string const &name) {
	std::set<std::string> parentNames;
	return typeOf(name, parentNames);
}

//get the llvm::Type of name using previous declaraions
llvm::Type *CodeGen::typeOf(std::string const &name, std::set<std::string> &parentNames) {
	//first, check if it has a llvm::Type(it means all done)
	llvm::Type *type = types[name];
	if (type)
		return type;
	//else, check if the name is in type declarations, if found, generate the llvm type
	TypeAST *typeDecl = typeDecls[name];
	if (typeDecl)
		return typeDecl->traverse(parentNames, *this);
	else
		return errorTypeMsg(name + " isn't a type!");
}

llvm::AllocaInst *CodeGen::getEntryBlockAllocate(llvm::Function *function, llvm::Type *type, std::string const &name, llvm::Value *size) {
	llvm::IRBuilder<> TempBuilder(&function->getEntryBlock(), function->getEntryBlock().begin());
	return TempBuilder.CreateAlloca(type, size, name.c_str());
}

//before we get val store in ptr, there is something to do with the nil type
llvm::Value *CodeGen::handleNil(llvm::Value *val, llvm::Value *ptr) {
	if (val->getType() == nilType) {
		llvm::Type *type = ptr->getType();
		if (isRecord(type)) {
			return llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(type));
		}
		else {
			return errorValueMsg("Only record type can be nil!");
		}
	}
	return val;
}

//store val in where ptr points to
llvm::Value *CodeGen::getStore(llvm::Value *val, llvm::Value *ptr) {
	//in case of val is nillType
	val = handleNil(val, ptr);
	return builder.CreateStore(val, ptr);
}

//used in codegen, when error occurs
llvm::Type *CodeGen::errorTypeMsg(std::string const &msg) {
	errorOccurs = true;
	std::cerr << msg << std::endl;
	return nullptr;
}

//used in type check and generation, when error occurs
llvm::Value *CodeGen::errorValueMsg(std::string const &msg) {
	errorOccurs = true;
	std::cerr << msg << std::endl;
	return nullptr;
}

//create built in functions
llvm::Function *CodeGen::createBuiltinFunction(std::string const &name, llvm::Type *returnType, std::vector<llvm::Type *> const &args) {
	llvm::FunctionType *funcTy = llvm::FunctionType::get(returnType, args, false);
	llvm::Function *function = llvm::Function::Create(funcTy, llvm::Function::ExternalLinkage, name, module.get());
	//store the built in function
	functions.push(name, function);
	return function;
}

//standard library
void CodeGen::builtin() {
	functions["print"] = createBuiltinFunction("Print", voidType, { stringType });
	functions["printi"] = createBuiltinFunction("Printi", voidType, { intType });
	functions["flush"] = createBuiltinFunction("Flush", voidType, {});
	functions["getchar"] = createBuiltinFunction("Getchar", stringType, {});
	functions["ord"] = createBuiltinFunction("Ord", intType, { stringType });
	functions["chr"] = createBuiltinFunction("Chr", stringType, { intType });
	functions["size"] = createBuiltinFunction("_Size", intType, { stringType });
	functions["substring"] = createBuiltinFunction("Substring", stringType, { stringType, intType, intType });
	functions["concat"] = createBuiltinFunction("Concat", stringType, { stringType, stringType });
	functions["not"] = createBuiltinFunction("Not", intType, { intType });
	functions["exit"] = createBuiltinFunction("Exit", voidType, { intType });
}

//do the strcmp of two llvm::Value
llvm::Value *CodeGen::strcmp(llvm::Value *a, llvm::Value *b) {
	return builder.CreateCall(strcmpFunction, std::vector<llvm::Value *>{a, b}, "strcmp");
}
