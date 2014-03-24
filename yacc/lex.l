%option yylineno

D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*

%{
	#include <stdio.h>
	#include "y.tab.h"
%}

%%
[ \t\n\b]					;
"auto" 		return AUTO ;
"break" 		return BREAK ;
"case" 		return CASE ;
"char" 		return CHAR ;
"const" 		return CONST ;
"continue" 		return CONTINUE ;
"default" 		return DEFAULT ;
"do" 		return DO ;
"double" 		return DOUBLE ;
"else" 		return ELSE ;
"enum" 		return ENUM ;
"extern" 		return EXTERN ;
"float" 		return FLOAT ;
"for" 		return FOR ;
"goto" 		return GOTO ;
"if" 		return IF ;
"int" 		return INT ;
"long" 		return LONG ;
"register" 		return REGISTER ;
"return" 		return RETURN ;
"short" 		return SHORT ;
"signed" 		return SIGNED ;
"sizeof" 		return SIZEOF ;
"static" 		return STATIC ;
"struct" 		return STRUCT ;
"switch" 		return SWITCH ;
"typedef" 		return TYPEDEF ;
"union" 		return UNION ;
"unsigned" 		return UNSIGNED ;
"void" 		return VOID ;
"volatile" 		return VOLATILE ;
"while" 		return WHILE ;
"main" 		return MAIN ;
"+" 		return PLUS ;
"-" 		return MINUS ;
"*" 		return TIMES ;
"/" 		return DIV ;
"%" 		return MODULO ;
"=" 		return EQUALS ;
"&" 		return BITAND ;
"|" 		return BITOR ;
"^" 		return BITXOR ;
"!" 		return NOT ;
"<" 		return LESS ;
">" 		return GREATER ;
"#" 		return HASH ;
"$" 		return DOLLAR ;
"@" 		return ATRATE ;
"[" 		return LS ;
"]" 		return RS ;
"{" 		return LB ;
"}" 		return RB ;
"(" 		{return LP ;}
")" 		return RP ;
"?" 		return QMARK ;
":" 		return COLON ;
"." 		return DOT ;
";" 		return SEMI ;
"," 		return COMMA ;
"+=" 		return PLUSEQ ;
"-=" 		return MINUSEQ ;
"*=" 		return TIMESEQ ;
"/=" 		return DIVEQ ;
"%=" 		return MODULOEQ ;
"&=" 		return EMPEQ ;
"&&" 		return AND ;
"||" 		return OR ;
"==" 		return REQUALS ;
"!=" 		return NOTEQUAL ;
"<=" 		return LESSEQ ;
">=" 		return GREATEREQ ;
"++" 		return INCR ;
"--" 		return DECR ;
"<<" 		return LSHIFT ;
">>" 		return RSHIFT ;
"|=" 		return OREQ ;
"^=" 		return XOREQ ;
"->" 		return PTRREF ;
"<<=" 		return LSHIFTEQ ;
">>=" 		return RSHIFTEQ ;

{L}({L}|{D})*			return IDENTIFIER ;
0[xX]{H}+{IS}?			return NUMBER ;
0{D}+{IS}?				return NUMBER ;
{D}+{IS}?				return NUMBER ;

L?'(\\.|[^\\'])+'		return REAL ; 
{D}+{E}{FS}?			return REAL ;
{D}*"."{D}+({E})?{FS}?	return REAL ;
{D}+"."{D}*({E})?{FS}?	return REAL ;
.						{ /* ignore bad characters */ }
%%

int yywrap(void)
{
	return 1 ;
}