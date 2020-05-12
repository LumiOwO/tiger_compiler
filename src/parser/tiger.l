%{
#include "parser.h"
#include "tiger.tab.hpp"
 
#include <iostream> 
%}
/* Comments */
LINE_COMMENT 		"//".*
BLOCK_COMMENT 		"/*"(.|\n)*"*/"

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

/* Operators */
ASSIGN 				:=
COMPARE 			>=|<=|<>

/* Punctuation */
PUNCTUATION 		[,:;\(\)\[\]\{\}\.+\-\*/=<>&\|]

/* Constant */
STRING 				\".*\"
INTEGER 			[0-9]+

/* ID */
ID 					[a-zA-Z_][0-9a-zA-Z_]*

%%
{NIL} {
	debug_print("NIL: " << yytext);
	return K_NIL;
}
{IF} {
	debug_print("IF: " << yytext);
	return K_IF;
}
{ELSE} {
	debug_print("ELSE: " << yytext);
	return K_ELSE;
}
{WHILE} {
	debug_print("WHILE: " << yytext);
	return K_WHILE;
}
{DO} {
	debug_print("DO: " << yytext);
	return K_DO;
}
{FOR} {
	debug_print("FOR: " << yytext);
	return K_FOR;
}
{TO} {
	debug_print("TO: " << yytext);
	return K_TO;
}
{BREAK} {
	debug_print("BREAK: " << yytext);
	return K_BREAK;
}
{IN} {
	debug_print("IN: " << yytext);
	return K_IN;
}
{END} {
	debug_print("END: " << yytext);
	return K_END;
}
{OF} {
	debug_print("OF: " << yytext);
	return K_OF;
}
{TYPE} {
	debug_print("TYPE: " << yytext);
	return K_TYPE;
}
{ARRAY} {
	debug_print("ARRAY: " << yytext);
	return K_ARRAY;
}
{VAR} {
	debug_print("VAR: " << yytext);
	return K_VAR;
}
{FUNC} {
	debug_print("FUNC: " << yytext);
	return K_FUNC;
}
{LET} {
	debug_print("LET: " << yytext);
	return K_LET;
}

{ASSIGN} {
	debug_print("ASSIGN: " << yytext);
	return O_ASSIGN;
}
{COMPARE} {
	debug_print("COMPARE: " << yytext);
	return O_COMPARE;
}

{STRING} {
	debug_print("STRING: " << yytext);
	return C_STRING;
}
{INTEGER} {
	debug_print("INTEGER: " << yytext);
	return C_INTEGER;
}

{ID} {
	debug_print("ID: " << yytext);
	yylval = yytext;
	return typeset.contains(yytext)? T_TYPE: T_ID;
}

{PUNCTUATION} {
	debug_print("PUNCTUATION: " << yytext);
	return yytext[0];
}

{LINE_COMMENT} { }
{BLOCK_COMMENT} { }
%%

int yywrap()
{
	return 1;
}