#include "AST/AST.h"

llvm::Type *MainAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	context.typeDecls.resetTable();
	return _mainExpr->traverse(mainVarTable, context);
}

//===----------------------------------------------------------------------===//
// Variable
//===----------------------------------------------------------------------===//

llvm::Type *SimpleVarAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	VarDeclAST *varDecl = context.varDecls[_name];
	if (varDecl)
		return varDecl->getType();
	else
		return context.errorTypeMsg(_name + "has not been defined!");
}

llvm::Type *FieldVarAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	llvm::Type *var = _var->traverse(varTable, context);
	if (!var)
		return nullptr;
	if (!context.isRecord(var))
		return context.errorTypeMsg("Field var can only be used by record type.");
	llvm::StructType *varType = llvm::cast<llvm::StructType>(context.getType(var));
	RecordTypeAST *typeDecl = dynamic_cast<RecordTypeAST *>(context.typeDecls[varType->getStructName().data()]);
	TypeFieldsAST *typeFields = typeDecl->getTypeFields();
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

llvm::Type *IndexVarAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	llvm::Type *var = _var->traverse(varTable, context);
	if (!var)
		return nullptr;
	if (!var->isPointerTy() || context.getType(var)->isStructTy())
		return context.errorTypeMsg("Index var can only be used by array type.");
	type_ = context.getType(var);

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

llvm::Type *SimpleTypeAST::traverse(std::set<std::string> &parentName, CodeGen &context) {
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
	parentName.insert(_name);
	type = context.typeOf(_type_id, parentName);
	parentName.erase(_name);
	return type;
}

llvm::Type *RecordTypeAST::traverse(std::set<std::string> &parentName, CodeGen &context) {
	llvm::Type *type = context.types[_name];
	if (type)
		return type;
	if (parentName.find(_name) != parentName.end()) {
		auto type = llvm::PointerType::getUnqual(llvm::StructType::create(context.context, _name));
		context.types.push(_name, type);
		return type;
	}
	std::vector<llvm::Type *> typeArr;
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
		if (!type->isPointerTy())
			return nullptr;
		auto elementType = context.getType(type);
		if (!elementType->isStructTy())
			return nullptr;
		llvm::cast<llvm::StructType>(elementType)->setBody(typeArr);
		return type;
	}
	else {
		type = llvm::PointerType::getUnqual(llvm::StructType::create(context.context, typeArr, _name));
		if (!type)
			return nullptr;
		context.types.push(_name, type);
		return type;
	}
}

llvm::Type *ArrayTypeAST::traverse(std::set<std::string> &parentName, CodeGen &context) {
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
	type = llvm::PointerType::getUnqual(type);
	context.types.push(_name, type);
	return type;
}

//===----------------------------------------------------------------------===//
// Declaration
//===----------------------------------------------------------------------===//

llvm::Type *VarDeclAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	if (context.varDecls.find(_id)) {
		return context.errorTypeMsg(_id + "is already defined!");
	}
	offset_ = varTable.size();
	level_ = context.currentLevel;
	varTable.push_back(this);
	llvm::Type *exprType = _expr->traverse(varTable, context);
	if (_type == nullptr) {
		type_ = exprType;
	}
	else {
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

llvm::Type *TypeDeclAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	_type->setName(_type_id);
	context.typeDecls[_type_id] = _type;
	return context.voidType;
}

llvm::FunctionType *FuncTy::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	std::vector<llvm::Type *> typeArr;
	auto staticLinkType = llvm::PointerType::getUnqual(context.staticLink.front());
	typeArr.push_back(staticLinkType);
	frame = llvm::StructType::create(context.context, _name + "Frame");
	staticLink_ = new VarDeclAST("staticLink", staticLinkType, varTable.size(), context.currentLevel);
	varTable.push_back(staticLink_);
	TypeFieldsAST *typeFields = _params;
	while (typeFields) {
		llvm::Type *type = typeFields->traverse(varTable, context);
		typeArr.push_back(type);
		if (!typeArr.back())
			return nullptr;
		typeFields = typeFields->getNext();
	}
	if (_returnType == nullptr)
		resultType_ = llvm::Type::getVoidTy(context.context);
	else
		resultType_ = context.typeOf(_returnType->getTypeId());
	if (!resultType_)
		return nullptr;
	auto functionType = llvm::FunctionType::get(resultType_, typeArr, false);
	function_ = llvm::Function::Create(functionType, llvm::Function::InternalLinkage, _name, context.module.get());
	return functionType;
}

