%token TK_CHAR TK_INT TK_FLOAT TK_IF  TK_ELSE  TK_WHILE TK_VOID  TK_RETURN TK_NEW  TK_AND  TK_OR  TK_EQ  TK_NEQ  TK_GEQ  TK_LEQ  TK_ID TK_LITERALINT TK_LITERALFLOAT TK_LITERALSTRING

%{
    void yyerror(char *);
    int yylex(void);
    int sym[26];
%}

%%
programa : programa declaracao | ;

declaracao : decvariavel | decfuncao ;

decvariavel : tipo listanomes ';' ;

listanomes : TK_ID | listanomes ',' TK_ID ;

tipo : tipobase | tipo '[' ']';

tipobase : TK_INT |  TK_FLOAT  | TK_CHAR;

decfuncao : tipo TK_ID '(' listaparametros ')' bloco | TK_VOID TK_ID '(' listaparametros ')' bloco;

listaparametros : parametros | ;

parametros : parametro | parametros ',' parametro ;

parametro : tipo TK_ID;

bloco : '{'  decsvariaveis  comandos  '}';

decsvariaveis: decsvariaveis decvariavel | ;

comandos: comandos comando | ;

comando : TK_IF '(' exp ')' comando parteelse
        | TK_WHILE '(' exp ')' comando
        | var '=' exp ';'
        | TK_RETURN [ exp ] ';'
        | chamada ';'
        | bloco
        ;
parteelse : TK_ELSE comando | ;
var : TK_ID | exp '[' exp ']' ;
exp : TK_LITERALINT 
    | var
    | '(' exp ')'
    | chamada
    | TK_NEW tipo '[' exp ']'
    | '-' exp
    | exp '+' exp
    | exp '-' exp
    | exp '*' exp
    | exp '/' exp
    | exp TK_EQ exp
    | exp TK_LEQ exp
    | exp TK_GEQ exp
    | exp '<' exp
    | exp '>' exp
    | '!' exp
    | exp TK_AND exp
    | exp TK_OR exp
    ;
chamada : TK_ID '(' listaexp ')';
listaexp : exps | ;
exps : exp |exps ',' exp ;
%%
