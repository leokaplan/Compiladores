/*
----------------------------------
Trabalho de Compiladores INF1715
----------------------------------
Bernardo Pinto de Alkmim - 1210514
Leonardo Kaplan - 1xxxxxx

Header do analisador lexico
*/

/*
    Macro para mensagens de erro
*/
#define ERROR(...); fprintf(stderr,__VA_ARGS__);exit(0);

/*
    Enumeracao dos Tokens, a partir de 128 para nao haver conflito com os tokens de caracteres
    avulsos, que sao representados por seus valores na tabela ASCII (como, por exemplo, '{')
*/
enum TOKEN {
    TK_CHAR = 127 + 1,  
    TK_INT,  
    TK_FLOAT, 
    TK_IF,  
    TK_ELSE,  
    TK_WHILE, 
    TK_VOID,  
    TK_RETURN,
    TK_NEW,  
    TK_AND,  
    TK_OR,  
    TK_EQ,  
    TK_NEQ,  
    TK_GEQ,  
    TK_LEQ,  
    TK_ID,
    TK_LITERALINT,
    TK_LITERALFLOAT,
    TK_LITERALSTRING
};

/*
    Union com o valor to token corrente
*/
union {
    int intval;
    float floatval;
    char * stringval;
    char * name;
} yylval;

/*
    Variavel para controle da linha atual do arquivo (e' incrementada a cada '\n' lido).
    Ela permite que as mensagens de erro sejam mais precisas.
*/
int currentLine = 1;

/*



*/
void cpy(char * dst, char * src, size_t len);

/*



*/
char * dupl(char * s);

/*




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
