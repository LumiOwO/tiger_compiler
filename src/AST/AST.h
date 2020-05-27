#pragma once

//#define SHUZIXI_DEBUG
#ifdef SHUZIXI_DEBUG
#define debug_print(x) \
    std::cout << "function " << __FUNCTION__ << " --- " << x << std::endl
#else
#define debug_print(x)
#endif // SHUZIXI_DEBUG

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "CodeGen/CodeGen.h"

namespace llvm {
    class Value;
    class Function;
	class Type;
};

//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//

class VarDeclAST;

/// Base class for AST, providing interfaces for visualization
class AST {
public:
    virtual ~AST() = default;

    // Label for AST node
    virtual std::string label() const = 0;
    // Transform the AST to string in order to visualize
    virtual std::string toString() const = 0;


    // Utils for Dot language when visualizing
    static std::string nodename(const void* addr) {
        return "Node_0x" + std::to_string((unsigned)addr);
    }
    static std::string labelInfo(const int &i) {
        std::ostringstream o;
        o << nodename(&i) << " [label = \"" << i << "\"];";
        return o.str();
    }
    static std::string labelInfo(const std::string &s) {
        std::ostringstream o;
        o << nodename(&s) << " [label = \"" << s << "\"];";
        return o.str();
    }
    std::string labelInfo() const {
        std::ostringstream o;
        o << nodename(this) << " [label = \"" << label() << "\"];";
        return o.str();
    }
    std::string edgeInfo(const void* child) const {
        return nodename(this) + " -> " + nodename(child) + ";";
    }

    void saveDot(std::string filepath) const {
        std::ofstream o(filepath);
        o << "digraph AST {" << std::endl
            << "fontname = \"Consolas\";" << std::endl
            << "fontsize = 16;" << std::endl
            << "node[shape = circle, fontname = \"Consolas\", fontsize = 16];" << std::endl
            << "edge[fontname = \"Verdana\", fontsize = 10, "
               "color = \"crimson\", style = \"solid\"];" << std::endl
            << "node[shape = \"plaintext\"];" << std::endl << std::endl
            << (*this) << "\n}";
        o.close();
        std::cout << "AST decription file saved to \"" << filepath << "\"\n";
    }

    friend std::ostream& operator<<(std::ostream &o, const AST& ast) {
        return o << ast.toString();
    }

#define CHILD_INFO_STREAM_AST(ast) \
    std::endl << edgeInfo(ast) << std::endl << ast->toString()
#define CHILD_INFO_STREAM_STR(s) \
    std::endl << edgeInfo(&s) << std::endl << labelInfo(s)
};

//===----------------------------------------------------------------------===//
// Base AST
//===----------------------------------------------------------------------===//

class VarAST : public AST {
public:
    virtual ~VarAST() = default;
	virtual llvm::Value *codegen(CodeGen &context) = 0;
	virtual llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) = 0;
};

class DeclAST : public AST {
public:
    virtual ~DeclAST() = default;
	virtual llvm::Value *codegen(CodeGen &context) = 0;
	virtual llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) = 0;
};

class TypeAST : public AST {
protected:
	std::string _name;
public:
	const std::string &getName() const { return _name; }
	void setName(std::string name) { _name = name; }
    virtual ~TypeAST() = default;
	virtual llvm::Type *traverse(std::set<std::string> &parentName, CodeGen &context) = 0;
};

class ExprAST : public AST {
public:
    virtual ~ExprAST() = default;
	virtual llvm::Value *codegen(CodeGen &context) = 0;
	virtual llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) = 0;
};

//===----------------------------------------------------------------------===//
// Entry for codegen
//===----------------------------------------------------------------------===//

class MainAST {
	ExprAST* _mainExpr;
	//AST* _mainExpr;
	std::vector<VarDeclAST *> mainVarTable;

public:
	MainAST(ExprAST* mainExpr): _mainExpr(mainExpr) {}
	llvm::Value *codegen(CodeGen &context);
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context);
};

