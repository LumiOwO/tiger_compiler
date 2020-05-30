#include "AST/AST.h"

llvm::Value *MainAST::codegen(CodeGen &context) {
	llvm::legacy::PassManager pm;
	pm.add(llvm::createPrintModulePass(llvm::outs()));
	llvm::InitializeAllTargetInfos();
	llvm::InitializeAllTargets();
	llvm::InitializeAllTargetMCs();
	llvm::InitializeAllAsmParsers();
	llvm::InitializeAllAsmPrinters();

	std::string targetTriple = llvm::sys::getDefaultTargetTriple();
	context.module->setTargetTriple(targetTriple);
	std::string err;
	auto target = llvm::TargetRegistry::lookupTarget(targetTriple, err);
	if (!target) {
		llvm::errs() << err;
		return nullptr;
	}
	auto cpu = llvm::sys::getHostCPUName();
	llvm::TargetOptions option;
	auto RM = llvm::Optional<llvm::Reloc::Model>();
	auto targetMachine = target->createTargetMachine(targetTriple, cpu, "", option, RM);
	context.module->setDataLayout(targetMachine->createDataLayout());

	//package the whole expression in a main function, return type "int"
	std::vector<llvm::Type *> params;
	llvm::FunctionType *mainFuncTy = llvm::FunctionType::get(llvm::Type::getInt64Ty(context.context), llvm::makeArrayRef(params), false);
	llvm::Function *mainFun = llvm::Function::Create(mainFuncTy, llvm::GlobalValue::ExternalLinkage, "main", context.module.get());
	context.staticLink.push_front(llvm::StructType::create(context.context, "main"));
	llvm::BasicBlock *basicBlock = llvm::BasicBlock::Create(context.context, "entry", mainFun);

	context.types["int"] = context.intType;
	context.types["string"] = context.stringType;
	context.builtin();
	//type check and generate
	traverse(mainVarTable, context);
	if (context.errorOccurs) {
		return nullptr;
	}

	context.varDecls.resetTable();
	context.funcDecls.resetTable();
	context.builder.SetInsertPoint(basicBlock);
	std::vector<llvm::Type *> localVars;
	for (VarDeclAST* &var : mainVarTable) {
		localVars.push_back(var->getType());
		context.varDecls.push(var->getId(), var);
	}
	//staticLink used for child function to access vars in parent function
	context.staticLink.front()->setBody(localVars);
	context.currentFrame = context.getEntryBlockAllocate(mainFun, context.staticLink.front(), "mainFrame");
	context.currentLevel = 0;

	//the real expression in main function
	_mainExpr->codegen(context);

	//return 0
	context.builder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt64Ty(context.context), llvm::APInt(64, 0)));
	if (llvm::verifyFunction(*mainFun, &llvm::errs())) {
		return context.errorValueMsg("codegen() fails.");
	}
	std::cout << "Code generation finishes." << std::endl;

	//notice we may need to implement some built in function
	std::string fileName = "output.o";
	std::error_code ec;
	llvm::raw_fd_ostream dest(fileName, ec, llvm::sys::fs::F_None);
	if (ec) {
		llvm::errs() << "Fail to open the file: " << ec.message();
		return nullptr;
	}
	if (targetMachine->addPassesToEmitFile(pm, dest, nullptr, llvm::CGFT_ObjectFile)) {
		llvm::errs() << "TargetMachine fails to add passes to emit the file.";
		return nullptr;
	}
	pm.run(*context.module);
	dest.flush();
	llvm::outs() << "Write to " << fileName << "\n";

	return nullptr;
}

//===----------------------------------------------------------------------===//
// Variable
//===----------------------------------------------------------------------===//

llvm::Value *SimpleVarAST::codegen(CodeGen &context) {
	VarDeclAST *var = context.varDecls[_name];
	if (var)
		return var->read(context);
	else
		return context.errorValueMsg("Variable " + _name + " unknown");
}

