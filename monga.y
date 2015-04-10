%token TK_CHAR TK_INT TK_FLOAT TK_IF  TK_ELSE  TK_WHILE TK_VOID  TK_RETURN TK_NEW  TK_AND  TK_OR  TK_EQ  TK_NEQ  TK_GEQ  TK_LEQ  TK_ID TK_LITERALINT TK_LITERALFLOAT TK_LITERALSTRING
%nonassoc IFX
%nonassoc TK_ELSE

%{
    #include <stdio.h>
    void yyerror(char *);
    int yylex(void);
    int sym[26];
%}
%union {
    int intval;
    float floatval;
    char * stringval;
    char * name;
};
%%
programa : programa declaracao 
         | 
         ;

declaracao : decvariavel 
           | decfuncao 
           ;

decvariavel : tipo listanomes ';' 
            ;

listanomes : TK_ID 
           | listanomes ',' TK_ID 
           ;

tipo : tipobase 
     | tipo '[' ']'
     ;

tipobase : TK_INT 
         | TK_FLOAT  
         | TK_CHAR
         ;

decfuncao : tipo TK_ID '(' listaparametros ')' bloco 
          | TK_VOID TK_ID '(' listaparametros ')' bloco
          ;

listaparametros : parametros 
                | 
                ;

parametros : parametro 
           | parametros ',' parametro 
           ;

parametro : tipo TK_ID
          ;

bloco : '{'  decsvariaveis  comandos  '}'
      ;

decsvariaveis: decsvariaveis decvariavel 
             | 
             ;

comandos: comandos comando 
        | 
        ;

comando : TK_IF '(' exp ')' comando %prec IFX
        | TK_IF '(' exp ')' comando TK_ELSE comando
        | TK_WHILE '(' exp ')' comando
        | var '=' exp ';'
        | TK_RETURN [ exp ] ';'
        | chamada ';'
        | bloco
        ;

var : TK_ID 
    | exp '[' exp ']' 
    ;

boolexp: compexp 
       | boolexp TK_AND compexp
       | boolexp TK_OR compexp
       ;

compexp: addexp
       | compexp TK_EQ addexp
       | compexp TK_LEQ addexp
       | compexp TK_GEQ addexp
       | compexp '<' addexp
       | compexp '>' addexp
       ;

addexp: multexp
      | addexp '+' multexp
      | addexp '-' multexp
      ;

multexp: unaryexp
       | unaryexp '*' multexp
       | unaryexp '/' multexp
       | unaryexp '%' multexp
       ;

unaryexp: '-' exp 
   | '!' exp
   | exp
   ;

exp : TK_LITERALINT 
    | TK_LITERALFLOAT 
    | TK_LITERALSTRING 
    | var
    | '(' boolexp ')'
    | chamada
    | TK_NEW tipo '[' boolexp ']'
    ;

chamada : TK_ID '(' listaexp ')';

listaexp : exps 
         | 
         ;

exps : exp 
     | exps ',' exp 
     ;
%%
void yyerror(char *s) {
 fprintf(stderr, "%s\n", s);
}
int main(void) {
 yyparse();
 return 0;
}
