%{
#include <stdio.h>
#include <string.h>
#define YYSTYPE char *
#include "bdf.tab.h"
extern "C"{
int yywrap(void)
{
return 1;
}
}
char lex_line[256];
extern char lex_line[];
%}
number [-+]?[[:xdigit:]]+
delim [[:space:]]
nl \n
%%
STARTCHAR.*	return STARTCHAR;
{number}	{yylval=strdup(yytext); return NUMBER;}
ENCODING	return ENCODING;
SWIDTH.*	return SWIDTH;
DWIDTH.* 	return DWIDTH;
BBX 		return BBX;
BITMAP 		return BITMAP;
ENDCHAR 	return ENDCHAR;
{nl} 		return NL;
{delim}+ 	return DELIM;
. 		return ANYCHAR;
%%