llvm::Value *FieldVarAST::codegen(CodeGen &context) {
	llvm::Value *value = _var->codegen(context);
	if (!value)
		return nullptr;
	value = context.builder.CreateLoad(value, "struct_ptr");
	llvm::Value *idx = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context.context), llvm::APInt(64, index));
	return context.builder.CreateGEP(type_, value, idx, "ptr");
}

llvm::Value *IndexVarAST::codegen(CodeGen &context) {
	llvm::Value *varValue = _var->codegen(context);
	llvm::Value *indexValue = _index->codegen(context);
	if (!varValue)
		return nullptr;
	varValue = context.builder.CreateLoad(varValue, "array_ptr");
	return context.builder.CreateGEP(type_, varValue, indexValue, "ptr");
}

//===----------------------------------------------------------------------===//
// Declaration
//===----------------------------------------------------------------------===//

llvm::Value *VarDeclAST::codegen(CodeGen &context) {
	llvm::Value *exprValue = _expr->codegen(context);
	if (!exprValue)
		return nullptr;
	llvm::Value *value = context.getStore(exprValue, read(context));
	context.varDecls.push(_id, this);
	return value;
}

llvm::Value *VarDeclAST::read(CodeGen &context) const {
	auto staticLink = context.staticLink.begin();
	llvm::Value *value = context.currentFrame;
	size_t curLevel = context.currentLevel;
	while (curLevel > level_) {
		value = context.builder.CreateGEP(llvm::PointerType::getUnqual(*++staticLink), value, context.zero, "staticLink");
		value = context.builder.CreateLoad(value, "frame");
		--curLevel;
	}
	std::vector<llvm::Value *> index(2);
	index[0] = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context.context), llvm::APInt(32, 0));
	index[1] = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context.context), llvm::APInt(32, offset_));
	return context.builder.CreateGEP(*staticLink, value, index, _id);
}

llvm::Value *TypeDeclAST::codegen(CodeGen &context) {
	return llvm::Constant::getNullValue(llvm::Type::getInt64Ty(context.context));
}

llvm::Function *FuncTy::codegen(CodeGen &context) {
	llvm::Type *rType = resultType_;
	if (!rType)
		return nullptr;
	size_t index = 0u;
	size_t offset = 0u;
	if (function_->getLinkage() != llvm::Function::ExternalLinkage)
		offset = 1u;
	TypeFieldsAST *typeField = _params;
	for (auto &arg : function_->args()) {
		if (index >= offset) {
			arg.setName(typeField->getId());
			typeField = typeField->getNext();
			++index;
		}
		else {
			arg.setName("staticLink");
			++index;
		}
	}
	return function_;
}

llvm::Value *FuncDeclAST::codegen(CodeGen &context) {
	llvm::Function *func = _funcTy->codegen(context);
	if (context.funcDecls.find(_name))
		return context.errorValueMsg("Function " + _name + " is already defined in this scope!");
	if (!func)
		return nullptr;
	context.funcDecls.push(_name, this);
	llvm::BasicBlock *oldBlock = context.builder.GetInsertBlock();
	llvm::BasicBlock *block = llvm::BasicBlock::Create(context.context, "entry", func);
	context.builder.SetInsertPoint(block);

	context.varDecls.enter();
	context.currentLevel += 1;
	context.staticLink.push_front(_funcTy->getFrame());
	std::vector<llvm::Type *> localVars;
	for (auto &var : varTable_) {
		localVars.push_back(var->getType());
	}
	_funcTy->getFrame()->setBody(localVars);
	llvm::AllocaInst *oldFrame = context.currentFrame;
	context.currentFrame = context.getEntryBlockAllocate(func, _funcTy->getFrame(), _name + "Frame");
	TypeFieldsAST *params = _funcTy->getParams();
	int index = 0;
	for (auto &arg : func->args()) {
		if (index == 0) {
			context.builder.CreateStore(&arg, _funcTy->getStaticLink()->read(context));
			++index;
		}
		else {
			//if (params == nullptr)
			//	return nullptr;
			VarDeclAST *curVar = params->getVarDecl();
			context.builder.CreateStore(&arg, curVar->read(context));
			context.varDecls.push(arg.getName().data(), curVar);
			params = params->getNext();
		}
	}
	llvm::Value *returnValue = _expr->codegen(context);
	if (returnValue) {
		if (_funcTy->getResultType()->isVoidTy()) {
			context.builder.CreateRetVoid();
		}
		else {
			context.builder.CreateRet(returnValue);
		}
		if (!llvm::verifyFunction(*func, &llvm::errs())) {
			context.varDecls.exit();
			context.builder.SetInsertPoint(oldBlock);
			context.currentFrame = oldFrame;
			context.staticLink.pop_front();
			context.currentLevel -= 1;
			return func;
		}
	}
	context.varDecls.exit();

	func->eraseFromParent();
	context.funcDecls.pop(_name);
	context.builder.SetInsertPoint(oldBlock);
	context.staticLink.pop_front();
	context.currentFrame = oldFrame;
	context.currentLevel -= 1;
	return context.errorValueMsg("Codegen of Function " + _name + " fails!");
}

