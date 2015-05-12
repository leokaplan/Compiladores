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


%}

%error-verbose

%union {
	int intval;	
	float floatval;
	char * stringval;
	char * name;
	AST_nodeType * node;	
}

%token <node> TK_CHAR 
%token <node> TK_INT
%token <node> TK_FLOAT
%token TK_IF
%token TK_ELSE
%token TK_WHILE
%token <node> TK_VOID
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

%type <name> TK_ID
%type <intval> TK_LITERALINT
%type <floatval> TK_LITERALFLOAT
%type <stringval> TK_LITERALSTRING

%type <node> programa
%type <node> listanomes
%type <node> exp 
%type <node> exps 
%type <node> declaracao 
%type <node> decvariavel 
%type <node> decfuncao 
%type <node> tipo 
%type <node> tipobase 
%type <node> listaparametros  
%type <node> parametros  
%type <node> parametro  
%type <node> bloco   
%type <node> decsvariaveis   
%type <node> comandos   
%type <node> comando   
%type <node> var   
%type <node> comandoreturn   
%type <node> boolexp   
%type <node> chamada   
%type <node> compexp   
%type <node> addexp   
%type <node> multexp   
%type <node> listaexp  

%nonassoc IF_NO_ELSE
%nonassoc TK_ELSE
%nonassoc UN_MINUS
%nonassoc '['

%%

programa : programa declaracao 	{ $$ = AST_handleList($1, $2);
				  AST_draw($$); } 
|				{ $$ = NULL; } 
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
| tipo '[' ']' 			{ $$ = $1;
				  AST_incInd($$); }
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

comando : TK_IF '(' boolexp ')' comando %prec IF_NO_ELSE 	{ $$ = AST_cmd_if($3, $5, NULL); }
| TK_IF '(' boolexp ')' comando TK_ELSE comando { $$ = AST_cmd_if($3, $5, $7); }
| TK_WHILE '(' boolexp ')' comando 		{ $$ = AST_cmd_while($3, $5); }
| var '=' boolexp ';' 				{ $$ = AST_cmd_attr($1, $3); }
| comandoreturn ';' 				{ $$ = $1; }
| boolexp ';' 					{ $$ = AST_cmd_exp($1); }
| bloco 					{ $$ = $1; }
;

var : TK_ID 				{ $$ = AST_var_simple(AST_id($1)); } 
| boolexp '[' boolexp ']' %prec '['	{ $$ = AST_var_array($1, $3); }
;


comandoreturn: TK_RETURN 		{ $$ = AST_cmd_ret(NULL); }
| TK_RETURN boolexp 			{ $$ = AST_cmd_ret($2);}
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

exp : '-' exp %prec UN_MINUS		{ $$ = AST_exp_opr('-', $2, NULL); }
| '!' exp %prec '!'			{ $$ = AST_exp_opr('!', $2, NULL); }
| TK_LITERALINT 			{ $$ = AST_litInt($1); } 
| TK_LITERALFLOAT  			{ $$ = AST_litFloat($1); }
| TK_LITERALSTRING 			{ $$ = AST_litString($1); }
| var 					{ $$ = AST_exp_var($1); }
| '(' boolexp ')' 			{ $$ = $2; }
| chamada 				{ $$ = $1; }
| TK_NEW tipo '[' boolexp ']' %prec '['	{ $$ = AST_exp_new($2, $4); }
;

chamada : TK_ID '(' listaexp ')' 	{ $$ = AST_exp_call(AST_id($1), $3); }
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
