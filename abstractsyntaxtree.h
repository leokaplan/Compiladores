/*
   abstractsyntaxtree.h
   Bernardo Alkmim - 1210514
   Leonardo Kaplan - 1212509
 */

#ifndef ABSTRACTSYNTAXTREE_H
	#define ABSTRACTSYNTAXTREE_H

/* Tipos da linguagem */
typedef enum {
	CHAR,
	INT,
	FLOAT,
	VOID
} AST_typeEnum;

/* Tipos dos nos da ast */
typedef enum {
	TYPE_ID,
	TYPE_EXP,
	TYPE_VAR,
	TYPE_DECL,
	TYPE_CMD
} AST_nodeEnum;

/* Expressoes */
typedef struct {
	int oper;
	int nops;
	AST_nodeType ** op; /* Lista de operandos */
} AST_expNodeType;

/* Variaveis */
typedef struct {
	
} AST_varNodeType;

/* Declaracoes */
typedef struct {

} AST_declNodeType;

/* Comandos */
typedef struct {

} AST_cmdNodeType;


typedef struct nodeTypeTag {
	AST_nodeEnum type;
	int line;
	union {
		AST_litIntNodeType litInt;
		AST_litFloatNodeType litFloat;
		AST_idNodeType id;
		AST_expNodeType exp;
		AST_varNodeType var;
		AST_declNodeType decl;
		AST_cmdNodeType cmd;
	};
} AST_nodeType;

// TODO Mudar depois para o trabalho 4. Tabela de símbolos de mais de um caracter
extern int sym[26];

/*  */
AST_nodeType * AST_litInt(int value);
AST_nodeType * AST_litFloat(float value);
AST_nodeType * AST_litString(char * value);
AST_nodeType * AST_id(char * name);

AST_nodeType * AST_exp(...);

AST_nodeType * AST_var(...);

AST_nodeType * AST_decl_var(...);
AST_nodeType * AST_decl_func(...);

AST_nodeType * AST_cmd_if(...);
AST_nodeType * AST_cmd_while(...);
AST_nodeType * AST_cmd_attr(...);
AST_nodeType * AST_cmd_ret(...);
AST_nodeType * AST_cmd_call(...);

void AST_freeNode(AST_nodeType *p);
int AST_ex(AST_nodeType *p);

#endif
