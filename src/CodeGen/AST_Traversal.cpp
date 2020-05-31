#include "AST/AST.h"

//traverse the whole AST for type check and generation

llvm::Type *MainAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	context.typeDecls.resetTable();
	return _mainExpr->traverse(mainVarTable, context);
}

//===----------------------------------------------------------------------===//
// Variable
//===----------------------------------------------------------------------===//

// name
// example: student
// _name = "student"
llvm::Type *SimpleVarAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	//look for the varaible in var declarations
	VarDeclAST *varDecl = context.varDecls[_name];
	if (varDecl)
		return varDecl->getType();
	else
		return context.errorTypeMsg(_name + "has not been defined!");
}

// var.field
// Example: student.age
// _var = student, _field = "age"
llvm::Type *FieldVarAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	//first check the var type, it should be record type
	llvm::Type *var = _var->traverse(varTable, context);
	if (!var)
		return nullptr;
	if (!context.isRecord(var))
		return context.errorTypeMsg("Field var can only be used by record type.");
	llvm::StructType *varType = llvm::cast<llvm::StructType>(context.getType(var));
	//get the type declaration of _var
	RecordTypeAST *typeDecl = dynamic_cast<RecordTypeAST *>(context.typeDecls[varType->getStructName().data()]);
	TypeFieldsAST *typeFields = typeDecl->getTypeFields();
	//the index of _field in the record
	index = 0u;
	while (typeFields) {
		if (typeFields->getId() == _field)
			break;
		else {
			typeFields = typeFields->getNext();
			index += 1;
		}
	}
	if (typeFields)
		type_ = typeFields->getType();
	return type_;
}

// var[index]
// Example: array[0]
// _var = array _index = 0
llvm::Type *IndexVarAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	//first check the var type, it should be array type
	llvm::Type *var = _var->traverse(varTable, context);
	if (!var)
		return nullptr;
	if (!var->isPointerTy() || context.getType(var)->isStructTy())
		return context.errorTypeMsg("Index var can only be used by array type.");
	type_ = context.getType(var);
	//the index should be integer type
	llvm::Type *exp = _index->traverse(varTable, context);
	if (!exp)
		return nullptr;
	if (!exp->isIntegerTy())
		return context.errorTypeMsg("Index must be an integer!");

	return type_;
}

//===----------------------------------------------------------------------===//
// Type
//===----------------------------------------------------------------------===//

// type-id
// Example: int
// _type_id = "int"
llvm::Type *SimpleTypeAST::traverse(std::set<std::string> &parentName, CodeGen &context) {
	//first check if the type has been generated, _name means alias, for example, a = int, "a" is _name
	llvm::Type *type = context.types[_name];
	if (type)
		return type;
	type = context.types[_type_id];
	if (type) {
		context.types.push(_name, type);
		return type;
	}
	if (parentName.find(_name) != parentName.end()) {
		return context.errorTypeMsg("An endless loop in type definition is detected.");
	}
	//the type is not found, then try to generate it using type declarations before
	parentName.insert(_name);
	type = context.typeOf(_type_id, parentName);
	parentName.erase(_name);
	return type;
}

// { type-fields }
// Example: {a: int, b: string}
llvm::Type *RecordTypeAST::traverse(std::set<std::string> &parentName, CodeGen &context) {
	//first check if the type has been generated, _name means alias
	llvm::Type *type = context.types[_name];
	if (type)
		return type;
	//if the record type has appeared before
	if (parentName.find(_name) != parentName.end()) {
		llvm::PointerType *type = llvm::PointerType::getUnqual(llvm::StructType::create(context.context, _name));
		context.types.push(_name, type);
		return type;
	}
	//to store all the field types
	std::vector<llvm::Type *> typeArr;
	//record type itself can also appear in its fields(ptr)
	parentName.insert(_name);
	TypeFieldsAST *typeFields = _type_fields;
	while (typeFields) {
		llvm::Type *type = context.typeOf(typeFields->getTypeID(), parentName);
		if (!type)
			return nullptr;
		typeFields->setType(type);
		typeArr.push_back(type);
		typeFields = typeFields->getNext();
	}
	parentName.erase(_name);
	type = context.types[_name];
	if (type) {
		//type found, check
		if (!type->isPointerTy())
			return nullptr;
		llvm::Type *elementType = context.getType(type);
		if (!elementType->isStructTy())
			return nullptr;
		llvm::cast<llvm::StructType>(elementType)->setBody(typeArr);
		return type;
	}
	else {
		//if type not found, generate
		type = llvm::PointerType::getUnqual(llvm::StructType::create(context.context, typeArr, _name));
		if (!type)
			return nullptr;
		context.types.push(_name, type);
		return type;
	}
}

