%{
/*
----------------------------------
Trabalho de Compiladores INF1715
----------------------------------
Bernardo Pinto de Alkmim - 1210514
Leonardo Kaplan - 1212509

Arquivo .lex com as expressoes regulares e regras de tratamento de tokens
*/
#include <stdlib.h>
#include "abstractsyntaxtree.h"

/*
    Macro para mensagens de erro
*/
#define ERROR(...); fprintf(stderr,__VA_ARGS__);exit(0);


/*
    Variavel para controle da linha atual do arquivo (e' incrementada a cada '\n' lido).
    Ela permite que as mensagens de erro sejam mais precisas.
*/
int currentLine = 1;

/*
    Copia uma string, ignorando aspas e tratando escapes. Chamada por escapeddupl().
    
    @param dst String destino
           src String fonte
           len Tamanho de src
*/
void cpy(char * dst, char * src, size_t len);

/*
    Duplica a string recebida por parametro. Utilizada para IDs.
    
    @param s String fonte
    
    @return String duplicada
*/
char * dupl(char * s);

/*
    Duplica a string recebida por parametro, tratando escapes. Utilizada para literais string.
    
    @param s String fonte
    
    @return String duplicada
*/
char * escapeddupl(char * s);

/*
    Trata escapes. Se o primeiro caracter for um '\', é retornado o segundo escapado (ex.: '\' 'n' -> '\n').
    Caso contrário, retorna-se o primeiro caracter.
    
    @param a Primeiro caracter
           b Segundo caracter
    @return O caractere a ser utilizado
*/
char escape(char a,char b);


%}


ID              [a-zA-Z_][a-zA-Z0-9_]*
DECIMALINT      [1-9][0-9]*|0
HEXAINT         0[xX][0-9a-fA-F]+
LITERALINT      {DECIMALINT}|{HEXAINT}
LITERALCHAR     \'([^\\]|\\.)\'
DECIMALFLOAT    ((([0-9]+\.[0-9]*)|([0-9]*\.[0-9]+))([eE][+\-]?[0-9]+)?)|([0-9]+[eE][+\-]?[0-9]+)
HEXAFLOAT       0[xX](([0-9a-fA-F]*\.[0-9a-fA-F]+)|([0-9a-fA-F]+\.[0-9a-fA-F]*)|([0-9a-fA-F]+))[pP][+\-]?[0-9]+
LITERALFLOAT    ({DECIMALFLOAT}|{HEXAFLOAT})[fF]?
LITERALSTRING   \"([^\\\"]|\\.)*\"
WHITESPACE      [\t\f\r\ ]+
NEWLINE         \n
COMMENT         (\/\*)([^\*])*([\*]+\/)
UNITARY         [][{}(),;+\-*/=<>!]

%x INSIDE_COMMENT
%%
<INITIAL>{
    "/*"        BEGIN(INSIDE_COMMENT);
}
<INSIDE_COMMENT>{
    "*/"        BEGIN(INITIAL);
    [^*\n]+     { }
    "*"         { }
    \n          { currentLine++; }
}
{WHITESPACE}    { }
{NEWLINE}       { currentLine++; }
"char"          { yylval.intval = currentLine; return TK_CHAR; }
"int"           { yylval.intval = currentLine; return TK_INT; } 
"float"         { yylval.intval = currentLine; return TK_FLOAT; } 
"boolean"       { yylval.intval = currentLine; return TK_BOOL; } 
"if"            { yylval.intval = currentLine; return TK_IF; } 
"else"          { yylval.intval = currentLine; return TK_ELSE; } 
"while"         { yylval.intval = currentLine; return TK_WHILE; } 
"void"          { yylval.intval = currentLine; return TK_VOID; } 
"return"        { yylval.intval = currentLine; return TK_RETURN; } 
"new"           { yylval.intval = currentLine; return TK_NEW; } 
"&&"            { yylval.intval = currentLine; return TK_AND; } 
"||"            { yylval.intval = currentLine; return TK_OR; } 
"=="            { yylval.intval = currentLine; return TK_EQ; } 
"!="            { yylval.intval = currentLine; return TK_NEQ; } 
">="            { yylval.intval = currentLine; return TK_GEQ; } 
"<="            { yylval.intval = currentLine; return TK_LEQ; } 
{LITERALCHAR}   { yylval.intval = escape(yytext[1],yytext[2]); return TK_LITERALINT; }
{LITERALINT}    { yylval.intval = strtol(yytext, NULL, 0); return TK_LITERALINT; }
{LITERALFLOAT}  { yylval.floatval = strtof(yytext, NULL); return TK_LITERALFLOAT; }
{LITERALSTRING} { yylval.stringval = escapeddupl(yytext); return TK_LITERALSTRING; }
"true"          { yylval.intval = 1; return TK_LITERALBOOL; }
"false"         { yylval.intval = 0; return TK_LITERALBOOL; }
{ID}            { 
                    yylval.identifier.name = dupl(yytext); 
                    yylval.identifier.line = currentLine; 
                    return TK_ID; 
                }
{UNITARY}       { yylval.intval = currentLine; return yytext[0]; } 
.               { yyerror("invalid character sequence"); }   

%%

int yywrap(void) {
    return 1;
}

char escape(char a, char b) {
    if(a == '\\') {
        switch(b) {
            case 't':
                return '\t';
                break;
            case '"':
                return '"';
                break;
            case 'n':
                return '\n';
                break;
            default:
                yyerror("invalid escape character");
                break;
        }
    }
    else {
        return a;
    }
}
void cpy(char * dst, char * src, size_t len) {
    size_t i, j;

    // As aspas de src nao vao para dst
    len -= 2;

    // i vai de 1 a (len - 1), j vai de 0 a (len - 2)
    // lembrando que as aspas de src nao devem ir para dst
    for(i = 1, j = 0; i < len; i++, j++) {
        if(src[i] != '\\') {
            dst[j] = src[i];
        }
        else {
            dst[j] = escape(src[i], src[i+1]);
            i++;
        }
    }
}

char * dupl(char * s) {
    size_t slen = strlen(s);
    char * d = (char *) malloc(slen + 1);
    if(d == NULL) { 
        yyerror("not enough memory to scan identifier");
    }
    memcpy(d, s, slen+1);
    return d;
}

char * escapeddupl(char * s) {
    size_t slen = strlen(s);
    char* d = (char *) malloc(slen + 1);
    if(d==NULL) { 
        yyerror("not enough memory to scan literal");
    }
    cpy(d, s, slen+1);
    return d;
}