//===----------------------------------------------------------------------===//
// Expression
//===----------------------------------------------------------------------===//

llvm::Value *FuncExprAST::codegen(CodeGen &context) {
	FuncDeclAST* funcDecl = context.funcDecls[_func_name];
	llvm::Function *function;
	llvm::Function::LinkageTypes linkageType;
	size_t curLevel = 0u;
	if (funcDecl) {
		function = funcDecl->getFuncTy().getFunction();
		linkageType = function->getLinkage();
		curLevel = funcDecl->getLevel();
	}
	else {
		function = context.functions[_func_name];
		if (!function)
			return context.errorValueMsg("Cannot find function " + _func_name);
		linkageType = llvm::Function::ExternalLinkage;
	}
	std::vector<llvm::Value *> args;
	if (linkageType == llvm::Function::InternalLinkage) {
		size_t _currentLevel = context.currentLevel;
		auto staticLink = context.staticLink.begin();
		llvm::Value *value = context.currentFrame;
		while (_currentLevel >= curLevel) {
			value = context.builder.CreateGEP(llvm::PointerType::getUnqual(*++staticLink), value, context.zero, "staticLink");
			value = context.builder.CreateLoad(value, "frame");
			--_currentLevel;
		}
		args.push_back(value);
	}
	ExprListAST* exprList = _expr_list;
	ExprAST* exprAST;
	while (exprList) {
		exprAST = exprList->getExpr();
		args.push_back(exprAST->codegen(context));
		if (!args.back())
			return nullptr;
		exprList = exprList->getNext();
	}
	if (function->getFunctionType()->getReturnType()->isVoidTy()) {
		return context.builder.CreateCall(function, args);
	}
	else {
		return context.builder.CreateCall(function, args, "calltmp");
	}
}

llvm::Value *IntConstExprAST::codegen(CodeGen &context) {
	return llvm::ConstantInt::get(context.context, llvm::APInt(64, _value));
}

llvm::Value *StrConstExprAST::codegen(CodeGen &context) {
	return context.builder.CreateGlobalStringPtr(_value, "str");
}

llvm::Value *NilExprAST::codegen(CodeGen &context) {
	return llvm::ConstantPointerNull::get(context.nilType);
}

llvm::Value *VarExprAST::codegen(CodeGen &context) {
	llvm::Value *value = _value->codegen(context);
	if (!value)
		return nullptr;
	return context.builder.CreateLoad(value, value->getName());
}

llvm::Value *UnaryExprAST::codegen(CodeGen &context) {
	if (_op[0] != '-')
		return context.errorValueMsg("The op of Unary Expr must be '-'!");
	llvm::Value *V = _expr->codegen(context);
	if (!V)
		return nullptr;
	return context.builder.CreateNeg(V, "negtmp");
}