// array of type-id
// Example: array of int
// _type_id = "int"
llvm::Type *ArrayTypeAST::traverse(std::set<std::string> &parentName, CodeGen &context) {
	//check if the type has been generated
	llvm::Type *type = context.types[_name];
	if (type)
		return type;
	if (parentName.find(_name) != parentName.end()) {
		return context.errorTypeMsg("An endless loop in type definition is detected.");
	}
	parentName.insert(_name);
	type = context.typeOf(_type_id, parentName);
	parentName.erase(_name);
	if (!type)
		return nullptr;
	//it should be pointer type
	type = llvm::PointerType::getUnqual(type);
	context.types.push(_name, type);
	return type;
}

//===----------------------------------------------------------------------===//
// Declaration
//===----------------------------------------------------------------------===//

// var id := expr
// var id : type_id := expr
// Example: var a : int := 1
// _id = "a", _type = int, _expr = 1
llvm::Type *VarDeclAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	//find var declaration in current scope
	if (context.varDecls.find(_id)) {
		return context.errorTypeMsg(_id + "is already defined!");
	}
	// Corresponds to current scope
	level_ = context.currentLevel;
	offset_ = varTable.size();
	varTable.push_back(this);
	llvm::Type *exprType = _expr->traverse(varTable, context);
	if (_type == nullptr) {
		type_ = exprType;
	}
	else {
		// _type is setted, so the expr type must match _type
		type_ = context.typeOf(_type->getTypeId());
		if (!context.typeMatch(type_, exprType)) {
			return context.errorTypeMsg("The type on the two sides is not match!");
		}
	}
	if (!type_)
		return nullptr;
	context.varDecls.push(_id, this);
	return context.voidType;
}

// type type-id = type
// Example: type intArray = array of int
// _type_id = "intArray", _type = array of int
llvm::Type *TypeDeclAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	_type->setName(_type_id);
	context.typeDecls[_type_id] = _type;
	return context.voidType;
}

// name (type-fields)
// name (type-fields) : type-id
// Example: f(a: int, b:int) = printi(a + b)
// _name = "f", _params = (a: int, b:int), _returnType = void
llvm::FunctionType *FuncTy::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	//to store all the paramaters type
	std::vector<llvm::Type *> typeArr;
	//staticLink, record type storing varaibles in parent function
	llvm::PointerType *staticLinkType = llvm::PointerType::getUnqual(context.staticLink.front());
	typeArr.push_back(staticLinkType);
	frame = llvm::StructType::create(context.context, _name + "Frame");
	//create staticLink declaration
	staticLink_ = new VarDeclAST("staticLink", staticLinkType, varTable.size(), context.currentLevel);
	varTable.push_back(staticLink_);
	//traverse in type fields 
	TypeFieldsAST *typeFields = _params;
	while (typeFields) {
		llvm::Type *type = typeFields->traverse(varTable, context);
		typeArr.push_back(type);
		if (!typeArr.back())
			return nullptr;
		typeFields = typeFields->getNext();
	}
	//handle return type
	if (_returnType == nullptr)
		resultType_ = llvm::Type::getVoidTy(context.context);
	else
		resultType_ = context.typeOf(_returnType->getTypeId());
	if (!resultType_)
		return nullptr;
	llvm::FunctionType *functionType = llvm::FunctionType::get(resultType_, typeArr, false);
	//create llvm::Function
	function_ = llvm::Function::Create(functionType, llvm::Function::InternalLinkage, _name, context.module.get());
	return functionType;
}