//===----------------------------------------------------------------------===//
// List 
// Note that these lists are not subclass of 4 kinds base AST
//===----------------------------------------------------------------------===//

class DeclListAST : public AST {
private:
    DeclAST*        _decl;      // not null
    DeclListAST*    _next;      // can be null
public:
    DeclListAST(DeclAST* decl, DeclListAST* next):
        _decl(decl), _next(next) {}
    virtual ~DeclListAST() {
        delete _decl;
        if (hasNext()) delete _next;
    }

	DeclAST* getDecl() { return _decl; }
	DeclListAST* getNext() { return _next; }

    llvm::Value *codegen(CodeGen &context);
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context);

    virtual std::string label() const override { return "DeclList"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo() 
            << CHILD_INFO_STREAM_AST(_decl);
        if (hasNext()) {
            o << CHILD_INFO_STREAM_AST(_next);
        }
        return o.str();
    }

    bool hasNext() const { return _next != nullptr; }
};

class ExprListAST : public AST {
private:
    ExprAST*        _expr;      // not null
    ExprListAST*    _next;      // can be null
public:
    ExprListAST(ExprAST* expr, ExprListAST* next) :
        _expr(expr), _next(next) {}
    virtual ~ExprListAST() {
        delete _expr;
        if (hasNext()) delete _next;
    }

	ExprAST* getExpr() const { return _expr; }
	ExprListAST* getNext() const { return _next; }

    llvm::Value *codegen(CodeGen &context);
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context);

    virtual std::string label() const override { return "ExprList"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo()
            << CHILD_INFO_STREAM_AST(_expr);
        if (hasNext()) {
            o << CHILD_INFO_STREAM_AST(_next);
        }
        return o.str();
    }

    bool hasNext() const { return _next != nullptr; }
};

// Example: { id: type-id, id: type-id }
class TypeFieldsAST : public AST {
private:
    std::string     _id;          
    std::string     _type_id;
    TypeFieldsAST*  _next;        // can be null

	llvm::Type *type_{ nullptr };
	VarDeclAST *varDecl_{ nullptr };
public:
    TypeFieldsAST(const std::string id, const std::string type_id, TypeFieldsAST* next) :
        _id(id), _type_id(type_id), _next(next) {}
    virtual ~TypeFieldsAST() {
        if (hasNext()) delete _next;
    }

	std::string &getId() { return _id; }
	std::string &getTypeID() { return _type_id; }
	TypeFieldsAST* getNext() { return _next; }
	llvm::Type *getType() { return type_; }
	VarDeclAST *getVarDecl() const { return varDecl_; }
	void setType(llvm::Type *type) { type_ = type; }

    llvm::Value *codegen(CodeGen &context);
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context);

    virtual std::string label() const override { return "TypeFields"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo()
            << CHILD_INFO_STREAM_STR(_id)
            << CHILD_INFO_STREAM_STR(_type_id);
        if (hasNext()) {
            o << CHILD_INFO_STREAM_AST(_next);
        }
        return o.str();
    }

    bool hasNext() const { return _next != nullptr; }
};

// Example: { id = expr, id = expr }
class FieldListAST : public AST {
private:
    std::string     _id;
    ExprAST*        _expr;      // not null
    FieldListAST*   _next;      // can be null

	llvm::Type *type_{ nullptr };
public:
    FieldListAST(const std::string id, ExprAST* expr, FieldListAST* next) :
        _id(id), _expr(expr), _next(next) {}
    virtual ~FieldListAST() {
        delete _expr;
        if (hasNext()) delete _next;
    }

	std::string &getId() { return _id; }
	ExprAST* getExpr()const { return _expr; }
	FieldListAST* getNext() const { return _next; }

    llvm::Value *codegen(CodeGen &context);
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context);

    virtual std::string label() const override { return "FieldList"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo()
            << CHILD_INFO_STREAM_STR(_id)
            << CHILD_INFO_STREAM_AST(_expr);
        if (hasNext()) {
            o << CHILD_INFO_STREAM_AST(_next);
        }
        return o.str();
    }

    bool hasNext() const { return _next != nullptr; }
};

