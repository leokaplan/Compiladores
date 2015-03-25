%{
#include "y.tab.h"
%}

%option noyywrap

ID              [a-zA-Z_][a-zA-Z0-9_]*
LITERALINT      [0-9]+|'0'[xX][0-9a-fA-F]+
DECIMALFLOAT    ((([0-9]+\.[0-9]*)|([0-9]*\.[0-9]+))([eE][+\-]?[0-9]+)?)|([0-9]+[eE][+\-]?[0-9]+)
HEXAFLOAT       '0'[xX](([0-9a-fA-F]*\.[0-9a-fA-F]+)|([0-9a-fA-F]+\.[0-9a-fA-F]*)|([0-9a-fA-F]+))[pP][+\-]?[0-9]+
LITERALFLOAT    ({DECIMALFLOAT}|{HEXAFLOAT})[fF]?
LITERALSTRING   \"([^\\\"]*|\\.)*\"
WHITESPACE      [\t\f\r\ ]+
NEWLINE         \n
COMMENT         "/""*"([^*]|"*"|[^/*])*"*"+"/"
UNITARY         [][{}(),;+\-*/=<>!]

%%

{COMMENT}       {}
{WHITESPACE}    {}
{NEWLINE}       {currentLine++;}
"char"          {return TK_CHAR;}
"int"           {return TK_INT;} 
"float"         {return TK_FLOAT;} 
"if"            {return TK_IF;} 
"else"          {return TK_ELSE;} 
"while"         {return TK_WHILE;} 
"void"          {return TK_VOID;} 
"return"        {return TK_RETURN;} 
"new"           {return TK_NEW;} 
"&&"            {return TK_AND;} 
"||"            {return TK_OR;} 
"=="            {return TK_EQ;} 
"!="            {return TK_NEQ;} 
">="            {return TK_GEQ;} 
"<="            {return TK_LEQ;} 
{ID}            {yylval.name = dupl(yytext); return TK_ID;}
{LITERALINT}    {yylval.intval = atoi(yytext); return TK_LITERALINT;}
{LITERALFLOAT}  {yylval.floatval = atof(yytext); return TK_LITERALFLOAT;}
{LITERALSTRING} {yylval.stringval = escapeddupl(yytext);return TK_LITERALSTRING;}
{UNITARY}       {return yytext[0];} 
.               {ERROR("\nERROR:scanner:unmatched sequence.\n");}   

%%

void cpy(char * dst, char * src, size_t len){
    size_t i;
    for(i = 0;len--;i++){
        if(src[i]!='\\'){
            dst[i] = src[i];
        }
        else{
            switch(src[i+1]){
                case 't':
                    dst[i] = '\t';
                    break;
                case '"':
                    dst[i] = '"';
                    break;
                case 'n':
                    dst[i] = '\n';
                    break;
                default:
                    ERROR("\nERROR:scanner:invalid literal escape\n");
                    break;
            }   
            i++;
        }
    }
}

char * dupl(char * s){
    size_t slen = strlen(s);
    char * d = (char *)malloc(slen + 1);
    if(d==NULL){ 
        ERROR("\nERROR:scanner:not enough memory to scan literal\n");
    }
    memcpy(d,s,slen+1);
    return d;
}
char * escapeddupl(char * s){
    size_t slen = strlen(s);
    char * d = (char *)malloc(slen + 1);
    if(d==NULL){ 
        ERROR("\nERROR:scanner:not enough memory to scan literal\n");
    }
    cpy(d,s,slen+1);
    return d;
}
