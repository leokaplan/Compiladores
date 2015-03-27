#define ERROR(...); fprintf(stderr,__VA_ARGS__);exit(0);

enum TOKEN {
    TK_CHAR = sizeof(char)+1,  
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
union {
    int intval;
    float floatval;
    char * stringval;
    char * name;
} yylval;

int currentLine = 1;

void cpy(char * dst, char * src, size_t len);
char * dupl(char * s);
char * escapeddupl(char * s);
char escape(char a,char b);