//===----------------------------------------------------------------------===//
// Variable
//===----------------------------------------------------------------------===//

// name
// example: student
class SimpleVarAST : public VarAST {
private:
    std::string     _name;      // name of variable
public:
    SimpleVarAST(const std::string name) :
        _name(name) {}
    virtual ~SimpleVarAST() = default;

    llvm::Value *codegen(CodeGen &context) override;
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) override;

    virtual std::string label() const override { return _name; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo();
        return o.str();
    }
};

// var.field
// Example: student.age
class FieldVarAST : public VarAST {
private:
    VarAST*         _var;       // not null
    std::string     _field;     // name of field
	size_t varIndex{ 0u };
	llvm::Type *type_{ nullptr };
	size_t index{0u};
public:
    FieldVarAST(VarAST* var, const std::string field) :
        _var(var), _field(field) {}
    virtual ~FieldVarAST() {
        delete _var;
    }

    llvm::Value *codegen(CodeGen &context) override;
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) override;

    virtual std::string label() const override { return "FieldVar"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo()
            << CHILD_INFO_STREAM_AST(_var)
            << CHILD_INFO_STREAM_STR(_field);
        return o.str();
    }
};

// var[index]
// Example: array[0]
class IndexVarAST : public VarAST {
private:
    VarAST*         _var;       // not null
    ExprAST*        _index;     // not null

	llvm::Type *type_{ nullptr };
public:
    IndexVarAST(VarAST* var, ExprAST* index) :
        _var(var), _index(index) {}
    virtual ~IndexVarAST() {
        delete _var;
        delete _index;
    }

    llvm::Value *codegen(CodeGen &context) override;
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) override;

    virtual std::string label() const override { return "IndexVar"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo()
            << CHILD_INFO_STREAM_AST(_var)
            << CHILD_INFO_STREAM_AST(_index);
        return o.str();
    }
};

//===----------------------------------------------------------------------===//
// Type
//===----------------------------------------------------------------------===//

// type-id
// Example: int
class SimpleTypeAST : public TypeAST {
private:
    std::string     _type_id;
public:
    SimpleTypeAST(const std::string type_id) :
        _type_id(type_id) {}
    virtual ~SimpleTypeAST() = default;

	const std::string &getTypeId() const { return _type_id; }

	llvm::Type *traverse(std::set<std::string> &parentName, CodeGen &context) override;

    virtual std::string label() const override { return _type_id; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo();
        return o.str();
    }
};

// { type-fields }
// Example: {a: int, b: string}
class RecordTypeAST : public TypeAST {
private:
    TypeFieldsAST*  _type_fields;   // not null
public:
    RecordTypeAST(TypeFieldsAST* type_fields) :
        _type_fields(type_fields) {}
    virtual ~RecordTypeAST() {
        delete _type_fields;
    }

	TypeFieldsAST *getTypeFields() { return _type_fields; }

	llvm::Type *traverse(std::set<std::string> &parentName, CodeGen &context) override;

    virtual std::string label() const override { return "RecordType"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo()
            << CHILD_INFO_STREAM_AST(_type_fields);
        return o.str();
    }
};

// array of type-id
// Example: array of int
class ArrayTypeAST : public TypeAST {
private:
    std::string     _type_id;
public:
    ArrayTypeAST(const std::string type_id) :
        _type_id(type_id) {}
    virtual ~ArrayTypeAST() = default;

	llvm::Type *traverse(std::set<std::string> &parentName, CodeGen &context) override;

    virtual std::string label() const override { return "ArrayType"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo()
            << CHILD_INFO_STREAM_STR(_type_id);
        return o.str();
    }
};

//===----------------------------------------------------------------------===//
// Declaration
//===----------------------------------------------------------------------===//

// var id := expr
// var id : type_id := expr
// Example: var a := 1
class VarDeclAST : public DeclAST {
private:
    std::string     _id;
	SimpleTypeAST*  _type{nullptr};      // can be null
    ExprAST*        _expr;      // not null

