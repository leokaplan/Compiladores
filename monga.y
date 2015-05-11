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
	AST_litNodeType * lit;
	AST_typNodeType * typ;
	AST_idNodeType * id;
	AST_expNodeType * expr;
	AST_varNodeType * var;
	AST_declNodeType * decl;
	AST_cmdNodeType * cmd;	
}

%token <typ> TK_CHAR 
%token <typ> TK_INT
%token <typ> TK_FLOAT
%token TK_IF
%token TK_ELSE
%token TK_WHILE
%token <typ> TK_VOID
%token TK_RETURN
%token TK_NEW
%token TK_AND
%token TK_OR
%token TK_EQ 
%token TK_NEQ 
%token TK_GEQ
%token TK_LEQ 
%token TK_ID
%token TK_LITERALINT
%token TK_LITERALFLOAT
%token TK_LITERALSTRING

%type <id>  TK_ID
%type <lit> TK_LITERALINT
%type <lit> TK_LITERALFLOAT
%type <lit> TK_LITERALSTRING

%type <expr> exp 
%type <expr> exps 
%type <decl> declaracao 
%type <decl> decvariavel 
%type <decl> decfuncao 
%type <typ> tipo 
%type <typ> tipobase 
%type <decl> listaparametros  
%type <decl> parametros  
%type <decl> parametro  
%type <cmd> bloco   
%type <decl> decsvariaveis   
%type <cmd> comandos   
%type <cmd> comando   
%type <var> var   
%type <cmd> comandoreturn   
%type <exp> boolexp   
%type <exp> chamada   
%type <exp> compexp   
%type <exp> addexp   
%type <exp> multexp   
%type <exp> listaexp  

%nonassoc IF_NO_ELSE
%nonassoc TK_ELSE
%nonassoc UN_MINUS
%nonassoc '['

%%

programa : programa declaracao 				{ /* TODO */ } 
|							{ $$ = NULL; } 
;

declaracao : decvariavel 				{ $$ = $1; } 
| decfuncao 						{ $$ = $1; } 
;

decvariavel : tipo listanomes ';' 			{ /* TODO */ }
;

listanomes : TK_ID 					{ $$ = AST_id($1); }
| listanomes ',' TK_ID 					{ $$ = AST_handleList($1, AST_id($3)); }
;

tipo : tipobase 					{ $$ = $1; }
| tipo '[' ']' 						{ $1->node.typ.indirections++;
							  $$ = $1; }

;

tipobase : TK_INT 					{ $$ = AST_type(INT, 0); }
| TK_FLOAT 						{ $$ = AST_type(FLOAT, 0); }
| TK_CHAR 						{ $$ = AST_type(CHAR, 0); } 
;

decfuncao : tipo TK_ID '(' listaparametros ')' bloco 	{ /* TODO */ } 
| TK_VOID TK_ID '(' listaparametros ')' bloco 		{ /* TODO */ } 
;

listaparametros : parametros 				{ $$ = $1; }
| 							{ $$ = NULL; } 
;

parametros : parametro 					{ $$ = $1; }
| parametros ',' parametro 				{ $$ = AST_handleList($1, $3); }
;

parametro : tipo TK_ID 					{ /* TODO */ }
;

bloco : '{'  decsvariaveis  comandos  '}' 		{ /* TODO */ }
;

decsvariaveis: decsvariaveis decvariavel 		{ $$ = AST_handleList($1, $2); }
| 							{ $$ = NULL; }
;

comandos: comandos comando 				{ $$ = AST_handleList($1, $2); }
| 							{ $$ = NULL; }
;

comando : TK_IF '(' boolexp ')' comando %prec IF_NO_ELSE 	{ $$ = AST_cmd_if($3, $5, NULL); }
| TK_IF '(' boolexp ')' comando TK_ELSE comando 		{ $$ = AST_cmd_if($3, $5, $7); }
| TK_WHILE '(' boolexp ')' comando 				{ $$ = AST_cmd_while($3, $5); }
| var '=' boolexp ';' 						{ $$ = AST_cmd_attr($1, $3); }
| comandoreturn ';' 						{ $$ = $1; }
| boolexp ';' 							{ $$ = $1; }
| bloco 							{ $$ = $1; }
;

var : TK_ID 				{ $$ = AST_id($1); } 
| boolexp '[' boolexp ']' %prec '['	{ $$ = AST_var_array($1, $2); }
;


comandoreturn: TK_RETURN 			{ $$ = AST_cmd_ret(NULL); }
| TK_RETURN boolexp 				{ $$ = AST_cmd_ret($2);}
;


boolexp: compexp 			{ $$ = $1; }
| boolexp TK_AND compexp 		{ $$ = AST_exp_opr(TK_AND, $1, $3); }
| boolexp TK_OR compexp 		{ $$ = AST_exp_opr(TK_OR, $1, $3); }
;

compexp: addexp 			{ $$ = $1; }
| compexp TK_EQ addexp 			{ $$ = AST_exp_opr(TK_EQ, $1, $3); }
| compexp TK_LEQ addexp 		{ $$ = AST_exp_opr(TK_LEQ, $1, $3); }
| compexp TK_GEQ addexp 		{ $$ = AST_exp_opr(TK_GEQ, $1, $3); }
| compexp '<' addexp 			{ $$ = AST_exp_opr('<', $1, $3); }
| compexp '>' addexp 			{ $$ = AST_exp_opr('>', $1, $3); }
;

addexp: multexp 			{ $$ = $1; }
| addexp '+' multexp			{ $$ = AST_exp_opr('+', $1, $3); }
| addexp '-' multexp			{ $$ = AST_exp_opr('-', $1, $3); }
;

multexp: exp 				{ $$ = $1; }
| multexp '*' exp 			{ $$ = AST_exp_opr('*', $1, $3); }
| multexp '/' exp 			{ $$ = AST_exp_opr('/', $1, $3); }
| multexp '%' exp 			{ $$ = AST_exp_opr('%', $1, $3); }
;

exp : '-' exp %prec UN_MINUS	{ /* TODO ver prioridade */ $$ = AST_exp_opr('-', $2, NULL); }
| '!' exp %prec '!'		{ /* TODO ver prioridade */ $$ = AST_exp_opr('!', $2, NULL); }
| TK_LITERALINT 		{ $$ = AST_litInt($1); } 
| TK_LITERALFLOAT  		{ $$ = AST_litFloat($1); }
| TK_LITERALSTRING 		{ $$ = AST_litString($1); }
| var 				{ /* TODO provavelmente tem que fazer mais coisa */ $$ = $1; }
| '(' boolexp ')' 		{ $$ = $2; }
| chamada 			{ $$ = $1; }
| TK_NEW tipo '[' boolexp ']' %prec '['	{ $$ = AST_exp_new($2, $4); }
;

chamada : TK_ID '(' listaexp ')' { /* TODO */ }
;

listaexp : exps 	{ $$ = $1; }
| 			{ $$ = NULL; }
;

exps : boolexp 		{ $$ = $1; }
| exps ',' boolexp 	{ $$ = AST_handleList($1, $3); }
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
