%token TK_CHAR TK_INT TK_FLOAT TK_IF  TK_ELSE  TK_WHILE TK_VOID  TK_RETURN TK_NEW  TK_AND  TK_OR  TK_EQ  TK_NEQ  TK_GEQ  TK_LEQ  TK_ID TK_LITERALINT TK_LITERALFLOAT TK_LITERALSTRING
%nonassoc IF_NO_ELSE
%nonassoc TK_ELSE

%{
    #include <stdio.h>
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
};

%%
programa : programa declaracao {printf("programa\n");} 
         | {printf("programa vazio\n");} 
         ;

declaracao : decvariavel {printf("dec var\n");} 
           | decfuncao {printf("dec func\n");} 
           ;

decvariavel : tipo listanomes ';' 
            ;

listanomes : TK_ID 
           | listanomes ',' TK_ID 
           ;

tipo : tipobase 
     | tipo '[' ']'
     ;

tipobase : TK_INT {printf("tipo int\n");} 
         | TK_FLOAT {printf("tipo float\n");}  
         | TK_CHAR {printf("tipo char\n");} 
         ;

decfuncao : tipo TK_ID '(' listaparametros ')' bloco {printf("func tipo\n");} 
          | TK_VOID TK_ID '(' listaparametros ')' bloco {printf("func void\n");} 
          ;

listaparametros : parametros {printf("parametros\n");} 
                | {printf("sem parametros\n");} 
                ;

parametros : parametro 
           | parametros ',' parametro 
           ;

parametro : tipo TK_ID {printf("parametro tipo: id:\n");}
          ;

bloco : '{'  decsvariaveis  comandos  '}'
      ;

decsvariaveis: decsvariaveis decvariavel 
             | 
             ;

comandos: comandos comando 
        | 
        ;

comando : TK_IF '(' exp ')' comando %prec IF_NO_ELSE
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
       | multexp '*' unaryexp
       | multexp '/' unaryexp
       | multexp '%' unaryexp
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

chamada : TK_ID '(' listaexp ')'
        ;

listaexp : exps 
         | 
         ;

exps : exp 
     | exps ',' exp 
     ;
%%
void yyerror(char *s) {
    fprintf(stderr, "line %d: %s\n", currentLine, s);
}
int main(void) {
 if(!yyparse())
    printf("parsing finished\n");
 else
    printf("parsing error\n");
 return 0;
}