	llvm::Type *type_{ nullptr };
	size_t offset_;
	size_t level_;
public:
    VarDeclAST(const std::string id, SimpleTypeAST* type, ExprAST* expr) :
        _id(id), _type(type), _expr(expr) {}
	VarDeclAST(const std::string id, llvm::Type *type, size_t const &offset, size_t const &level) :
		_id(id), type_(type), offset_(offset), level_(level) {}

    virtual ~VarDeclAST() {
        if (hasType()) delete _type;
        delete _expr;
    }

	const std::string &getId() const { return _id; }
	llvm::Type *getType() const { return type_; }

    llvm::Value *codegen(CodeGen &context) override;
	llvm::Value *read(CodeGen &context) const;
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) override;

    virtual std::string label() const override { return "VarDecl"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo()
            << CHILD_INFO_STREAM_STR(_id);
        if (hasType()) {
            o << CHILD_INFO_STREAM_AST(_type);
        }
        o << CHILD_INFO_STREAM_AST(_expr);
        return o.str();
    }

    bool hasType() const { return _type != nullptr; }
};

// type type-id = type
// Example: type intArray = array of int
class TypeDeclAST : public DeclAST {
private:
    std::string     _type_id;   
    TypeAST*        _type;      // not null
public:
    TypeDeclAST(const std::string type_id, TypeAST* type) :
        _type_id(type_id), _type(type) {}
    virtual ~TypeDeclAST() {
        delete _type;
    }

    llvm::Value *codegen(CodeGen &context) override;
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) override;

    virtual std::string label() const override { return "TypeDecl"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo()
            << CHILD_INFO_STREAM_STR(_type_id)
            << CHILD_INFO_STREAM_AST(_type);
        return o.str();
    }
};

class FuncTy {
	std::string _name;
	TypeFieldsAST*  _params;
	SimpleTypeAST* _returnType;
	llvm::Type *resultType_{ nullptr };
	llvm::Function *function_{ nullptr };
	llvm::StructType *frame{ nullptr };
	VarDeclAST *staticLink_{ nullptr };

public:
	FuncTy(std::string name, TypeFieldsAST *params, SimpleTypeAST *returnType)
		: _name(name), _params(params), _returnType(returnType) { }

	llvm::Function *codegen(CodeGen &context);
	llvm::FunctionType *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context);

	const std::string &getName() const { return _name; }
	void rename(std::string name) { _name = name; }
	TypeFieldsAST *getParams() const { return _params; }
	llvm::Type *getResultType() const { return resultType_; }
	llvm::StructType *getFrame() const { return frame; }
	llvm::Function *getFunction() const { return function_; }
	VarDeclAST *getStaticLink() const { return staticLink_; }
};

// function name (type-fields) = expr
// function name (type-fields) : type-id = expr
// Example: function f() = print("hello")
class FuncDeclAST : public DeclAST {
private:
    std::string     _name;
    TypeFieldsAST*  _type_fields;   // can be null
    SimpleTypeAST*  _type;          // can be null
    ExprAST*        _expr;          // not null
	size_t			_level{0u};
	FuncTy*			_funcTy;
	std::vector<VarDeclAST *> varTable_;
public:
    FuncDeclAST(const std::string name, TypeFieldsAST* type_fields,
        SimpleTypeAST* type, ExprAST* expr) :
        _name(name), _type_fields(type_fields), 
        _type(type), _expr(expr) {
		_funcTy = new FuncTy(name, type_fields, type);
	}
    virtual ~FuncDeclAST() {
        if (hasTypeFields()) delete _type_fields;
        if (hasType()) delete _type;
        delete _expr;
    }

	FuncTy &getFuncTy() const { return *_funcTy; }
	size_t getLevel() const { return _level; }

    llvm::Value *codegen(CodeGen &context) override;
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) override;

    virtual std::string label() const override { return "FuncDecl"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo()
            << CHILD_INFO_STREAM_STR(_name);
        if (hasTypeFields()) {
            o << CHILD_INFO_STREAM_AST(_type_fields);
        }
        if (hasType()) {
            o << CHILD_INFO_STREAM_AST(_type);
        }
        o << CHILD_INFO_STREAM_AST(_expr);
        return o.str();
    }

    bool hasType() const { return _type != nullptr; }
    bool hasTypeFields() const { return _type_fields != nullptr; }
};

