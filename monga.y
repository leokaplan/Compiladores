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
    #define DEBUG(X) printf(X)
    #define DEBUG(x) 
%}
%error-verbose
%union {
    int intval;
    float floatval;
    char * stringval;
    char * name;
};
%%
programa : programa declaracao {DEBUG("\n programa");} 
         | {DEBUG("\n programa vazio");} 
         ;

declaracao : decvariavel {DEBUG("\ndec var");} 
           | decfuncao {DEBUG("\ndec func");} 
           ;

decvariavel : tipo listanomes ';' 
            ;

listanomes : TK_ID {DEBUG(" var id ");}
           | listanomes ',' TK_ID {DEBUG(", var id ");}
           ;

tipo : tipobase 
     | tipo '[' ']'
     ;

tipobase : TK_INT {DEBUG(" T(int) ");} 
         | TK_FLOAT {DEBUG(" T(float ");}  
         | TK_CHAR {DEBUG(" T(char) ");} 
         ;

decfuncao : tipo TK_ID '(' listaparametros ')' bloco {DEBUG("\nfunc tipo");} 
          | TK_VOID TK_ID '(' listaparametros ')' bloco {DEBUG("\nfunc void");} 
          ;

listaparametros : parametros {DEBUG("\nparametros");} 
                | {DEBUG("\nsem parametros");} 
                ;

parametros : parametro 
           | parametros ',' parametro 
           ;

parametro : tipo TK_ID {DEBUG("parametro tipo: id:");}
          ;

bloco : '{'  decsvariaveis  comandos  '}'
      ;

decsvariaveis: decsvariaveis decvariavel 
             | 
             ;

comandos: comandos comando 
        | 
        ;

comando : TK_IF '(' boolexp ')' comando %prec IF_NO_ELSE  {DEBUG("\nif sem else");}
        | TK_IF '(' boolexp ')' comando TK_ELSE comando {DEBUG("\nif com else");}
        | TK_WHILE '(' boolexp ')' comando {DEBUG("\nwhile");}
        | var '=' boolexp ';' {DEBUG("\natribuicao");}
        | comandoreturn ';' {DEBUG("\nreturn ");}
        | chamada ';' {DEBUG("\nchamada");}
        | bloco {DEBUG("\nbloco");}
        ;

var : TK_ID {DEBUG(" id ");} 
    | boolexp '[' boolexp ']' {DEBUG(" indexavel ");}


comandoreturn: TK_RETURN 
     	     | TK_RETURN boolexp 
	         ;


boolexp: compexp 
       | boolexp TK_AND compexp {DEBUG(" and ");}
       | boolexp TK_OR compexp {DEBUG(" or ");}
       ;

compexp: addexp
       | compexp TK_EQ addexp {DEBUG(" == ");}
       | compexp TK_LEQ addexp {DEBUG(" <= ");}
       | compexp TK_GEQ addexp {DEBUG(" >= ");}
       | compexp '<' addexp {DEBUG(" < ");}
       | compexp '>' addexp {DEBUG(" > ");}
       ;

addexp: multexp
      | addexp '+' multexp{DEBUG(" + ");}
      | addexp '-' multexp{DEBUG(" - ");}
      ;

multexp: unaryexp
       | multexp '*' unaryexp {DEBUG(" * ");}
       | multexp '/' unaryexp {DEBUG(" / ");}
       | multexp '%' unaryexp {DEBUG(" % ");}
       ;

unaryexp: '-' unaryexp 
   | '!' unaryexp
   | exp
   ;

exp : TK_LITERALINT {DEBUG(" L(int) ");} 
    | TK_LITERALFLOAT  {DEBUG(" L(float) ");}
    | TK_LITERALSTRING  {DEBUG(" L(string) ");}
    | var {DEBUG(" var ");}
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
