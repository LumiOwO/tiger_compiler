#include "parser/parser.h"

int main() {
	llvm::InitializeNativeTarget();
	llvm::InitializeNativeTargetAsmPrinter();
	llvm::InitializeNativeTargetAsmParser();

	std::string dir("tiger/");
	std::string name("test");

	AST *ast = TigerParser::parse(dir + name + ".tiger");
	ast->saveDot(dir + name + ".dot");

	if (ast) {
		ExprAST *exprAST = dynamic_cast<ExprAST *>(ast);
		//package the whole expression in a main function, return type "int"
		MainAST *mainAST = new MainAST(exprAST);
		CodeGen context;
		mainAST->codegen(context);
		std::cout << "success" << std::endl;
	}

	delete ast;
}