//===----------------------------------------------------------------------===//
// Expression
//===----------------------------------------------------------------------===//

// integer-constant
// Example: 1234
class IntConstExprAST : public ExprAST {
private:
    int _value;
public:
    IntConstExprAST(const std::string value) :
        _value(std::stoi(value)) {}
    virtual ~IntConstExprAST() = default;

    llvm::Value *codegen(CodeGen &context) override;
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) override;

    virtual std::string label() const override { return std::to_string(_value); }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo();
        return o.str();
    }
};

// string-constant
// Example: "hello"
class StrConstExprAST : public ExprAST {
private:
    std::string _value;
public:
    StrConstExprAST(const std::string value, const std::string label_str) :
        _value(value), _label(label_str) {}
    virtual ~StrConstExprAST() = default;

    llvm::Value *codegen(CodeGen &context) override;
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) override;

    virtual std::string label() const override { return _label; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo();
        return o.str();
    }

private:
    std::string _label; // only used for visualization
};

// nil
class NilExprAST : public ExprAST {
public:
    NilExprAST() = default;
    virtual ~NilExprAST() = default;

    llvm::Value *codegen(CodeGen &context) override;
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) override;

    virtual std::string label() const override { return "nil"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo();
        return o.str();
    }
};

// lvalue
class VarExprAST : public ExprAST {
private:
    VarAST* _value;     // not null
public:
    VarExprAST(VarAST* value) :
        _value(value) {}
    virtual ~VarExprAST() { delete _value; }

    llvm::Value *codegen(CodeGen &context) override;
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) override;

    virtual std::string label() const override { return "VarExpr"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo()
            << CHILD_INFO_STREAM_AST(_value);
        return o.str();
    }
};

// - expr
class UnaryExprAST : public ExprAST {
private:
    std::string     _op;
    ExprAST*        _expr;     // not null

	llvm::Type *type_{ nullptr };
public:
    UnaryExprAST(const std::string op, ExprAST* expr) :
        _op(op), _expr(expr) {}
    virtual ~UnaryExprAST() { delete _expr; }

    llvm::Value *codegen(CodeGen &context) override;
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) override;

    virtual std::string label() const override { return "UnaryExpr"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo()
            << CHILD_INFO_STREAM_STR(_op)
            << CHILD_INFO_STREAM_AST(_expr);
        return o.str();
    }
};

// expr binary-operator expr
class BinaryExprAST : public ExprAST {
private:
    ExprAST*        _expr_left;     // not null
    std::string     _op;
    ExprAST*        _expr_right;    // not null
public:
    BinaryExprAST(ExprAST* expr_left, const std::string op, ExprAST* expr_right) :
        _expr_left(expr_left), _op(op), _expr_right(expr_right) {}
    virtual ~BinaryExprAST() { 
        delete _expr_left;
        delete _expr_right;
    }

    llvm::Value *codegen(CodeGen &context) override;
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) override;

    virtual std::string label() const override { return "BinaryExpr"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo()
            << CHILD_INFO_STREAM_AST(_expr_left)
            << CHILD_INFO_STREAM_STR(_op)
            << CHILD_INFO_STREAM_AST(_expr_right);
        return o.str();
    }
};

// lvalue: = expr
// Example: a := 3
class AssignExprAST : public ExprAST {
private:
    VarAST*     _var;     // not null
    ExprAST*    _expr;    // not null
public:
    AssignExprAST(VarAST* var, ExprAST* expr) :
        _var(var), _expr(expr) {}
    virtual ~AssignExprAST() {
        delete _var;
        delete _expr;
    }

    llvm::Value *codegen(CodeGen &context) override;
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) override;

    virtual std::string label() const override { return "AssignExpr"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo()
            << CHILD_INFO_STREAM_AST(_var)
            << CHILD_INFO_STREAM_AST(_expr);
        return o.str();
    }
};

