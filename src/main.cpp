#include <parser/parser.h>

int main() {
    std::string dir("tiger/");
    std::string name("test");

    AST *ast = TigerParser::parse(dir + name + ".tiger");
    ast->saveDot(dir + name + ".dot");
    delete ast;
}