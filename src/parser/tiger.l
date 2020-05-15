%{
#include "parser.h"
#include "tiger.tab.hpp"
 
#include <iostream> 

using namespace TigerParser;

%}

%x STRING

/* Comments */
LINE_COMMENT 		"//".*
BLOCK_COMMENT 		"/*"([^\*]|(\*)*[^\*/])*(\*)*"*/"
BLANK				(" "|\n|\t|\r)+

/* Keywords */
NIL					nil
IF					if
ELSE				else
WHILE				while
DO					do
FOR					for
TO					to
BREAK				break
IN					in
END					end
OF					of
TYPE				type
ARRAY				array
VAR					var
FUNC				function
LET					let
THEN				then

/* Operators */
ASSIGN 				:=
COMPARE 			>=|<=|<>

/* Punctuation */
PUNCTUATION 		[,:;\(\)\[\]\{\}\.]
OPERATORS 			[+\-\*/=<>&\|]

/* Constant */
STRING 				\"(\\.|[^"\\])*\"
INTEGER 			[0-9]+

/* ID */
ID 					[a-zA-Z_][0-9a-zA-Z_]*

%%
{NIL} {
	debug_print("NIL: " << yytext);
	yylval = nullptr;
	return K_NIL;
}
{IF} {
	debug_print("IF: " << yytext);
	yylval = nullptr;
	return K_IF;
}
{ELSE} {
	debug_print("ELSE: " << yytext);
	yylval = nullptr;
	return K_ELSE;
}
{WHILE} {
	debug_print("WHILE: " << yytext);
	yylval = nullptr;
	return K_WHILE;
}
{DO} {
	debug_print("DO: " << yytext);
	yylval = nullptr;
	return K_DO;
}
{FOR} {
	debug_print("FOR: " << yytext);
	yylval = nullptr;
	return K_FOR;
}
{TO} {
	debug_print("TO: " << yytext);
	yylval = nullptr;
	return K_TO;
}
{BREAK} {
	debug_print("BREAK: " << yytext);
	yylval = nullptr;
	return K_BREAK;
}
{IN} {
	debug_print("IN: " << yytext);
	yylval = nullptr;
	return K_IN;
}
{END} {
	debug_print("END: " << yytext);
	yylval = nullptr;
	return K_END;
}
{OF} {
	debug_print("OF: " << yytext);
	yylval = nullptr;
	return K_OF;
}
{TYPE} {
	debug_print("TYPE: " << yytext);
	yylval = nullptr;
	return K_TYPE;
}
{ARRAY} {
	debug_print("ARRAY: " << yytext);
	yylval = nullptr;
	return K_ARRAY;
}
{VAR} {
	debug_print("VAR: " << yytext);
	yylval = nullptr;
	return K_VAR;
}
{FUNC} {
	debug_print("FUNC: " << yytext);
	yylval = nullptr;
	return K_FUNC;
}
{LET} {
	debug_print("LET: " << yytext);
	yylval = nullptr;
	return K_LET;
}
{THEN} {
	debug_print("THEN: " << yytext);
	yylval = nullptr;
	return K_THEN;
}

{ASSIGN} {
	debug_print("ASSIGN: " << yytext);
	yylval = nullptr;
	return O_ASSIGN;
}
{COMPARE} {
	debug_print("COMPARE: " << yytext);
	yylval = new LexAST(yytext);
	return O_COMPARE;
}

\"                  { BEGIN STRING; resetStrBuffer(); appendLabel("\\\""); }
<STRING>[^\\"\n]*   { appendReal(yytext); appendLabel(yytext); }
<STRING>\\n         { appendReal('\n'); appendLabel("\\\\n"); }
<STRING>\\t         { appendReal('\t'); appendLabel("\\\\t"); }
<STRING>\\r         { appendReal('\r'); appendLabel("\\\\r"); }
<STRING>\\b         { appendReal('\b'); appendLabel("\\\\b"); }
<STRING>\\[0-7]*    { char c = (char)strtol(yytext + 1, 0, 8); 
					  appendReal(c); appendLabel(std::string("\\\\") + (yytext + 1)); }
<STRING>\\[\\"']    { appendReal(yytext[1]); appendLabel(std::string("\\\\\\") + yytext[1]); }
<STRING>\"          { 
	appendLabel("\\\"");
	debug_print("Real string:  " << realstr() << "   Label string:  " << labelstr());
	yylval = new StrConstExprAST(realstr(), labelstr());
	BEGIN 0; 
	return C_STRING; 
}
<STRING>\\.         { std::cerr << "bogus escape \"" << yytext << "\" in string" << std::endl; }
<STRING>\n          { std::cerr << "newline in string" << std::endl; }

{INTEGER} {
	debug_print("INTEGER: " << yytext);
	yylval = new IntConstExprAST(yytext);
	return C_INTEGER;
}

{ID} {
	debug_print("ID: " << yytext);
	yylval = new LexAST(yytext);
	return hasType(yytext)? T_TYPE: T_ID;
}

{PUNCTUATION} {
	debug_print("PUNCTUATION: " << yytext);
	yylval = nullptr;
	return yytext[0];
}

{OPERATORS} {
	debug_print("OPERATOR: " << yytext);
	yylval = new LexAST(yytext);
	return yytext[0];
}

{LINE_COMMENT} { }
{BLOCK_COMMENT} { }
{BLANK} { debug_print(""); }
%%

int yywrap()
{
	return 1;
}