// function name (type-fields) = expr
// function name (type-fields) : type-id = expr
// Example: f(a: int, b:int) = printi(a + b)
// _name = "f", _type_fields = (a: int, b:int), expr = printi(a + b)
llvm::Type *FuncDeclAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	//find function declaration in current scope
	if (context.functions.find(_name)) {
		return context.errorTypeMsg(_name + "() is already defined in this scope!");
	}
	//create a new scope
	context.varDecls.enter();
	//increase level
	context.currentLevel += 1;
	_level = context.currentLevel;
	//check llvm::FunctionType, create llvm::Function
	llvm::FunctionType *funcTy = _funcTy->traverse(varTable_, context);
	if (!funcTy)
		return nullptr;
	//create staticLink for expr
	context.staticLink.push_front(_funcTy->getFrame());
	//store llvm::Function
	context.functions.push(_name, _funcTy->getFunction());
	llvm::Type *expr = _expr->traverse(varTable_, context);
	//destory staticLink
	context.staticLink.pop_front();
	if (!expr)
		return nullptr;
	//exit current scope
	context.varDecls.exit();
	context.currentLevel -= 1;
	llvm::Type *returnType = _funcTy->getResultType();
	//check return type
	if (!returnType->isVoidTy() && returnType != expr)
		return context.errorTypeMsg("Result type doesn't match the defined return type!");
	else
		return context.voidType;
}

//===----------------------------------------------------------------------===//
// List 
//===----------------------------------------------------------------------===//

llvm::Type *DeclListAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	//just traverse all declarations in list
	DeclListAST* declList = this;
	DeclAST* curDecl;
	llvm::Type *type;
	while (declList) {
		curDecl = declList->getDecl();
		type = curDecl->traverse(varTable, context);
		declList = declList->getNext();
	}
	//return the type of last declaration
	return type;
}

llvm::Type *ExprListAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	//just traverse all expressions in list
	ExprListAST* exprList = this;
	ExprAST* curExpr;
	llvm::Type *type;
	while (exprList) {
		curExpr = exprList->getExpr();
		type = curExpr->traverse(varTable, context);
		exprList = exprList->getNext();
	}
	//return the type of last expression
	return type;
}

// Example: { id: type-id, id: type-id }
// {a: int, b: string}
// _id = "a", _type_id = "int"
llvm::Type *TypeFieldsAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	//get the llvm::type
	type_ = context.typeOf(_type_id);
	//create varaible declaration
	varDecl_ = new VarDeclAST(_id, type_, varTable.size(), context.currentLevel);
	context.varDecls.push(_id, varDecl_);
	varTable.push_back(varDecl_);
	return type_;
}

// Example: { id = expr, id = expr }
// {a = 1 + b, c = chr(a)}
// _id = a, _expr = 1 + b
llvm::Type *FieldListAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	// just traverse the _expr and store the llvm::type
	type_ = _expr->traverse(varTable, context);
	return type_;
}

//===----------------------------------------------------------------------===//
// Expression
//===----------------------------------------------------------------------===//

// integer-constant
// Example: 1234
// _value = 1234
llvm::Type *IntConstExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	return context.intType;
}

// string-constant
// Example: "hello"
// _value = "hello"
llvm::Type *StrConstExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	return context.stringType;
}

// nil
llvm::Type *NilExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	return context.nilType;
}

// lvalue
llvm::Type *VarExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	return _value->traverse(varTable, context);
}

// - expr, '-' is the only operator
llvm::Type *UnaryExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	llvm::Type *exprType = _expr->traverse(varTable, context);
	if (!exprType)
		return nullptr;
	//the expr must be integer type
	if (exprType->isIntegerTy())
		return context.intType;
	else
		return context.errorTypeMsg("The expr in Unary Expr should be integer type!");
}

// expr binary-operator expr
llvm::Type *BinaryExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	llvm::Type *leftType = _expr_left->traverse(varTable, context);
	llvm::Type *rightType = _expr_right->traverse(varTable, context);
	if (!leftType || !rightType)
		return nullptr;
	//for number operators("+", "-", "*", "/", "&", "|" ), the left and right should be both integer type
	if (numOp.find(_op) != numOp.end()) {
		if (leftType->isIntegerTy() && rightType->isIntegerTy())
			return context.intType;
		else
			return context.errorTypeMsg("The two expr in binary operation should be integer type!");
	}
	//for compare operators("=", "<>", ">=", "<=", ">", "<"), the left and right should be the same type
	else if (cmpOp.find(_op) != cmpOp.end()) {
		if (context.typeMatch(leftType, rightType)) {
			if (leftType == context.nilType && rightType == context.nilType)
				return context.errorTypeMsg("Nil cannot compare with nil!");
			else
				return context.intType;
		}
		else {
			return context.errorTypeMsg("The two expr in binary operation should be the same type!");
		}
	}
	else {
		return nullptr;
	}
}

