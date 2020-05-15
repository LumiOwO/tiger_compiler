# tiger_compiler

Compiler for tiger language using LLVM
* IDE环境
  * Visual Studio 2017, 项目配置为Win32 Release版本

## 语法树

语法树的类图可以参考找到的那篇实验报告的11 - 13页，AST基本上就是参照那个做的

**注意：某些成员变量可能为空指针，已经在注释里标注了。例如next可能为空，则相应有一个hasNext()函数。如果没有对应的has函数则指针一定不为空**

* 所有语法树结点的基类为AST，里面只提供语法树可视化的接口
* 列表，均为链表形式
    * 声明列表DeclListAST
        * DeclAST*    _decl，当前声明
        * DeclListAST*  _next，下一结点，可以为空
    * 表达式列表ExprListAST
        * ExprAST*    _expr，当前表达式
        * ExprListAST*  _next，下一结点，可以为空
    * 类型字段定义列表TypeFieldsAST，用于定义新的类型，例如type human = { age: int, height: int}
        * string   _id，字段名
        * string   _type_id，字段类型
        * TypeFieldsAST*  _next，下一结点，可以为空
    * 字段初始化列表FieldListAST，例如var LiHua = human { age = 18, height = 170 }
        * string   _id，字段
        * ExprAST*    _expr，初始化的表达式值
        * FieldListAST*  _next，下一结点，可以为空
* 变量，基类VarAST
    * 简单变量SimpleVarAST，例如 a，student
        * string _name，表示变量的名字
    * 字段访问变量FieldVarAST，例如 student.age
        * VarAST* _var，表示一个变量
        * string   _field，字段名
    * 索引访问变量IndexVarAST，例如a[0]，a[i + 1]
        * VarAST* _var，表示一个变量
        * ExprAST*  _index，表示索引值
* 变量的类型，基类TypeAST
    * 简单类型SimpleTypeAST，例如int
        * string   _type_id，表示类型名称
    * 记录类型RecordTypeAST，例如 {a: int, b: string}
        * TypeFieldsAST* _type_fields，字段声明的列表
    * 数组类型ArrayTypeAST，例如 array of int
        * string   _type_id，表示数组元素的类型名称
* 声明，基类DeclAST
    * 变量声明VarDeclAST，例如 var a := 1，var b: int := 3
        * string   _id，表示变量名
        * SimpleTypeAST* _type，表示变量的类型名，可以为空
        * ExprAST* _expr，表示右值
    * 类型声明TypeDeclAST，例如type intArray = array of int
        * std::string   _type_id， 表示类型名
        * TypeAST*    _type，表示等式右边的类型
    * 函数声明FuncDeclAST，例如function f(a) = print(a)
        * string   _name，函数名
        * TypeFieldsAST* _type_fields,函数的参数列表，可以为空（函数可以没有参数）
        * SimpleTypeAST* _type，函数的返回类型名，可以为空
        * ExprAST*    _expr，函数体
* 表达式，基类ExprAST
    * 数字常量IntConstExprAST，例如1234
        * int _value，整数值
    * 字符串常量StrConstExprAST，例如"hello"
        * string _value，字符串，已经把引号去掉了
    * 空表达式NilExprAST，例如 nil
        * 无成员变量
    * 变量表达式VarExprAST，例如a[0]，student.age
        * VarAST* _value，表示变量
    * 单目运算表达式UnaryExprAST，例如 -3
        * string   _op，运算符，目前只会出现负号
        * ExprAST*    _expr，参与运算的表达式
    * 双目运算表达式BinaryExprAST，例如 1+2
        * ExprAST*    _expr_left，左侧的表达式
        * string   _op，运算符，包括四则运算(+, -, \*, /), 大小比较, 逻辑运算(&, |)
        * ExprAST*    _expr_right，右侧的表达式
    * 赋值表达式AssignExprAST，例如 a := 3
        * VarAST*   _var，左值
        * ExprAST*  _expr，右值
    * 函数表达式FuncExprAST，例如 f()  , f(1, 2)
        * string   _func_name，函数名
        * ExprListAST*  _expr_list，参数列表
    * 多个表达式SequenceExprAST，例如 (a := 1 ; b := 2)
        * ExprListAST*  _expr_list，表达式列表
    * 定义记录RecordDefineExprAST，用于记录类型变量初始化，例如var LiHua = human { age = 18, height = 170 }
        * string   _type_id，变量类型名
        * FieldListAST*  _field_list，初始化列表
    * 定义数组ArrayDefineExprAST，用于数组类型变量初始化，例如 var a = int[3] of 0
        * string   _type_id，数组类型名
        * ExprAST*    _size，数组大小
        * ExprAST*    _value，每个元素的值
    * if语句 IfExprAST，例如 if expr then expr else expr，可以没有else
        * ExprAST*    _if_expr，非空
        * ExprAST*    _then_expr，非空
        * ExprAST*    _else_expr，可以为空
    * while语句 WhileExprAST，例如 while expr do expr
        * ExprAST*    _while_expr，非空
        * ExprAST*    _do_expr，非空
    * for语句 ForExprAST，例如 for i := 0 to 10 do expr
        * string   _id，临时变量的名称
        * ExprAST*    _init_expr，临时变量的初始值
        * ExprAST*    _to_expr，变量的最大值
        * ExprAST*    _do_expr，每次循环的操作
    * break语句 BreakExprAST，例如 break
        * 无成员变量
    * let语句 LetExprAST，例如 let var a:=1 in print(a) end
        * DeclListAST*  _decl_list，声明列表，声明变量、函数、类型等
        * ExprListAST*  _expr_list，声明后进行的操作，可以为空