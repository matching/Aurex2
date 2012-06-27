%{
#define YYSTYPE char *

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"


void yyerror(char* s) {
   printf("%s\n", s);
}

int yylex(void);


static char buf[256]="";
static int code;
static int width;

extern char lex_line[];
%}

%token NUMBER
%token ENCODING
%token SWIDTH
%token DWIDTH
%token BBX
%token BITMAP
%token ENDCHAR
%token STARTCHAR
%token NL
%token DELIM
%token ANYCHAR
%%
bdf :	/* ¶õ */
	| bdf bdffont 	{}
	| bdf bdfheader {}
	;

bdfheader :
	| ANYCHAR {}
	| DELIM {}
	| NL {}
	| NUMBER {free($1);}
	| ENCODING {}
	;

bdffont :  start encoding misc font ENDCHAR
	{
#ifdef TEST
		printf("charcode %d, %d, char (%s)\n", code, width, buf);
#else
		kakuno(code, width, buf);
#endif
		*buf=0;
	}
	;

font : | font NUMBER NL
	{
		strcat(buf, $2);
		strcat(buf, "\x0a");

		free($2);
	}
	;

start : STARTCHAR NL
	{
	}
	;

encoding : ENCODING DELIM NUMBER NL
	{
		code=strtol($3, NULL, 10);
		free($3);
	}
	;

misc : SWIDTH NL DWIDTH NL bbx NL BITMAP NL
	{
	}
	;

bbx : BBX DELIM NUMBER DELIM NUMBER DELIM NUMBER DELIM NUMBER
	{
		width=strtol((char *)$3, NULL, 10);

		free($3);
		free($5);
		free($7);
		free($9);
	}
	;
%%
#ifdef TEST
int main(void)
{
	yyparse();
}

#endif
