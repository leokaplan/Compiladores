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
	TYPE_LIT,
	TYPE_ID,
	TYPE_TYP,
	TYPE_EXP,
	TYPE_VAR,
	TYPE_DECL,
	TYPE_CMD
} AST_nodeEnum;

/* Tipos */
typedef struct {
	AST_typeEnum type;
	/* Numero de ponteiros */
	int indirections;
} AST_typNodeType;

/* Variaveis */
typedef union {
	AST_NodeType * id;
	/* Array */
	struct {
		AST_NodeType * exp1;
		AST_NodeType * exp2;
	} indexed;
} AST_varNodeType;

/* Expressoes */
typedef union {
	struct {
		int opr;
		AST_NodeType * exp1;
		AST_NodeType * exp2;
	} operexp;

	

	/* New (para arrays) */
	struct {
		AST_nodeType * type;
		AST_nodeType * exp;
	} newexp;
} AST_expNodeType;


/* Declaracoes */
typedef struct {

} AST_declNodeType;

/* Comandos */
typedef union {
	
	struct {
		int token;
		AST_nodeType * exp;
		AST_nodeType * cmd1;
		AST_nodeType * cmd2;
	} ifcmd;

	struct {
		int token;
		AST_nodeType * exp;
		AST_nodeType * cmd;
	} whilecmd;

	struct {
		int token;
		AST_nodeType * var;
		AST_nodeType * exp;
	} attrcmd;

	struct {
		int token;
		AST_nodeType * exp;
	} retcmd;



} AST_cmdNodeType;


typedef struct nodeTypeTag {
	AST_nodeEnum type;
	int line;
	union {
		AST_litIntNodeType litInt;
		AST_litFloatNodeType litFloat;
		AST_litStringNodeType litString;
		AST_typNodeType typ;
		AST_idNodeType id;
		AST_expNodeType exp;
		AST_varNodeType var;
		AST_declNodeType decl;
		AST_cmdNodeType cmd;
	} node;
} AST_nodeType;

// TODO Mudar depois para o trabalho 4. Tabela de símbolos de mais de um caracter
extern int sym[26];

nodeType * opr(int oper, int nops, ...);
void freeNode(nodeType *p);
void draw(nodeType *p);

extern void yyerror(char *);

/* Literal int */
AST_nodeType * AST_litInt(int value);

/* Literal float */
AST_nodeType * AST_litFloat(float value);

/* Literal string */
AST_nodeType * AST_litString(char * value);

/* Identificador */
AST_nodeType * AST_id(char * name);

/* Tipo da linguagem */
AST_nodeType * AST_type(AST_typeEnum type, int indirections);

/* Expressão de operação (binaria ou unaria) */
AST_nodeType * AST_exp_opr(int oper, AST_NodeType * exp1, AST_NodeType * exp2);



AST_nodeType * AST_var(...);

AST_nodeType * AST_decl_var(...);
AST_nodeType * AST_decl_func(...);

/* Comando if (com ou sem else) */
AST_nodeType * AST_cmd_if(AST_NodeType * exp, AST_NodeType * cmd1, AST_NodeType * cmd2);

/* Comando while */
AST_nodeType * AST_cmd_while(AST_NodeType * exp, AST_NodeType * cmd);

/* Comando de atribuição */
AST_nodeType * AST_cmd_attr(AST_NodeType * var, AST_NodeType * exp);

/* Comando de retorno (com ou sem valor) */
AST_nodeType * AST_cmd_ret(AST_NodeType * exp);

/* Comando de chamada */
AST_nodeType * AST_cmd_call(char * name, ...);

/* Comando de new array */
AST_nodeType * AST_cmd_new(AST_NodeType * type, AST_NodeType * exp);

/* Liberaçao de memória */
void AST_freeNode(AST_nodeType * p);


int AST_ex(AST_nodeType * p);

#endif
