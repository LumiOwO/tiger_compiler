%{
#include "parser.h"
#include "parser.tab.hpp"
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}
digit [0-9]
num {digit}+
float {digit}+[.]{digit}*|[.]{digit}+

%%
{float}		{ 
	yylval.type = T_FLOAT;
	yylval.f = atof(yytext);
	yylval.i = (int)yylval.f;
	return FLOAT; 
}
{num}		{ 
	yylval.type = T_INT;
	yylval.i = atoi(yytext);
	yylval.f = (double)yylval.i;
	return INT; 
}
[ \t] 		{ }
[\n] 		{ return 0; }
. 			{ return yytext[0]; }
%%

int yywrap()
{
	return 1;
}