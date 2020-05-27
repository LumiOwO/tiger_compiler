#include "AST.h"

#include <llvm/IR/Value.h>
#include <llvm/IR/Function.h>

llvm::Value *MainAST::codegen(CodeGen &context) {
	// clear context and context.builder
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
	auto mainFuncTy = llvm::FunctionType::get(llvm::Type::getInt64Ty(context.context), llvm::makeArrayRef(params), false);
	auto mainFun = llvm::Function::Create(mainFuncTy, llvm::GlobalValue::ExternalLinkage, "main", context.module.get());
	context.staticLink.push_front(llvm::StructType::create(context.context, "main"));
	auto basicBlock = llvm::BasicBlock::Create(context.context, "entry", mainFun);

	context.types["int"] = context.intType;
	context.types["string"] = context.stringType;
	//type check and generate
	traverse(mainVarTable, context);
	if (context.errorOccurs) {
		return nullptr;
	}

	context.varDecls.resetTable();
	context.funcDecls.resetTable();
	context.builder.SetInsertPoint(basicBlock);
	std::vector<llvm::Type *> localVars;
	for (auto &var : mainVarTable) {
		localVars.push_back(var->getType());
		context.varDecls.push(var->getId(), var);
	}
	//staticLink used for child function to access vars in parent function
	context.staticLink.front()->setBody(localVars);
	context.currentFrame = context.getEntryBlockAllocate(mainFun, context.staticLink.front(), "mainframe");
	context.currentLevel = 0;

	//the real expression in main function
	_mainExpr->codegen(context);

	//return 0
	context.builder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt64Ty(context.context), llvm::APInt(64, 0)));
	if (llvm::verifyFunction(*mainFun, &llvm::errs())) {
		return context.errorValueMsg("codegen() fails.");
	}
	std::cout << "Code generation finishes." << std::endl;

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
	TypeFieldsAST *typeField;
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
		return context.errorValueMsg("Function " + _name + "is already defined in this scope!");
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
			--context.currentLevel;
			return func;
		}
	}
	context.varDecls.exit();

	func->eraseFromParent();
	context.funcDecls.pop(_name);
	context.builder.SetInsertPoint(oldBlock);
	context.staticLink.pop_front();
	context.currentFrame = oldFrame;
	--context.currentLevel;
	return context.errorValueMsg("Codegen of Function" + _name + "fails!");
}

//===----------------------------------------------------------------------===//
// Expression
//===----------------------------------------------------------------------===//

llvm::Value *FuncExprAST::codegen(CodeGen &context) {
	FuncDeclAST* funcDecl = context.funcDecls[_func_name];
	llvm::Function *function;
	llvm::Function::LinkageTypes type;
	size_t curLevel = 0u;
	if (funcDecl) {
		function = funcDecl->getFuncTy().getFunction();
		type = function->getLinkage();
		curLevel = funcDecl->getLevel();
	}
	else {
		//built in function
		function = context.functions[_func_name];
		if (!function)
			return context.errorValueMsg("Function" + _func_name + "not found!");
		type = llvm::Function::ExternalLinkage;
	}
	std::vector<llvm::Value *> args;
	if (type == llvm::Function::InternalLinkage) {
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
	while (exprList) {
		args.push_back(exprList->getExpr()->codegen(context));
		if (!args.back())
			return nullptr;
		exprList = exprList->getNext();
	}
	if (function->getFunctionType()->getReturnType()->isVoidTy()) {
		return context.builder.CreateCall(function, args);
	}
	else {
		return context.builder.CreateCall(function, args, "call");
	}
}


