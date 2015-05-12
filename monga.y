%{
/*
   Arquivo monga.y
   Bernardo Alkmim 1210514
   Leonardo Kaplan 1212509
*/

#include <stdio.h>
#include "abstractsyntaxtree.h"


void yyerror(char *);
int yylex(void);
int sym[26];
extern int currentLine;
extern char yytext[];

AST_nodeType * prog = NULL;

%}

%error-verbose

%union {
	int intval;	
	float floatval;
	char * stringval;
	char * name;
	AST_nodeType * node;	
}

%nonassoc IF_NO_ELSE
%nonassoc TK_ELSE
%nonassoc '['
%nonassoc UN_MINUS
%nonassoc '!'

%token <intval> TK_CHAR 
%token <intval> TK_INT
%token <intval> TK_FLOAT
%token <intval> TK_IF
%token <intval> TK_ELSE
%token <intval> TK_WHILE
%token <intval> TK_VOID
%token <intval> TK_RETURN
%token <intval> TK_NEW
%token <intval> TK_AND
%token <intval> TK_OR
%token <intval> TK_EQ 
%token <intval> TK_NEQ 
%token <intval> TK_GEQ
%token <intval> TK_LEQ
%token <name> TK_ID
%token <intval> TK_LITERALINT
%token <floatval> TK_LITERALFLOAT
%token <stringval> TK_LITERALSTRING

%type <intval> '>' '<' '+' '-' '*' '/' '%' '!'
%type <node> programa listanomes declaracoes declaracao decvariavel decfuncao decsvariaveis listaparametros parametros parametro bloco comandos comando comandoreturn simpleexp exps logicexp chamada compexp addexp multexp listaexp var tipobase tipo unaryexp

%%

programa : declaracoes	 	{ $$ = $1; prog = $$; AST_draw(prog); }
|				{ $$ = NULL; } 
;

declaracoes : declaracoes declaracao	{ $$ = AST_handleList($1, $2); }
|					{ $$ = NULL; }
;

declaracao : decvariavel 	{ $$ = $1; } 
| decfuncao 			{ $$ = $1; } 
;

decvariavel : tipo listanomes ';' 	{ $$ = AST_decl_var($1, $2); }
;

listanomes : TK_ID 		{ $$ = AST_id($1); }
| listanomes ',' TK_ID 		{ $$ = AST_handleList($1, AST_id($3)); }
;

tipo : tipobase 		{ $$ = $1; }
| tipo '[' ']' 			{ $$ = $1; AST_incInd($$); }
;

tipobase : TK_INT 		{ $$ = AST_type(INT, 0); }
| TK_FLOAT 			{ $$ = AST_type(FLOAT, 0); }
| TK_CHAR 			{ $$ = AST_type(CHAR, 0); } 
;

decfuncao : tipo TK_ID '(' listaparametros ')' bloco 	{ $$ = AST_decl_func($1, AST_id($2), $4, $6); } 
| TK_VOID TK_ID '(' listaparametros ')' bloco 		{ $$ = AST_decl_func(AST_type(VOID, 0), AST_id($2), $4, $6); } 
;

listaparametros : parametros 			{ $$ = $1; }
| 						{ $$ = NULL; } 
;

parametros : parametro 				{ $$ = $1; }
| parametros ',' parametro 			{ $$ = AST_handleList($1, $3); }
;

parametro : tipo TK_ID 				{ $$ = AST_decl_var($1, AST_id($2)); }
;

bloco : '{'  decsvariaveis  comandos  '}' 	{ $$ = AST_cmd_block($2, $3); }
;

decsvariaveis: decsvariaveis decvariavel 	{ $$ = AST_handleList($1, $2); }
| 						{ $$ = NULL; }
;

comandos: comandos comando 			{ $$ = AST_handleList($1, $2); }
| 						{ $$ = NULL; }
;

