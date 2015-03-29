/*
----------------------------------
Trabalho de Compiladores INF1715
----------------------------------
Bernardo Pinto de Alkmim - 1210514
Leonardo Kaplan - 1xxxxxx

Arquivo de testes do analisador léxico
*/
#include "lex.yy.c"

int main() {
    int token;
    while(token = yylex()) {
        switch(token) {
            case TK_ID:
                printf("ID(%s)\n", yylval.name);
                break;
            case TK_LITERALINT:
                printf("LITERAL(%d)\n", yylval.intval);
                break;
            case TK_LITERALFLOAT:
                printf("LITERAL(%f)\n", yylval.floatval);
                break;
            case TK_LITERALSTRING:
                printf("LITERAL(%s)\n", yylval.stringval);
                break;
            case TK_CHAR: 
                printf("TK_CHAR\n");
                break;
            case TK_INT: 
                printf("TK_INT\n");
                break;
            case TK_FLOAT: 
                printf("TK_FLOAT\n");
                break;
            case TK_IF: 
                printf("TK_IF\n");
                break;
            case TK_ELSE: 
                printf("TK_ELSE\n");
                break;
            case TK_WHILE: 
                printf("TK_WHILE\n");
                break;
            case TK_VOID: 
                printf("TK_VOID\n");
                break;
            case TK_RETURN: 
                printf("TK_RETURN\n");
                break;
            case TK_NEW: 
                printf("TK_NEW\n");
                break;
            case TK_AND: 
                printf("TK_AND\n");
                break;
            case TK_OR: 
                printf("TK_OR\n");
                break;
            case TK_EQ: 
                printf("TK_EQ\n");
                break;
            case TK_NEQ: 
                printf("TK_NEQ\n");
                break;
            case TK_GEQ: 
                printf("TK_GEQ\n");
                break;
            case TK_LEQ: 
                printf("TK_LEQ\n");
                break;
            default:
                printf("%c\n",token);
        }    
    }
    return 0;
}