// lvalue := expr
// Example: a := 3
// _var = a, _expr = 3
llvm::Type *AssignExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	llvm::Type *varType = _var->traverse(varTable, context);
	if (!varType)
		return nullptr;
	llvm::Type *exprType = _expr->traverse(varTable, context);
	if (!exprType)
		return nullptr;
	//the _var type and _expr type must match
	if (context.typeMatch(varType, exprType))
		return exprType;
	else
		return context.errorTypeMsg("The types on the two sides of assign expr don't match.");
}

// func_name(expr-list)
// Example: f() f(1, 2)
// _func_name = "f", _expr_list = (1, 2)
llvm::Type *FuncExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	//find the declared llvm::Function
	llvm::Function *function = context.functions[_func_name];
	if (!function)
		return context.errorTypeMsg("Function " + _func_name + " isn't declared!");
	size_t paramNum = 0u;
	size_t index = 0u;
	ExprListAST *exprList = _expr_list;
	//get parameter number
	while (exprList) {
		++paramNum;
		exprList = exprList->getNext();
	}
	//staticLink exists
	if (function->getLinkage() != llvm::Function::ExternalLinkage) {
		++paramNum;
		index = 1u;
	}
	llvm::FunctionType *functionType = function->getFunctionType();
	if (paramNum != functionType->getNumParams())
		return context.errorTypeMsg("The number of parameters is wrong!");
	exprList = _expr_list;
	ExprAST* curExpr;
	//traverse the expr list
	while (exprList) {
		curExpr = exprList->getExpr();
		llvm::Type *type = curExpr->traverse(varTable, context);
		llvm::Type *paramType = functionType->getParamType(index);
		++index;
		//args in function call expression must match the function declaration
		if (type != paramType) {
			return context.errorTypeMsg("The type of parameters is wrong!");
		}
		exprList = exprList->getNext();
	}
	return function->getReturnType();
}

// ( expr-list ) , list can be null
// Example: (a := 1 ; b := 2)  ()
llvm::Type *SequenceExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	//just traverse the expression list
	llvm::Type *type;
	ExprAST *curExpr;
	ExprListAST *exprList = _expr_list;
	while (exprList) {
		curExpr = exprList->getExpr();
		type = curExpr->traverse(varTable, context);
		exprList = exprList->getNext();
	}
	//return the type of last expression
	return type;
}

// type-id { field-list } , list can be null
// Example:  human { age = 18, height = 170 }
// _type_id = "human" field-list = { age = 18, height = 170 }
llvm::Type *RecordDefineExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	//first check if it's true record type
	type_ = context.typeOf(_type_id);
	if (!type_->isPointerTy())
		return context.errorTypeMsg("It should be record type!");
	llvm::Type *elementType = context.getType(type_);
	if (!elementType->isStructTy())
		return context.errorTypeMsg("It should be record type!");
	//get the record type declaration
	RecordTypeAST *typeDecl = dynamic_cast<RecordTypeAST *>(context.typeDecls[_type_id]);
	if (!typeDecl)
		return nullptr;
	//the number of fields in type declaration
	int typeFieldsNum = 0;
	TypeFieldsAST *typeField = typeDecl->getTypeFields();
	while (typeField) {
		++typeFieldsNum;
		typeField = typeField->getNext();
	}
	//the number field expression in this definition
	int fieldExpNum = 0;
	FieldListAST* fieldExp = _field_list;
	while (fieldExp) {
		++fieldExpNum;
		fieldExp = fieldExp->getNext();
	}
	//must be same
	if (typeFieldsNum != fieldExpNum) {
		return context.errorTypeMsg("The field expr number is wrong!");
	}
	typeField = typeDecl->getTypeFields();
	fieldExp = _field_list;
	while (typeField && fieldExp) {
		//the _id of fields must be the same and at the same position
		if (typeField->getId() != fieldExp->getId()) {
			return context.errorTypeMsg(fieldExp->getId() + " isn't a field id or isn't at right position!");
		}
		llvm::Type *type = fieldExp->traverse(varTable, context);
		//the type of fields must match
		if (!context.typeMatch(type, typeField->getType())) {
			return context.errorTypeMsg("The type of field expr is wrong!");
		}
		typeField = typeField->getNext();
		fieldExp = fieldExp->getNext();
	}
	return type_;
}