// func_name(expr-list)
// Example: f() f(1, 2)
class FuncExprAST : public ExprAST {
private:
    std::string     _func_name;
    ExprListAST*    _expr_list;    // can be null
public:
    FuncExprAST(const std::string func_name, ExprListAST* expr_list) :
        _func_name(func_name), _expr_list(expr_list) {}
    virtual ~FuncExprAST() {
        if(hasExprList()) delete _expr_list;
    }

    llvm::Value *codegen(CodeGen &context) override;
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) override;

    virtual std::string label() const override { return "FuncExprAST"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo()
            << CHILD_INFO_STREAM_STR(_func_name);
        if (hasExprList()) {
            o << CHILD_INFO_STREAM_AST(_expr_list);
        }
        return o.str();
    }

    bool hasExprList() const { return _expr_list != nullptr; }
};

// ( expr-list ) , list can be null
// Example: (a := 1 ; b := 2)  ()
class SequenceExprAST : public ExprAST {
private:
    ExprListAST*    _expr_list;    // can be null
public:
    SequenceExprAST(ExprListAST* expr_list) :
        _expr_list(expr_list) {}
    virtual ~SequenceExprAST() {
        if (hasExprList()) delete _expr_list;
    }

    llvm::Value *codegen(CodeGen &context) override;
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) override;

    virtual std::string label() const override { return "SequenceExpr"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo();
        if (hasExprList()) {
            o << CHILD_INFO_STREAM_AST(_expr_list);
        }
        return o.str();
    }

    bool hasExprList() const { return _expr_list != nullptr; }
};

// type-id { field-list } , list can be null
// Example:  human { age = 18, height = 170 }
class RecordDefineExprAST : public ExprAST {
private:
    std::string     _type_id;
    FieldListAST*   _field_list;    // can be null

	llvm::Type *type_{ nullptr };
public:
    RecordDefineExprAST(const std::string type_id, FieldListAST* field_list) :
        _type_id(type_id), _field_list(field_list) {}
    virtual ~RecordDefineExprAST() {
        if (hasFieldList()) delete _field_list;
    }

	FieldListAST* getFieldList() { return _field_list; }

    llvm::Value *codegen(CodeGen &context) override;
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) override;

    virtual std::string label() const override { return "RecordDefineExpr"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo()
            << CHILD_INFO_STREAM_STR(_type_id);
        if (hasFieldList()) {
            o << CHILD_INFO_STREAM_AST(_field_list);
        }
        return o.str();
    }

    bool hasFieldList() const { return _field_list != nullptr; }
};

// type-id [size] of value
// Example: int[3] of 0
class ArrayDefineExprAST : public ExprAST {
private:
    std::string     _type_id;
    ExprAST*        _size;          // not null
    ExprAST*        _value;         // not null

	llvm::Type *type_{ nullptr };
public:
    ArrayDefineExprAST(const std::string type_id, ExprAST* size, ExprAST* value) :
        _type_id(type_id), _size(size), _value(value) {}
    virtual ~ArrayDefineExprAST() {
        delete _size;
        delete _value;
    }

    llvm::Value *codegen(CodeGen &context) override;
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) override;

    virtual std::string label() const override { return "ArrayDefineExpr"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo()
            << CHILD_INFO_STREAM_STR(_type_id)
            << CHILD_INFO_STREAM_AST(_size)
            << CHILD_INFO_STREAM_AST(_value);
        return o.str();
    }
};

// if expr then expr
// if expr then expr else expr
class IfExprAST : public ExprAST {
private:
    ExprAST*        _if_expr;       // not null
    ExprAST*        _then_expr;     // not null
    ExprAST*        _else_expr;     // can be null
public:
    IfExprAST(ExprAST* if_expr, ExprAST* then_expr, ExprAST* else_expr) :
        _if_expr(if_expr), _then_expr(then_expr), _else_expr(else_expr) {}
    virtual ~IfExprAST() {
        delete _if_expr;
        delete _then_expr;
        if (hasElseExpr()) delete _else_expr;
    }

