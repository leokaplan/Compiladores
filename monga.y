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
programa : programa declaracao {printf("\n programa");} 
         | {printf("\n programa vazio");} 
         ;

declaracao : decvariavel {printf("\ndec var");} 
           | decfuncao {printf("\ndec func");} 
           ;

decvariavel : tipo listanomes ';' 
            ;

listanomes : TK_ID {printf(" var id ");}
           | listanomes ',' TK_ID {printf(", var id ");}
           ;

tipo : tipobase 
     | tipo '[' ']'
     ;

tipobase : TK_INT {printf(" T(int) ");} 
         | TK_FLOAT {printf(" T(float ");}  
         | TK_CHAR {printf(" T(char) ");} 
         ;

decfuncao : tipo TK_ID '(' listaparametros ')' bloco {printf("\nfunc tipo");} 
          | TK_VOID TK_ID '(' listaparametros ')' bloco {printf("\nfunc void");} 
          ;

listaparametros : parametros {printf("\nparametros");} 
                | {printf("\nsem parametros");} 
                ;

parametros : parametro 
           | parametros ',' parametro 
           ;

parametro : tipo TK_ID {printf("parametro tipo: id:");}
          ;

bloco : '{'  decsvariaveis  comandos  '}'
      ;

decsvariaveis: decsvariaveis decvariavel 
             | 
             ;

comandos: comandos comando 
        | 
        ;

comando : TK_IF '(' boolexp ')' comando %prec IF_NO_ELSE  {printf("\nif sem else");}
        | TK_IF '(' boolexp ')' comando TK_ELSE comando {printf("\nif com else");}
        | TK_WHILE '(' boolexp ')' comando {printf("\nwhile");}
        | var '=' boolexp ';' {printf("\natribuicao");}
        | comandoreturn ';' {printf("\nreturn ");}
        | chamada ';' {printf("\nchamada");}
        | bloco {printf("\nbloco");}
        ;

var : TK_ID {printf(" id ");} 
    | boolexp '[' boolexp ']' {printf(" indexavel ");}


comandoreturn: TK_RETURN 
     	     | TK_RETURN boolexp 
	         ;


boolexp: compexp 
       | boolexp TK_AND compexp {printf(" and ");}
       | boolexp TK_OR compexp {printf(" or ");}
       ;

compexp: addexp
       | compexp TK_EQ addexp {printf(" == ");}
       | compexp TK_LEQ addexp {printf(" <= ");}
       | compexp TK_GEQ addexp {printf(" >= ");}
       | compexp '<' addexp {printf(" < ");}
       | compexp '>' addexp {printf(" > ");}
       ;

addexp: multexp
      | addexp '+' multexp{printf(" + ");}
      | addexp '-' multexp{printf(" - ");}
      ;

multexp: unaryexp
       | multexp '*' unaryexp {printf(" * ");}
       | multexp '/' unaryexp {printf(" / ");}
       | multexp '%' unaryexp {printf(" % ");}
       ;

unaryexp: '-' unaryexp 
   | '!' unaryexp
   | exp
   ;

exp : TK_LITERALINT {printf(" L(int) ");} 
    | TK_LITERALFLOAT  {printf(" L(float) ");}
    | TK_LITERALSTRING  {printf(" L(string) ");}
    | var {printf(" var ");}
    | '(' boolexp ')'
    | chamada
    | TK_NEW tipo '[' boolexp ']'
    ;

chamada : TK_ID '(' listaexp ')'
        ;

listaexp : exps 
         | 
         ;

exps : boolexp 
     | exps ',' boolexp 
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