llvm::Type *FuncDeclAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	if (context.functions.find(_name)) {
		return context.errorTypeMsg(_name + "() is already defined in this scope!");
	}

	context.varDecls.enter();
	context.currentLevel += 1;
	_level = context.currentLevel;
	llvm::FunctionType *funcTy = _funcTy->traverse(varTable_, context);
	if (!funcTy)
		return nullptr;
	context.staticLink.push_front(_funcTy->getFrame());
	context.functions.push(_name, _funcTy->getFunction());
	llvm::Type *expr = _expr->traverse(varTable_, context);
	context.staticLink.pop_front();
	if (!expr)
		return nullptr;
	context.varDecls.exit();

	context.currentLevel -= 1;
	llvm::Type *returnType = _funcTy->getResultType();
	if (!returnType->isVoidTy() && returnType != expr)
		return context.errorTypeMsg("Result type doesn't match the defined return type!");
	else
		return context.voidType;
}

//===----------------------------------------------------------------------===//
// List 
//===----------------------------------------------------------------------===//

llvm::Type *DeclListAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	DeclListAST* declList = this;
	DeclAST* curDecl;
	llvm::Type *type;
	while (declList) {
		curDecl = declList->getDecl();
		type = curDecl->traverse(varTable, context);
		declList = declList->getNext();
	}
	return type;
}

llvm::Type *ExprListAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	ExprListAST* exprList = this;
	ExprAST* curExpr;
	llvm::Type *type;
	while (exprList) {
		curExpr = exprList->getExpr();
		type = curExpr->traverse(varTable, context);
		exprList = exprList->getNext();
	}
	return type;
}

llvm::Type *TypeFieldsAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	type_ = context.typeOf(_type_id);
	varDecl_ = new VarDeclAST(_id, type_, varTable.size(), context.currentLevel);
	context.varDecls.push(_id, varDecl_);
	varTable.push_back(varDecl_);
	return type_;
}

llvm::Type *FieldListAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	type_ = _expr->traverse(varTable, context);
	return type_;
}

//===----------------------------------------------------------------------===//
// Expression
//===----------------------------------------------------------------------===//

llvm::Type *IntConstExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	return context.intType;
}

llvm::Type *StrConstExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	return context.stringType;
}

llvm::Type *NilExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	return context.nilType;
}

llvm::Type *VarExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	return _value->traverse(varTable, context);
}

llvm::Type *UnaryExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	llvm::Type *exprType = _expr->traverse(varTable, context);
	if (!exprType)
		return nullptr;
	if (exprType->isIntegerTy())
		return context.intType;
	else
		return context.errorTypeMsg("The expr in Unary Expr should be integer type!");
}

llvm::Type *BinaryExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	llvm::Type *leftType = _expr_left->traverse(varTable, context);
	llvm::Type *rightType = _expr_right->traverse(varTable, context);
	if (!leftType || !rightType)
		return nullptr;
	if (numOp.find(_op) != numOp.end()) {
		if (leftType->isIntegerTy() && rightType->isIntegerTy())
			return context.intType;
		else
			return context.errorTypeMsg("The two expr in binary operation should be integer type!");
	}
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

llvm::Type *AssignExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	llvm::Type *varType = _var->traverse(varTable, context);
	if (!varType)
		return nullptr;
	llvm::Type *exprType = _expr->traverse(varTable, context);
	if (!exprType)
		return nullptr;
	if (context.typeMatch(varType, exprType))
		return exprType;
	else
		return context.errorTypeMsg("The types on the two sides of assign expr don't match.");
}

llvm::Type *FuncExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	llvm::Function *function = context.functions[_func_name];
	if (!function)
		return context.errorTypeMsg("Function " + _func_name + " isn't declared!");
	size_t paramNum = 0u;
	size_t index = 0u;
	ExprListAST *exprList = _expr_list;
	while (exprList) {
		++paramNum;
		exprList = exprList->getNext();
	}
	if (function->getLinkage() != llvm::Function::ExternalLinkage) {
		++paramNum;
		index = 1u;
	}
	llvm::FunctionType *functionType = function->getFunctionType();
	if (paramNum != functionType->getNumParams())
		return context.errorTypeMsg("The number of parameters is wrong!");
	exprList = _expr_list;
	ExprAST* curExpr;
	while (exprList) {
		curExpr = exprList->getExpr();
		llvm::Type *type = curExpr->traverse(varTable, context);
		llvm::Type *paramType = functionType->getParamType(index);
		++index;
		if (type != paramType) {
			return context.errorTypeMsg("The type of parameters is wrong!");
		}
		exprList = exprList->getNext();
	}
	return function->getReturnType();
}

llvm::Type *SequenceExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	llvm::Type *type;
	ExprAST* curExpr;
	ExprListAST *exprList = _expr_list;
	while (exprList) {
		curExpr = exprList->getExpr();
		type = curExpr->traverse(varTable, context);
		exprList = exprList->getNext();
	}
	return type;
}

