#include "AST.h"

#include <llvm/IR/Value.h>
#include <llvm/IR/Function.h>

llvm::Value* DeclListAST::codegen() const
{
    return nullptr;
}

llvm::Value* ExprListAST::codegen() const
{
    return nullptr;
}

llvm::Value* TypeFieldsAST::codegen() const
{
    return nullptr;
}

llvm::Value* FieldListAST::codegen() const
{
    return nullptr;
}

llvm::Value* SimpleVarAST::codegen() const
{
    return nullptr;
}

llvm::Value* FieldVarAST::codegen() const
{
    return nullptr;
}

llvm::Value* IndexVarAST::codegen() const
{
    return nullptr;
}

llvm::Value* SimpleTypeAST::codegen() const
{
    return nullptr;
}

llvm::Value* RecordTypeAST::codegen() const
{
    return nullptr;
}

llvm::Value* ArrayTypeAST::codegen() const
{
    return nullptr;
}

llvm::Value* VarDeclAST::codegen() const
{
    return nullptr;
}

llvm::Value* TypeDeclAST::codegen() const
{
    return nullptr;
}

llvm::Value* FuncDeclAST::codegen() const
{
    return nullptr;
}

llvm::Value* IntConstExprAST::codegen() const
{
    return nullptr;
}

llvm::Value* StrConstExprAST::codegen() const
{
    return nullptr;
}

llvm::Value* NilExprAST::codegen() const
{
    return nullptr;
}

llvm::Value* VarExprAST::codegen() const
{
    return nullptr;
}

llvm::Value* UnaryExprAST::codegen() const
{
    return nullptr;
}

llvm::Value* BinaryExprAST::codegen() const
{
    return nullptr;
}

llvm::Value* AssignExprAST::codegen() const
{
    return nullptr;
}

llvm::Function* FuncExprAST::codegen() const
{
    return nullptr;
}

llvm::Value* SequenceExprAST::codegen() const
{
    return nullptr;
}

llvm::Value* RecordDefineExprAST::codegen() const
{
    return nullptr;
}

llvm::Value* ArrayDefineExprAST::codegen() const
{
    return nullptr;
}

llvm::Value* IfExprAST::codegen() const
{
    return nullptr;
}

llvm::Value* WhileExprAST::codegen() const
{
    return nullptr;
}

llvm::Value* ForExprAST::codegen() const
{
    return nullptr;
}

llvm::Value* BreakExprAST::codegen() const
{
    return nullptr;
}

llvm::Value* LetExprAST::codegen() const
{
    return nullptr;
}
