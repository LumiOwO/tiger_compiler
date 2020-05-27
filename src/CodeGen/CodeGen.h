#ifndef CODEGEN_H
#define CODEGEN_H

#include <iostream>
#include <set>
#include <llvm/IR/Constant.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

#include "SymbolTable.h"
#include "AST/AST.h"

class CodeGen {
public:
	llvm::LLVMContext context;
	llvm::IRBuilder<> builder{ context };
	std::unique_ptr<llvm::Module> module{ std::make_unique<llvm::Module>("main", context) };

	llvm::Type *voidType{ llvm::Type::getVoidTy(context) };
	llvm::Type *intType{ llvm::Type::getInt64Ty(context) };
	llvm::PointerType *stringType{ llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)) };
	llvm::PointerType *nilType{ llvm::PointerType::getUnqual(llvm::StructType::get(context)) };
	llvm::Value *zero{ llvm::ConstantInt::get(intType, llvm::APInt(64, 0)) };

	//for llvm::Type generated in traversal
	SymbolTable<llvm::Type> types;
	//for llvm::Function generated in traversal
	SymbolTable<llvm::Function> functions;
	//TypeAST, types defined in tiger code
	SymbolTable<TypeAST> typeDecls;
	//VarDeclAST, varaible declared in tiger code
	SymbolTable<VarDeclAST> varDecls;
	//FuncDeclAST, functions declared in tiger code
	SymbolTable<FuncDeclAST> funcDecls;

	std::deque<llvm::StructType *> staticLink;
	llvm::AllocaInst *currentFrame;
	size_t currentLevel = 0u;
	bool errorOccurs = false;
	
	CodeGen();
	llvm::Type *getType(llvm::Type *exp);
	bool typeMatch(llvm::Type *a, llvm::Type *b);
	bool isRecord(llvm::Type *exp);
	llvm::Type *typeOf(std::string const &name);
	llvm::Type *typeOf(std::string const &name, std::set<std::string> &parentName);
	llvm::AllocaInst *getEntryBlockAllocate(llvm::Function *function, llvm::Type *type, std::string const &name, llvm::Value *size = nullptr);
	llvm::Value *handleNil(llvm::Value *val, llvm::Value *ptr);
	llvm::Value *getStore(llvm::Value *val, llvm::Value *ptr);
	llvm::Type *errorTypeMsg(std::string const &msg);
	llvm::Value *errorValueMsg(std::string const &msg);
};

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

llvm::AllocaInst *CodeGen::getEntryBlockAllocate(llvm::Function *function, llvm::Type *type, std::string const &name, llvm::Value *size = nullptr) {
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

#endif