comando : TK_IF '(' logicexp ')' comando %prec IF_NO_ELSE 	{ $$ = AST_cmd_if($3, $5, NULL); }
| TK_IF '(' logicexp ')' comando TK_ELSE comando { $$ = AST_cmd_if($3, $5, $7); }
| TK_WHILE '(' logicexp ')' comando 		{ $$ = AST_cmd_while($3, $5); }
| var '=' logicexp ';' 				{ $$ = AST_cmd_attr($1, $3); }
| comandoreturn ';' 				{ $$ = $1; }
| logicexp ';' 					{ $$ = AST_cmd_exp($1); }
| bloco 					{ $$ = $1; }
;

var : TK_ID 				{ $$ = AST_var_simple(AST_id($1)); } 
| simpleexp '[' logicexp ']'		{ $$ = AST_var_array($1, $3); }
;

comandoreturn: TK_RETURN 		{ $$ = AST_cmd_ret(NULL); }
| TK_RETURN logicexp 			{ $$ = AST_cmd_ret($2); }
;

logicexp: compexp 			{ $$ = $1; }
| logicexp TK_AND compexp 		{ $$ = AST_exp_opr($2, $1, $3); }
| logicexp TK_OR compexp 		{ $$ = AST_exp_opr($2, $1, $3); }
;

compexp: addexp 			{ $$ = $1; }
| compexp TK_EQ addexp 			{ $$ = AST_exp_opr($2, $1, $3); }
| compexp TK_NEQ addexp			{ $$ = AST_exp_opr($2, $1, $3); }
| compexp TK_LEQ addexp 		{ $$ = AST_exp_opr($2, $1, $3); }
| compexp TK_GEQ addexp 		{ $$ = AST_exp_opr($2, $1, $3); }
| compexp '<' addexp 			{ $$ = AST_exp_opr($2, $1, $3); }
| compexp '>' addexp 			{ $$ = AST_exp_opr($2, $1, $3); }
;

addexp: multexp 			{ $$ = $1; }
| addexp '+' multexp			{ $$ = AST_exp_opr($2, $1, $3); }
| addexp '-' multexp			{ $$ = AST_exp_opr($2, $1, $3); }
;

multexp: unaryexp 			{ $$ = $1; }
| multexp '*' unaryexp 			{ $$ = AST_exp_opr($2, $1, $3); }
| multexp '/' unaryexp 			{ $$ = AST_exp_opr($2, $1, $3); }
| multexp '%' unaryexp 			{ $$ = AST_exp_opr($2, $1, $3); }
;

unaryexp: simpleexp			{ $$ = $1; }
| '-' simpleexp %prec UN_MINUS		{ $$ = AST_exp_opr($1, $2, NULL); }
| '!' logicexp				{ $$ = AST_exp_opr($1, $2, NULL); }
;

simpleexp : TK_LITERALINT 		{ $$ = AST_litInt($1); } 
| TK_LITERALFLOAT  			{ $$ = AST_litFloat($1); }
| TK_LITERALSTRING 			{ $$ = AST_litString($1); }
| var 					{ $$ = AST_exp_var($1); }
| '(' logicexp ')' 			{ $$ = $2; }
| chamada 				{ $$ = $1; }
| TK_NEW tipo '[' logicexp ']'		{ $$ = AST_exp_new($2, $4); }
;

chamada : simpleexp '(' listaexp ')' 	{ $$ = AST_exp_all($1, $3); }
;

listaexp : exps 	{ $$ = $1; }
| 			{ $$ = NULL; }
;

exps : logicexp 		{ $$ = $1; }
| exps ',' logicexp 		{ $$ = AST_handleList($1, $3); }
;

%%

void yyerror (char * s) {
	fprintf(stderr, "line %d: %s \n", currentLine, s);
}

int main (void) {
	if(!yyparse()) {
		printf("\n\nparsing finished\n\n");
	}
	else
		printf("\n\nparsing error\n\n");
	return 0;
}