// type-id [size] of value
// Example: int[3] of 0
// _type_id = "int", size = 3, value = 0
llvm::Type *ArrayDefineExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	//first check if it's true array type
	type_ = context.typeOf(_type_id);
	if (!type_)
		return nullptr;
	if (!type_->isPointerTy())
		return context.errorTypeMsg("It should be array type!");
	//get the element type of the array
	llvm::Type *elementType = context.getType(type_);
	//get the value type of the array
	llvm::Type *valueType = _value->traverse(varTable, context);
	if (!valueType)
		return nullptr;
	//the size must be integer type
	llvm::Type *sizeType = _size->traverse(varTable, context);
	if (!sizeType)
		return nullptr;
	if (!sizeType->isIntegerTy())
		return context.errorTypeMsg("The size of array should be an integer!");
	//the element type and value type must match
	if (elementType != valueType)
		return context.errorTypeMsg("The value type of array is wrong!");
	return type_;
}

// if expr then expr
// if expr then expr else expr
llvm::Type *IfExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	//the test condition must be integer type
	llvm::Type *ifType = _if_expr->traverse(varTable, context);
	if (!ifType)
		return nullptr;
	if (!ifType->isIntegerTy())
		return context.errorTypeMsg("The condition of if expr should be integer!");
	llvm::Type *thenType = _then_expr->traverse(varTable, context);
	if (!thenType)
		return nullptr;
	if (_else_expr) {
		llvm::Type *elseType = _else_expr->traverse(varTable, context);
		if (!elseType)
			return nullptr;
		//if else expr exists, the type of then type and else type must match
		if (!context.typeMatch(thenType, elseType))
			return context.errorTypeMsg("The types of then expr and else expr should match!");
		return thenType;
	}
	else {
		return context.voidType;
	}
}

// while expr do expr
llvm::Type *WhileExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	//the test condition must be integer type
	llvm::Type *whileType = _while_expr->traverse(varTable, context);
	if (!whileType)
		return nullptr;
	if (!whileType->isIntegerTy())
		return context.errorTypeMsg("The condition of while expr should be integer!");
	_do_expr->traverse(varTable, context);
	return context.voidType;
}

// for id := expr to expr do expr
llvm::Type *ForExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	llvm::Type *initType = _init_expr->traverse(varTable, context);
	if (!initType)
		return nullptr;
	llvm::Type *toType = _to_expr->traverse(varTable, context);
	if (!toType)
		return nullptr;
	//the init expr and to expr should be both integer type
	if (!initType->isIntegerTy() || !toType->isIntegerTy())
		return context.errorTypeMsg("Both init expr and to expr should be integer!");
	//create id's varaible declaration
	_varDecl = new VarDeclAST(_id, context.intType, varTable.size(), context.currentLevel);
	varTable.push_back(_varDecl);
	context.varDecls.push(_id, _varDecl);
	llvm::Type *doType = _do_expr->traverse(varTable, context);
	if (!doType)
		return nullptr;
	else
		return context.voidType;
}

// break
llvm::Type *BreakExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	return context.voidType;
}

// let declaration-list in expr-list end
// Example: 
//    let 
//        var a := 1
//        var b := 2
//    in
//        a + b
//    end
llvm::Type *LetExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	//create new scopes
	context.types.enter();
	context.typeDecls.enter();
	context.varDecls.enter();
	context.functions.enter();

	//just traverse the declaration-list
	DeclListAST* declList = _decl_list;
	DeclAST* curDecl;
	while (declList) {
		curDecl = declList->getDecl();
		curDecl->traverse(varTable, context);
		declList = declList->getNext();
	}
	//just traverse the expression-list
	ExprListAST* exprList = _expr_list;
	ExprAST* curExpr;
	llvm::Type *type;
	while (exprList) {
		curExpr = exprList->getExpr();
		type = curExpr->traverse(varTable, context);
		exprList = exprList->getNext();
	}

	//exit current scopes
	context.functions.exit();
	context.varDecls.exit();
	context.typeDecls.exit();
	context.types.exit();
	//return the type of last expression
	return type;
}