llvm::Type *RecordDefineExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	type_ = context.typeOf(_type_id);
	if (!type_->isPointerTy())
		return context.errorTypeMsg("It should be record type!");
	llvm::Type *elementType = context.getType(type_);
	if (!elementType->isStructTy())
		return context.errorTypeMsg("It should be record type!");
	RecordTypeAST *typeDecl = dynamic_cast<RecordTypeAST *>(context.typeDecls[_type_id]);
	if (!typeDecl)
		return nullptr;
	int typeFieldsNum = 0;
	TypeFieldsAST *typeField = typeDecl->getTypeFields();
	while (typeField) {
		++typeFieldsNum;
		typeField = typeField->getNext();
	}
	int fieldExpNum = 0;
	FieldListAST* fieldExp = _field_list;
	while (fieldExp) {
		++fieldExpNum;
		fieldExp = fieldExp->getNext();
	}
	if (typeFieldsNum != fieldExpNum) {
		return context.errorTypeMsg("The field expr number is wrong!");
	}
	typeField = typeDecl->getTypeFields();
	fieldExp = _field_list;
	while (typeField && fieldExp) {
		if (typeField->getId() != fieldExp->getId()) {
			return context.errorTypeMsg(fieldExp->getId() + " isn't a field id or isn't at right position!");
		}
		llvm::Type *type = fieldExp->traverse(varTable, context);
		if (!context.typeMatch(type, typeField->getType())) {
			return context.errorTypeMsg("The type of field expr is wrong!");
		}
		typeField = typeField->getNext();
		fieldExp = fieldExp->getNext();
	}
	return type_;
}

llvm::Type *ArrayDefineExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	type_ = context.typeOf(_type_id);
	if (!type_)
		return nullptr;
	if (!type_->isPointerTy())
		return context.errorTypeMsg("It should be array type!");
	llvm::Type *elementType = context.getType(type_);
	llvm::Type *valueType = _value->traverse(varTable, context);
	if (!valueType)
		return nullptr;
	llvm::Type *sizeType = _size->traverse(varTable, context);
	if (!sizeType)
		return nullptr;
	if (!sizeType->isIntegerTy())
		return context.errorTypeMsg("The size of array should be an integer!");
	if (elementType != valueType)
		return context.errorTypeMsg("The value type of array is wrong!");
	return type_;
}

llvm::Type *IfExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
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
		if (!context.typeMatch(thenType, elseType))
			return context.errorTypeMsg("The types of then expr and else expr should match!");
		return thenType;
	}
	else {
		return context.voidType;
	}
}

llvm::Type *WhileExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	llvm::Type *whileType = _while_expr->traverse(varTable, context);
	if (!whileType)
		return nullptr;
	if (!whileType->isIntegerTy())
		return context.errorTypeMsg("The condition of while expr should be integer!");
	_do_expr->traverse(varTable, context);
	return context.voidType;
}

llvm::Type *ForExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	llvm::Type *initType = _init_expr->traverse(varTable, context);
	if (!initType)
		return nullptr;
	llvm::Type *toType = _to_expr->traverse(varTable, context);
	if (!toType)
		return nullptr;
	if (!initType->isIntegerTy() || !toType->isIntegerTy())
		return context.errorTypeMsg("Both init expr and to expr should be integer!");
	_varDecl = new VarDeclAST(_id, context.intType, varTable.size(), context.currentLevel);
	varTable.push_back(_varDecl);
	context.varDecls.push(_id, _varDecl);
	llvm::Type *doType = _do_expr->traverse(varTable, context);
	if (!doType)
		return nullptr;
	else
		return context.voidType;
}

llvm::Type *BreakExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	return context.voidType;
}

llvm::Type *LetExprAST::traverse(std::vector<VarDeclAST *> &varTable, CodeGen &context) {
	context.types.enter();
	context.typeDecls.enter();
	context.varDecls.enter();
	context.functions.enter();

	DeclListAST* declList = _decl_list;
	DeclAST* curDecl;
	while (declList) {
		curDecl = declList->getDecl();
		curDecl->traverse(varTable, context);
		declList = declList->getNext();
	}

	ExprListAST* exprList = _expr_list;
	ExprAST* curExpr;
	llvm::Type *type;
	while (exprList) {
		curExpr = exprList->getExpr();
		type = curExpr->traverse(varTable, context);
		exprList = exprList->getNext();
	}

	context.functions.exit();
	context.varDecls.exit();
	context.typeDecls.exit();
	context.types.exit();

	return type;
}