llvm::Value *BinaryExprAST::codegen(CodeGen &context) {
	llvm::Value *L = _expr_left->codegen(context);
	llvm::Value *R = _expr_right->codegen(context);
	if (!L || !R)
		return nullptr;
	switch (_op[0]) {
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
	case '=':
		L = context.handleNil(L, R);
		R = context.handleNil(R, L);
		if (L->getType() == context.stringType) {
			L = context.strcmp(L, R);
			R = context.zero;
		}
		return context.builder.CreateZExt(context.builder.CreateICmpEQ(L, R, "cmptmp"), context.intType, "cmptmp");
	case '>':
		if (_op.length() > 1) {
			if (L->getType() == context.stringType) {
				L = context.strcmp(L, R);
				R = context.zero;
			}
			return context.builder.CreateZExt(context.builder.CreateICmpSGE(L, R, "cmptmp"), context.intType, "cmptmp");
		}
		else {
			if (L->getType() == context.stringType) {
				L = context.strcmp(L, R);
				R = context.zero;
			}
			return context.builder.CreateZExt(context.builder.CreateICmpSGT(L, R, "cmptmp"), context.intType, "cmptmp");
		}
	case '<':
		if (_op.length() > 1) {
			if (_op[1] == '=') {
				if (L->getType() == context.stringType) {
					L = context.strcmp(L, R);
					R = context.zero;
				}
				return context.builder.CreateZExt(context.builder.CreateICmpSLE(L, R, "cmptmp"), context.intType, "cmptmp");
			}
			else {
				L = context.handleNil(L, R);
				R = context.handleNil(R, L);
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
		else {
			if (L->getType() == context.stringType) {
				L = context.strcmp(L, R);
				R = context.zero;
			}
			return context.builder.CreateZExt(context.builder.CreateICmpSLT(L, R, "cmptmp"), context.intType, "cmptmp");
		}
	}

	return nullptr;
}

llvm::Value *AssignExprAST::codegen(CodeGen &context) {
	llvm::Value *varValue = _var->codegen(context);
	if (!varValue)
		return nullptr;
	llvm::Value *expValue = _expr->codegen(context);
	if (!expValue)
		return nullptr;
	context.getStore(expValue, varValue);
	return expValue;
}

llvm::Value *SequenceExprAST::codegen(CodeGen &context) {
	llvm::Value *value = nullptr;
	ExprListAST* exprList = _expr_list;
	ExprAST* curExpr;
	while (exprList) {
		curExpr = exprList->getExpr();
		value = curExpr->codegen(context);
		exprList = exprList->getNext();
	}
	return value;
}

llvm::Value *RecordDefineExprAST::codegen(CodeGen &context) {
	if (!type_)
		return nullptr;
	llvm::Type *elementType = context.getType(type_);
	auto size = context.module->getDataLayout().getTypeAllocSize(elementType);
	llvm::Value *value = context.builder.CreateCall(context.allocateRecordFunction, llvm::ConstantInt::get(context.intType, llvm::APInt(64, size)), "alloca");
	value = context.builder.CreateBitCast(value, type_, "record");
	size_t index = 0u;
	FieldListAST* fieldList = _field_list;
	ExprAST* curFieldExpr;
	llvm::Type *curType;
	while (fieldList) {
		curFieldExpr = fieldList->getExpr();
		llvm::Value *exprValue = curFieldExpr->codegen(context);
		if (!exprValue)
			return nullptr;
		curType = fieldList->getType();
		if (!curType)
			return nullptr;
		llvm::Value *elementPtr = context.builder.CreateGEP(curType, value, llvm::ConstantInt::get(llvm::Type::getInt64Ty(context.context), llvm::APInt(64, index)), "elementPtr");
		context.getStore(exprValue, elementPtr);
		index += 1;
		fieldList = fieldList->getNext();
	}
	return value;
}

llvm::Value *ArrayDefineExprAST::codegen(CodeGen &context) {
	llvm::Function* func = context.builder.GetInsertBlock()->getParent();
	llvm::Type *elementType = context.getType(type_);
	llvm::Value *sizeV = _size->codegen(context);
	llvm::Value *valueV = _value->codegen(context);
	auto elementSize = context.module->getDataLayout().getTypeAllocSize(elementType);
	llvm::Value *arrayPtr =
		context.builder.CreateCall(context.allocateArrayFunction, std::vector<llvm::Value *>{sizeV, llvm::ConstantInt::get(llvm::Type::getInt64Ty(context.context), llvm::APInt(64, elementSize))}, "alloca");
	arrayPtr = context.builder.CreateBitCast(arrayPtr, type_, "array");
	llvm::Value *zero = llvm::ConstantInt::get(context.context, llvm::APInt(64, 0, true));
	llvm::AllocaInst *indexPtr = context.getEntryBlockAllocate(func, llvm::Type::getInt64Ty(context.context), "index");
	context.builder.CreateStore(zero, indexPtr);
	llvm::BasicBlock *testBB = llvm::BasicBlock::Create(context.context, "test", func);
	llvm::BasicBlock *doBB = llvm::BasicBlock::Create(context.context, "do", func);
	llvm::BasicBlock *continueBB = llvm::BasicBlock::Create(context.context, "continue", func);
	llvm::BasicBlock *afterBB = llvm::BasicBlock::Create(context.context, "after", func);
	context.builder.CreateBr(testBB);
	context.builder.SetInsertPoint(testBB);
	llvm::Value *index = context.builder.CreateLoad(indexPtr, "index");
	llvm::Value *EndCond = context.builder.CreateICmpSLT(index, sizeV, "loopcond");
	context.builder.CreateCondBr(EndCond, doBB, afterBB);
	context.builder.SetInsertPoint(doBB);
	llvm::Value *elementPtr = context.builder.CreateGEP(elementType, arrayPtr, index, "elementPtr");
	context.getStore(valueV, elementPtr);
	context.builder.CreateBr(continueBB);
	context.builder.SetInsertPoint(continueBB);
	llvm::Value *nextVar = context.builder.CreateAdd(index, llvm::ConstantInt::get(context.context, llvm::APInt(64, 1)), "nextVar");
	context.builder.CreateStore(nextVar, indexPtr);
	context.builder.CreateBr(testBB);
	context.builder.SetInsertPoint(afterBB);
	return arrayPtr;
}

llvm::Value *IfExprAST::codegen(CodeGen &context) {
	llvm::Function* func = context.builder.GetInsertBlock()->getParent();
	llvm::Value* ifV = _if_expr->codegen(context);
	if (!ifV)
		return nullptr;
	ifV = context.builder.CreateICmpNE(ifV, context.zero, "ifcond");
	llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create(context.context, "then", func);
	llvm::BasicBlock *ElseBB = llvm::BasicBlock::Create(context.context, "else");
	llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(context.context, "ifcont");
	context.builder.CreateCondBr(ifV, ThenBB, ElseBB);
	context.builder.SetInsertPoint(ThenBB);
	llvm::Value* ThenV = _then_expr->codegen(context);
	if (!ThenV)
		return nullptr;
	context.builder.CreateBr(MergeBB);
	ThenBB = context.builder.GetInsertBlock();
	func->getBasicBlockList().push_back(ElseBB);
	context.builder.SetInsertPoint(ElseBB);
	llvm::Value* ElseV;
	if (_else_expr) {
		ElseV = _else_expr->codegen(context);
		if (!ElseV)
			return nullptr;
	}
	context.builder.CreateBr(MergeBB);
	ElseBB = context.builder.GetInsertBlock();
	func->getBasicBlockList().push_back(MergeBB);
	context.builder.SetInsertPoint(MergeBB);
	if (_else_expr && !ThenV->getType()->isVoidTy() && !ElseV->getType()->isVoidTy()) {
		llvm::PHINode *PN = context.builder.CreatePHI(ThenV->getType(), 2, "iftmp");
		ThenV = context.handleNil(ThenV, ElseV);
		ElseV = context.handleNil(ElseV, ThenV);
		PN->addIncoming(ThenV, ThenBB);
		PN->addIncoming(ElseV, ElseBB);
		return PN;
		;
	}
	else {
		return llvm::Constant::getNullValue(llvm::Type::getInt64Ty(context.context));
	}
}

llvm::Value *WhileExprAST::codegen(CodeGen &context) {
	llvm::Function* func = context.builder.GetInsertBlock()->getParent();
	llvm::BasicBlock *whileBB = llvm::BasicBlock::Create(context.context, "whileCond", func);
	llvm::BasicBlock *doBB = llvm::BasicBlock::Create(context.context, "do", func);
	llvm::BasicBlock *continueBB = llvm::BasicBlock::Create(context.context, "continue", func);
	llvm::BasicBlock *afterBB = llvm::BasicBlock::Create(context.context, "after", func);
	context.loopStack.push({ continueBB, afterBB });
	context.builder.CreateBr(whileBB);
	context.builder.SetInsertPoint(whileBB);
	llvm::Value* WhileV = _while_expr->codegen(context);
	if (!WhileV)
		return nullptr;
	llvm::Value* EndCond = context.builder.CreateICmpEQ(WhileV, context.zero, "loopcond");
	context.builder.CreateCondBr(EndCond, afterBB, doBB);
	context.builder.SetInsertPoint(doBB);
	if (!_do_expr->codegen(context))
		return nullptr;
	context.builder.CreateBr(continueBB);
	context.builder.SetInsertPoint(continueBB);
	context.builder.CreateBr(whileBB);
	context.builder.SetInsertPoint(afterBB);
	context.loopStack.pop();//
	return llvm::Constant::getNullValue(llvm::Type::getInt64Ty(context.context));
}

llvm::Value *ForExprAST::codegen(CodeGen &context) {
	llvm::Value *InitV = _init_expr->codegen(context);
	if (!InitV)
		return nullptr;
	if (!InitV->getType()->isIntegerTy())
		return context.errorValueMsg("Type of 'init' expr of 'for' expr should be integer!");
	llvm::Value *ToV = _to_expr->codegen(context);
	if (!ToV)
		return nullptr;
	if (!ToV->getType()->isIntegerTy())
		return context.errorValueMsg("Type of 'to' expr of 'for' expr should be integer!");
	llvm::Function* func = context.builder.GetInsertBlock()->getParent();
	context.builder.CreateStore(InitV, _varDecl->read(context));
	llvm::BasicBlock *testBB = llvm::BasicBlock::Create(context.context, "test", func);
	llvm::BasicBlock *doBB = llvm::BasicBlock::Create(context.context, "do", func);
	llvm::BasicBlock *continueBB = llvm::BasicBlock::Create(context.context, "continue", func);
	llvm::BasicBlock *afterBB = llvm::BasicBlock::Create(context.context, "after", func);
	context.loopStack.push({ continueBB, afterBB });
	context.builder.CreateBr(testBB);
	context.builder.SetInsertPoint(testBB);
	llvm::Value *EndCond = context.builder.CreateICmpSLE(context.builder.CreateLoad(_varDecl->read(context), _id), ToV, "loopcond");
	context.builder.CreateCondBr(EndCond, doBB, afterBB);
	context.builder.SetInsertPoint(doBB);
	VarDeclAST *var = context.varDecls[_id];
	if (var)
		context.varDecls.pop(_id);
	context.varDecls.push(_id, _varDecl);
	if (!_do_expr->codegen(context))
		return nullptr;
	context.builder.CreateBr(continueBB);
	context.builder.SetInsertPoint(continueBB);
	llvm::Value* nextVar = context.builder.CreateAdd(context.builder.CreateLoad(_varDecl->read(context), _id), llvm::ConstantInt::get(context.context, llvm::APInt(64, 1)), "nextVar");
	context.builder.CreateStore(nextVar, _varDecl->read(context));
	context.builder.CreateBr(testBB);
	context.builder.SetInsertPoint(afterBB);
	if (var)
		context.varDecls[_id] = var;
	else
		context.varDecls.pop(_id);
	context.loopStack.pop();
	return llvm::Constant::getNullValue(llvm::Type::getInt64Ty(context.context));
}

llvm::Value *BreakExprAST::codegen(CodeGen &context) {
	context.builder.CreateBr(std::get<1>(context.loopStack.top()));
	return llvm::Constant::getNullValue(llvm::Type::getInt64Ty(context.context));
}

llvm::Value *LetExprAST::codegen(CodeGen &context) {
	context.varDecls.enter();
	context.funcDecls.enter();

	DeclListAST* declList = _decl_list;
	DeclAST* curDecl;
	while (declList) {
		curDecl = declList->getDecl();
		curDecl->codegen(context);
		declList = declList->getNext();
	}

	ExprListAST* exprList = _expr_list;
	ExprAST* curExpr;
	llvm::Value *resultValue;
	while (exprList) {
		curExpr = exprList->getExpr();
		resultValue = curExpr->codegen(context);
		exprList = exprList->getNext();
	}

	context.funcDecls.exit();
	context.varDecls.exit();

	return resultValue;
}

//===----------------------------------------------------------------------===//
// CodeGen
//===----------------------------------------------------------------------===//

CodeGen::CodeGen() {}

//get the element type of exp
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

//check if the exp is record type
bool CodeGen::isRecord(llvm::Type *exp) {
	return exp->isPointerTy() && getType(exp)->isStructTy();
}

llvm::Type *CodeGen::typeOf(std::string const &name) {
	std::set<std::string> parentNames;
	return typeOf(name, parentNames);
}

//get the type of 'name'
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

llvm::Value *CodeGen::getStore(llvm::Value *val, llvm::Value *ptr) {
	//in case of val is nillType
	val = handleNil(val, ptr);
	return builder.CreateStore(val, ptr);
}

llvm::Type *CodeGen::errorTypeMsg(std::string const &msg) {
	errorOccurs = true;
	std::cerr << msg << std::endl;
	return nullptr;
}

llvm::Value *CodeGen::errorValueMsg(std::string const &msg) {
	errorOccurs = true;
	std::cerr << msg << std::endl;
	return nullptr;
}

llvm::Function *CodeGen::createBuiltinFunction(std::string const &name, llvm::Type *returnType, std::vector<llvm::Type *> const &args) {
	llvm::FunctionType *funcTy = llvm::FunctionType::get(returnType, args, false);
	llvm::Function *function = llvm::Function::Create(funcTy, llvm::Function::ExternalLinkage, name, module.get());
	functions.push(name, function);
	return function;
}

void CodeGen::builtin() {
	functions["print"] = createBuiltinFunction("Print", voidType, { stringType });
	functions["printi"] = createBuiltinFunction("Printi", voidType, { intType });
	functions["flush"] = createBuiltinFunction("Flush", voidType, {});
	functions["getchar"] = createBuiltinFunction("Getchar", stringType, {});
	functions["ord"] = createBuiltinFunction("Ord", intType, { stringType });
	functions["chr"] = createBuiltinFunction("Chr", stringType, { intType });
	functions["size"] = createBuiltinFunction("Size", intType, { stringType });
	functions["substring"] = createBuiltinFunction("Substring", stringType, { stringType, intType, intType });
	functions["concat"] = createBuiltinFunction("Concat", stringType, { stringType, stringType });
	functions["not"] = createBuiltinFunction("Not", intType, { intType });
	functions["exit"] = createBuiltinFunction("Exit", voidType, { intType });
}

llvm::Value *CodeGen::strcmp(llvm::Value *a, llvm::Value *b) {
	return builder.CreateCall(strcmpFunction, std::vector<llvm::Value *>{a, b}, "strcmp");
}
