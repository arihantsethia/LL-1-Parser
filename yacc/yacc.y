%{
	#include <stdio.h>
	extern int yylex(void);
	extern int yylineno ;
	void yyerror(char *);
%}

%token AUTO   BREAK   CASE   CHAR   CONST   CONTINUE   DEFAULT   DO   DOUBLE   ELSE   ENUM   EXTERN   FLOAT   FOR   GOTO   IF   INT   LONG   REGISTER   RETURN   SHORT   SIGNED   SIZEOF   STATIC   STRUCT   SWITCH   TYPEDEF   UNION   UNSIGNED   VOID   VOLATILE   WHILE   MAIN   PLUS   MINUS   TIMES   DIV   MODULO   EQUALS   BITAND   BITOR   BITXOR   NOT   LESS   GREATER   HASH   DOLLAR   ATRATE   LS   RS   LB   RB   LP   RP   QMARK   COLON   DOT   SEMI   COMMA   PLUSEQ   MINUSEQ   TIMESEQ   DIVEQ   MODULOEQ   EMPEQ   AND   OR   REQUALS   NOTEQUAL   LESSEQ   GREATEREQ   INCR   DECR   LSHIFT   RSHIFT   OREQ   XOREQ   PTRREF   LSHIFTEQ   RSHIFTEQ NUMBER IDENTIFIER REAL

%%
Start:		Function 
			;

Function: 	Type IDENTIFIER  LP ArgList RP CompoundStmt 
			;
			
ArgList:  	Arg 
			| ArgList COMMA Arg 
			| /* empty */
			;

Arg:	    Type IDENTIFIER
			;

Declaration: Type IdentList SEMI
			;

Type:		INT 
			| FLOAT
			;

IdentList:   IDENTIFIER COMMA IdentList 
			| IDENTIFIER
			;


Stmt:   	ForStmt 
			| WhileStmt 
			| Expr SEMI 
			| IfStmt 
			| CompoundStmt 
			| Declaration 
			| SEMI
			;

ForStmt:	FOR LP Expr SEMI OptExpr SEMI OptExpr RP Stmt
			;

OptExpr:   	Expr  
			| /* empty */
			;

WhileStmt:  WHILE LP Expr RP Stmt
			;

IfStmt:   	IF LP Expr RP Stmt ElsePart
			;

ElsePart:   ELSE Stmt 
			| /* empty */
			;

CompoundStmt:   LB StmtList RB

StmtList:   StmtList Stmt 
			| /* empty */
			;

Expr:   	IDENTIFIER EQUALS Expr 
			| Rvalue
			;

Rvalue:	    Rvalue Compare Mag 
			| Mag
			;

Compare:   	REQUALS 
			| LESS
			| GREATER
			| LESSEQ 
			| GREATEREQ 
			| NOTEQUAL
			;

Mag:   		Mag PLUS Term 
			| Mag MINUS Term 
			| Term
			;

Term:   	Term TIMES Factor 
			| Term DIV Factor 
			| Factor
			;

Factor:   	LP Expr RP 
			| MINUS Factor 
			| PLUS Factor 
			| IDENTIFIER 
			| NUMBER
			;
%%

void yyerror(char *s) {
	fprintf(stderr, "Error at Line no. : %d, %s\n", yylineno,s);
}