    llvm::Value *codegen(CodeGen &context) override;
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) override;

    virtual std::string label() const override { return "IfExpr"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo()
            << CHILD_INFO_STREAM_AST(_if_expr)
            << CHILD_INFO_STREAM_AST(_then_expr);
        if (hasElseExpr()) {
            o << CHILD_INFO_STREAM_AST(_else_expr);
        }
        return o.str();
    }

    bool hasElseExpr() const { return _else_expr != nullptr; }
};

// while expr do expr
class WhileExprAST : public ExprAST {
private:
    ExprAST*        _while_expr;    // not null
    ExprAST*        _do_expr;       // not null
public:
    WhileExprAST(ExprAST* while_expr, ExprAST* do_expr) :
        _while_expr(while_expr), _do_expr(do_expr) {}
    virtual ~WhileExprAST() {
        delete _while_expr;
        delete _do_expr;
    }

    llvm::Value *codegen(CodeGen &context) override;
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) override;

    virtual std::string label() const override { return "WhileExpr"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo()
            << CHILD_INFO_STREAM_AST(_while_expr)
            << CHILD_INFO_STREAM_AST(_do_expr);
        return o.str();
    }
};

// for id := expr to expr do expr
class ForExprAST : public ExprAST {
private:
    std::string     _id;           
    ExprAST*        _init_expr;     // not null 
    ExprAST*        _to_expr;       // not null
    ExprAST*        _do_expr;       // not null

	VarDeclAST*		_varDecl{ nullptr };
public:
    ForExprAST(const std::string id, ExprAST* init_expr, ExprAST* to_expr, ExprAST* do_expr) :
        _id(id), _init_expr(init_expr), _to_expr(to_expr), _do_expr(do_expr) {}
    virtual ~ForExprAST() {
        delete _init_expr;
        delete _to_expr;
        delete _do_expr;
    }

    llvm::Value *codegen(CodeGen &context) override;
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) override;

    virtual std::string label() const override { return "ForExpr"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo()
            << CHILD_INFO_STREAM_STR(_id)
            << CHILD_INFO_STREAM_AST(_init_expr)
            << CHILD_INFO_STREAM_AST(_to_expr)
            << CHILD_INFO_STREAM_AST(_do_expr);
        return o.str();
    }
};

// break
class BreakExprAST : public ExprAST {
public:
    BreakExprAST() = default;
    virtual ~BreakExprAST() = default;

    llvm::Value *codegen(CodeGen &context) override;
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) override;

    virtual std::string label() const override { return "break"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo();
        return o.str();
    }
};

// let declaration-list in expr-list end
// Example: 
//    let 
//        var a := 1
//        var b := 2
//    in
//        a + b
//    end
class LetExprAST : public ExprAST {
private:
    DeclListAST*    _decl_list;     // not null 
    ExprListAST*    _expr_list;     // can be null
public:
    LetExprAST(DeclListAST* decl_list, ExprListAST* expr_list) :
        _decl_list(decl_list), _expr_list(expr_list) {}
    virtual ~LetExprAST() {
        delete _decl_list;
        if (hasExprList()) delete _expr_list;
    }

    llvm::Value *codegen(CodeGen &context) override;
	llvm::Type *traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) override;

    virtual std::string label() const override { return "LetExpr"; }
    virtual std::string toString() const override {
        std::ostringstream o;
        o << labelInfo()
            << CHILD_INFO_STREAM_AST(_decl_list);
        if (hasExprList()) {
            o << CHILD_INFO_STREAM_AST(_expr_list);
        }
        return o.str();
    }

    bool hasExprList() const { return _expr_list != nullptr; }
};

// AST node for yylex
class LexAST : public AST {
public:
    std::string text;
    LexAST(const char* yytext): text(yytext) {}
    virtual ~LexAST() = default;
  
    virtual std::string label() const override { return "LexAST"; }
    virtual std::string toString() const override { return ""; }
};