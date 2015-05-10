/*
   abstractsyntaxtree.h
   Bernardo Alkmim - 1210514
   Leonardo Kaplan - 1212509
 */

#ifndef ABSTRACTSYNTAXTREE
	#define ABSTRACTSYNTAXTREE

typedef enum {
	TYPE_LIT,
	TYPE_ID,
	TYPE_OPR,
} AST_nodeEnum;

/* Literais */
typedef union {
	int ivalue; 
	float fvalue;
	char * svalue;
} AST_litNodeType;

/* Identificadores */
typedef struct {
	char * name;
} AST_idNodeType;

/* Operadores */
typedef struct {
	int oper;
	int nops;
	struct AST_nodeType ** op; /* Lista de operandos */
} AST_oprNodeType;

typedef struct nodeTypeTag {
	AST_nodeEnum type;
	int line;
	union {
		AST_litNodeType lit;
		AST_idNodeType id;
		AST_oprNodeType opr;
	};
} AST_nodeType;

// TODO Mudar depois para o trabalho 4. Tabela de símbolos de mais de um caracter
extern int sym[26];

AST_nodeType * AST_opr(int oper, int nops, ...);
AST_nodeType * AST_id(char * name);
AST_nodeType * AST_litInt(int value);
AST_nodeType * AST_litFloat(float value);
AST_nodeType * AST_litString(char * value);
void AST_freeNode(AST_nodeType *p);
int AST_ex(AST_nodeType *p);

#endif
