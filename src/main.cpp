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
		//the root must be an expression
		ExprAST *exprAST = dynamic_cast<ExprAST *>(ast);
		//package the whole expression into a main function, return type "int"
		MainAST *mainAST = new MainAST(exprAST);
		CodeGen context;
		//code generation
		mainAST->codegen(context);
		std::cout << "success" << std::endl;
	}

	delete ast;
}