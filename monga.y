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


%%

programa : programa declaracao 								{ DEBUG("\n programa"); } 
         | 													{ } 
         ;

declaracao : decvariavel 									{ $$ = $1; } 
           | decfuncao 										{ $$ = $1; } 
           ;

decvariavel : tipo listanomes ';' 							{ }
            ;

listanomes : TK_ID 											{ DEBUG(" var id "); }
           | listanomes ',' TK_ID 							{ DEBUG(", var id "); }
           ;

tipo : tipobase 											{ $$ = $1; }
     | tipo '[' ']' 										{ // provavelmente tem que fazer alguma louca com array na ast
															  $$ = $1; }
     ;

tipobase : TK_INT 											{ DEBUG(" T(int) "); }
         | TK_FLOAT 										{ DEBUG(" T(float "); }
         | TK_CHAR 											{ DEBUG(" T(char) "); } 
         ;

decfuncao : tipo TK_ID '(' listaparametros ')' bloco 		{ DEBUG("\nfunc tipo"); } 
          | TK_VOID TK_ID '(' listaparametros ')' bloco 	{ DEBUG("\nfunc void"); } 
          ;

listaparametros : parametros 								{ $$ = $1; }
                | 											{ $$ = NULL; } 
                ;

parametros : parametro 										{ $$ = $1; }
           | parametros ',' parametro 						{ }
           ;

parametro : tipo TK_ID 										{ DEBUG("parametro tipo: id:"); }
          ;

bloco : '{'  decsvariaveis  comandos  '}' 					{ }
      ;

decsvariaveis: decsvariaveis decvariavel 					{ }
             | 												{ $$ = NULL; }
             ;

comandos: comandos comando 									{ }
        | 													{ $$ = NULL;}
        ;

comando : TK_IF '(' boolexp ')' comando %prec IF_NO_ELSE 	{ DEBUG("\nif sem else"); }
        | TK_IF '(' boolexp ')' comando TK_ELSE comando 	{ DEBUG("\nif com else"); }
        | TK_WHILE '(' boolexp ')' comando 					{ DEBUG("\nwhile"); }
        | var '=' boolexp ';' 								{ DEBUG("\natribuicao"); }
        | comandoreturn ';' 								{ $$ = $1; }
        | chamada ';' 										{ $$ = $1; }
        | bloco 											{ $$ = $1; }
        ;

var : TK_ID 												{ DEBUG(" id "); } 
    | boolexp '[' boolexp ']' 								{ DEBUG(" indexavel "); }
    ;


comandoreturn: TK_RETURN 									{ }
     	     | TK_RETURN boolexp 							{ }
             ;


boolexp: compexp 											{ $$ = $1; }
       | boolexp TK_AND compexp 							{ DEBUG(" and "); }
       | boolexp TK_OR compexp 								{ DEBUG(" or "); }
       ;

compexp: addexp 											{ $$ = $1; }
       | compexp TK_EQ addexp 								{ DEBUG(" == "); }
       | compexp TK_LEQ addexp 								{ DEBUG(" <= "); }
       | compexp TK_GEQ addexp 								{ DEBUG(" >= "); }
       | compexp '<' addexp 								{ DEBUG(" < "); }
       | compexp '>' addexp 								{ DEBUG(" > "); }
       ;

addexp: multexp 											{ $$ = $1; }
      | addexp '+' multexp									{ DEBUG(" + "); }
      | addexp '-' multexp									{ DEBUG(" - "); }
      ;

multexp: exp 												{ $$ = $1; }
       | multexp '*' exp 									{ DEBUG(" * "); }
       | multexp '/' exp 									{ DEBUG(" / "); }
       | multexp '%' exp 									{ DEBUG(" % "); }
       ;

exp : '-' exp 												{ // provavelmente tem que fazer mais coisa
															  $$ = $2; }
    | '!' exp 												{ // provavelmente tem que fazer mais coisa
															  $$ = $2; }
    | TK_LITERALINT 										{ DEBUG(" L(int) "); } 
    | TK_LITERALFLOAT  										{ DEBUG(" L(float) "); }
    | TK_LITERALSTRING 										{ DEBUG(" L(string) "); }
    | var 													{ // provavelmente tem que fazer mais coisa
															  $$ = $1; }
    | '(' boolexp ')' 										{ $$ = $2; }
    | chamada 												{ $$ = $1; }
    | TK_NEW tipo '[' boolexp ']' 							{ }
    ;

chamada : TK_ID '(' listaexp ')' 							{ }
        ;

listaexp : exps 											{ $$ = $1; }
         | 													{ $$ = NULL; }
         ;

exps : boolexp 												{ $$ = $1; }
     | exps ',' boolexp 									{ }
     ;

%%

void yyerror(char *s) {
    fprintf(stderr, "line %d: %s \n", currentLine, s);
}

int main(void) {
	if(!yyparse())
    	printf("\n\nparsing finished\n\n");
 	else
    	printf("\n\nparsing error\n\n");
 	return 0;
}
