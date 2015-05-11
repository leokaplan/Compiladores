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

/* Marcacoes para as unions */
typedef enum {
	LIT_INT,
	LIT_FLOAT,
	LIT_STRING,
	
	ID,
	
	TYP,

	VAR_SIMPLE,
	VAR_ARRAY,
	
	EXP_BINOP,
	EXP_UNOP,
	EXP_NEW,
	EXP_CALL,
	
	DEC_VAR,
	DEC_FUNC,

	CMD_IF,
	CMD_WHILE,
	CMD_ATTR

} AST_unionTag;

/* Tipos */
typedef struct {
	AST_typeEnum type;
	/* Numero de ponteiros */
	int indirections;
} AST_typNodeType;

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

/* Variaveis */
typedef union {
	AST_nodeType * id;
	/* Array */
	struct {
		AST_nodeType * exp1;
		AST_nodeType * exp2;
	} indexed;
} AST_varNodeType;

/* Expressoes */
typedef union {
	struct {
		int opr;
		AST_nodeType * exp1;
		AST_nodeType * exp2;
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
		AST_nodeType * exp;
		AST_nodeType * cmd1;
		AST_nodeType * cmd2;
	} ifcmd;

	struct {
		AST_nodeType * exp;
		AST_nodeType * cmd;
	} whilecmd;

	struct {
		AST_nodeType * var;
		AST_nodeType * exp;
	} attrcmd;

	struct {
		AST_nodeType * exp;
	} retcmd;



} AST_cmdNodeType;

typedef struct nodeTypeTag {
	AST_nodeEnum type;
	AST_unionTag tag;
	int line;

	/* Utilizados para listas */
	AST_nodeType * nextElem;
	AST_nodeType * lastElem;		

	union {
		AST_litNodeType lit;
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
AST_nodeType * AST_exp_opr(int oper, AST_nodeType * exp1, AST_nodeType * exp2);



AST_nodeType * AST_var(...);

AST_nodeType * AST_decl_var(...);
AST_nodeType * AST_decl_func(...);

/* Comando if (com ou sem else) */
AST_nodeType * AST_cmd_if(AST_nodeType * exp, AST_nodeType * cmd1, AST_nodeType * cmd2);

/* Comando while */
AST_nodeType * AST_cmd_while(AST_nodeType * exp, AST_nodeType * cmd);

/* Comando de atribuição */
AST_nodeType * AST_cmd_attr(AST_nodeType * var, AST_nodeType * exp);

/* Comando de retorno (com ou sem valor) */
AST_nodeType * AST_cmd_ret(AST_nodeType * exp);

/* Comando de chamada */
AST_nodeType * AST_cmd_call(char * name, ...);

/* Comando de new array */
AST_nodeType * AST_cmd_new(AST_nodeType * type, AST_nodeType * exp);

/* Liberaçao de memória */
void AST_freeNode(AST_nodeType * p);

int AST_ex(AST_nodeType * p);

/* Lida com listas de não terminais */
AST_nodeType * AST_handleList(AST_nodeType * list, AST_nodeType * element);

#endif
