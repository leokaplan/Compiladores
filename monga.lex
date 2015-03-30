%{
/*
----------------------------------
Trabalho de Compiladores INF1715
----------------------------------
Bernardo Pinto de Alkmim - 1210514
Leonardo Kaplan - 1212509

Arquivo .lex com as expressões regulares e regras de tratamento de tokens
*/
#include "y.tab.h"
#include <stdlib.h>
%}

%option noyywrap

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
"char"          { return TK_CHAR; }
"int"           { return TK_INT; } 
"float"         { return TK_FLOAT; } 
"if"            { return TK_IF; } 
"else"          { return TK_ELSE; } 
"while"         { return TK_WHILE; } 
"void"          { return TK_VOID; } 
"return"        { return TK_RETURN; } 
"new"           { return TK_NEW; } 
"&&"            { return TK_AND; } 
"||"            { return TK_OR; } 
"=="            { return TK_EQ; } 
"!="            { return TK_NEQ; } 
">="            { return TK_GEQ; } 
"<="            { return TK_LEQ; } 
{LITERALCHAR}   { yylval.intval = escape(yytext[1],yytext[2]); return TK_LITERALINT; }
{LITERALINT}    { yylval.intval = strtol(yytext, NULL, 0); return TK_LITERALINT; }
{LITERALFLOAT}  { yylval.floatval = strtof(yytext, NULL); return TK_LITERALFLOAT; }
{LITERALSTRING} { yylval.stringval = escapeddupl(yytext);return TK_LITERALSTRING; }
{ID}            { yylval.name = dupl(yytext); return TK_ID; }
{UNITARY}       { return yytext[0]; } 
.               { ERROR("\nERROR > scanner > on line %d > unmatched sequence.\n",currentLine); }   

%%
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
                ERROR("\nERROR > scanner > on line %d > invalid literal escape\n",currentLine);
                break;
        }
    }
    else {
        return a;
    }
}
void cpy(char * dst, char * src, size_t len) {
    size_t i, j;

    // As aspas de src não vão para dst
    len -= 2;

    // i vai de 1 a (len - 1), j vai de 0 a (len - 2)
    // lembrando que as aspas de src não devem ir para dst
    for(i = 1, j = 0;i < len ;i++, j++) {
        if(src[i]!='\\') {
            dst[j] = src[i];
        }
        else {
            dst[j] = escape(src[i],src[i+1]);
            i++;
        }
    }
}

char * dupl(char * s) {
    size_t slen = strlen(s);
    char * d = (char *) malloc(slen + 1);
    if(d==NULL) { 
        ERROR("\nERROR > scanner > on line %d > not enough memory to scan ID\n",currentLine);
    }
    memcpy(d,s,slen+1);
    return d;
}

char * escapeddupl(char * s) {
    size_t slen = strlen(s);
    char* d = (char *) malloc(slen + 1);
    if(d==NULL) { 
        ERROR("\nERROR > scanner > on line %d > not enough memory to scan literal\n",currentLine);
    }
    cpy(d, s, slen+1);
    return d;
}
