/*
	abstractsyntaxtree.h
	Bernardo Alkmim - 1210514
	Leonardo Kaplan - 1212509
*/

typedef enum {
	TYPE_LIT,
	TYPE_ID,
	TYPE_OPR,
} nodeEnum;

/* Literais */
typedef union {
	int ivalue; 
	float fvalue;
	char * svalue;
} litNodeType;

/* Identificadores */
typedef struct {
	char * name;
} idNodeType;

/* operators */
typedef struct {
	int oper; /* operator */
	int nops; /* number of operands */
	struct nodeTypeTag ** op; /* operands */
} oprNodeType;

typedef struct nodeTypeTag {
	nodeEnum type; /* type of node */
	int line;
	union {
		litNodeType lit;
		idNodeType id;
		oprNodeType opr;
	};
} nodeType;

// TODO Mudar depois para o trabalho 4. Tabela de símbolos de mais de um caracter
extern int sym[26];

nodeType * opr(int oper, int nops, ...);
nodeType * id(char * name);
nodeType * litInt(int value);
nodeType * litFloat(float value);
nodeType * litString(char * value);
void freeNode(nodeType *p);
int ex(nodeType *p);
