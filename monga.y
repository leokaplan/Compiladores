%{
/*
   Arquivo monga.y
   Bernardo Alkmim 1210514
   Leonardo Kaplan 1212509
*/

#include <stdio.h>
#include "abstractsyntaxtree.h"

#define DEBUG(X) printf(X)
#define DEBUG(x) 

void yyerror(char *);
int yylex(void);
int sym[26];
extern int currentLine;
extern char yytext[];

%}

%error-verbose

%union {
	int intval;
	float floatval;
	char * stringval;
	char * name;
}

%token TK_CHAR 
%token TK_INT
%token TK_FLOAT
%token TK_IF
%token TK_ELSE
%token TK_WHILE
%token TK_VOID
%token TK_RETURN
%token TK_NEW
%token TK_AND
%token TK_OR
%token TK_EQ 
%token TK_NEQ 
%token TK_GEQ
%token TK_LEQ 
%token <name>      TK_ID 
%token <intval>    TK_LITERALINT 
%token <floatval>  TK_LITERALFLOAT
%token <stringval> TK_LITERALSTRING

%type <intval> exp exps declaracao decvariavel decfuncao tipo tipobase listaparametros parametros parametro bloco decsvariaveis comandos comando var comandoreturn boolexp chamada compexp addexp multexp listaexp  


%nonassoc IF_NO_ELSE
%nonassoc TK_ELSE
%nonassoc UN_MINUS
%nonassoc '['

%%

programa : programa declaracao 								{ DEBUG("\n programa"); } 
|													{ } 
;

declaracao : decvariavel 									{ $$ = $1; } 
| decfuncao 										{ $$ = $1; } 
;

decvariavel : tipo listanomes ';' 							{ }
;

listanomes : TK_ID 											{ $$ = AST_id($1); }
| listanomes ',' TK_ID 							{ $$ = AST_opr($2, 2, $1, AST_id($3)); }
;

tipo : tipobase 											{ $$ = $1; }
| tipo '[' ']' 										{ $$ = AST_opr(tipo, 1, $1); }
;

tipobase : TK_INT 											{ $$ = AST_opr(tipobase, 1, $1); }
| TK_FLOAT 										{ $$ = AST_opr(tipobase, 1, $1); }
| TK_CHAR 											{ $$ = AST_opr(tipobase, 1, $1); } 
;

decfuncao : tipo TK_ID '(' listaparametros ')' bloco 		{ DEBUG("\nfunc tipo"); } 
| TK_VOID TK_ID '(' listaparametros ')' bloco 	{ DEBUG("\nfunc void"); } 
;

listaparametros : parametros 								{ $$ = $1; }
| 											{ } 
;

parametros : parametro 										{ $$ = $1; }
| parametros ',' parametro 						{ $$ = AST_opr($2, 2, $1, $3); }
;

parametro : tipo TK_ID 										{ DEBUG("parametro tipo: id:"); }
;

bloco : '{'  decsvariaveis  comandos  '}' 					{ }
;

decsvariaveis: decsvariaveis decvariavel 					{ }
| 												{ }
;

comandos: comandos comando 									{ }
| 													{ }
;

comando : TK_IF '(' boolexp ')' comando %prec IF_NO_ELSE 	{ $$ = AST_opr($1, 2, $3, $5); }
| TK_IF '(' boolexp ')' comando TK_ELSE comando 	{ $$ = AST_opr($1, 3, $3, $5, $7); }
| TK_WHILE '(' boolexp ')' comando 					{ $$ = AST_opr($1, 2, $3, $5); }
| var '=' boolexp ';' 								{ $$ = AST_opr($2, 2, $1, $3); }
| comandoreturn ';' 								{ $$ = $1; }
| chamada ';' 										{ $$ = $1; }
| bloco 											{ $$ = $1; }
;

var : TK_ID 												{ $$ = AST_id($1); } 
| boolexp '[' boolexp ']' %prec '['								{ DEBUG(" indexavel "); }
;


comandoreturn: TK_RETURN 									{ $$ = AST_opr($1, 1, NULL); }
| TK_RETURN boolexp 							{ $$ = AST_opr($1, 1, $2);}
;


boolexp: compexp 											{ $$ = $1; }
| boolexp TK_AND compexp 							{ $$ = AST_opr($2, 2, $1, $3); }
| boolexp TK_OR compexp 								{ $$ = AST_opr($2, 2, $1, $3); }
;

compexp: addexp 											{ $$ = $1; }
| compexp TK_EQ addexp 								{ $$ = AST_opr($2, 2, $1, $3); }
| compexp TK_LEQ addexp 								{ $$ = AST_opr($2, 2, $1, $3); }
| compexp TK_GEQ addexp 								{ $$ = AST_opr($2, 2, $1, $3); }
| compexp '<' addexp 								{ $$ = AST_opr($2, 2, $1, $3); }
| compexp '>' addexp 								{ $$ = AST_opr($2, 2, $1, $3); }
;

addexp: multexp 											{ $$ = $1; }
| addexp '+' multexp									{ $$ = AST_opr($2, 2, $1, $3); }
| addexp '-' multexp									{ $$ = AST_opr($2, 2, $1, $3); }
;

multexp: exp 												{ $$ = $1; }
| multexp '*' exp 									{ $$ = AST_opr($2, 2, $1, $3); }
| multexp '/' exp 									{ $$ = AST_opr($2, 2, $1, $3); }
| multexp '%' exp 									{ $$ = AST_opr($2, 2, $1, $3); }
;

exp : '-' exp %prec UN_MINUS												{ /* TODO ver prioridade */ $$ = AST_opr($1, 1, $2); }
| '!' exp %prec '!'												{ /* TODO ver prioridade */ $$ = AST_opr($1, 1, $2); }
| TK_LITERALINT 										{ $$ = AST_litInt($1); } 
| TK_LITERALFLOAT  										{ $$ = AST_litFloat($1); }
| TK_LITERALSTRING 										{ $$ = AST_litString($1); }
| var 													{ /* TODO provavelmente tem que fazer mais coisa */ $$ = $1; }
| '(' boolexp ')' 										{ $$ = $2; }
| chamada 												{ $$ = $1; }
| TK_NEW tipo '[' boolexp ']' %prec '['							{ }
;

chamada : TK_ID '(' listaexp ')' 							{ $$ = AST_opr(chamada, 2, id($1), $3); }
;

listaexp : exps 											{ $$ = $1; }
| 													{ }
;

exps : boolexp 												{ $$ = $1; }
| exps ',' boolexp 									{ $$ = AST_opr($2, 2, $1, $3); }
;

%%

void yyerror (char * s) {
	fprintf(stderr, "line %d: %s \n", currentLine, s);
}

int main (void) {
	if(!yyparse())
		printf("\n\nparsing finished\n\n");
	else
		printf("\n\nparsing error\n\n");
	return 0;
}
