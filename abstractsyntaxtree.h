/*
   abstractsyntaxtree.h
   Bernardo Alkmim - 1210514
   Leonardo Kaplan - 1212509
 */

#ifndef ABSTRACTSYNTAXTREE_H
#define ABSTRACTSYNTAXTREE_H
#include "monga.tab.h"

/* Tipos da linguagem */
enum typeEnum {
	CHAR,
	INT,
	FLOAT,
	VOID,
    BOOL,
    NUM_BASETYPES 
};

int size_types = NUM_BASETYPES;
void ** types;

/* Tipos dos nos da ast */
enum nodeEnum {
	TYPE_ID,
	TYPE_TYP,
	TYPE_EXP,
	TYPE_DECL,
	TYPE_VAR,
	TYPE_CMD
};

/* Marcacoes para as unions */
enum unionTag {
	
	ID,
	
	TYP,

	
	EXP_BINOP,
	EXP_UNOP,
	EXP_NEW,
	EXP_CALL,
	EXP_VAR,
	LIT_INT,
	LIT_FLOAT,
	LIT_STRING,
	VAR_SIMPLE,
	VAR_ARRAY,
	
	DEC_VAR,
	DEC_FUNC,

	CMD_IF,
	CMD_WHILE,
	CMD_ATTR,
	CMD_EXP,
	CMD_BLOCK,
	CMD_RET

};

typedef enum typeEnum AST_typeEnum;
typedef enum nodeEnum AST_nodeEnum;
typedef enum unionTag AST_unionTag;

typedef struct typNodeType AST_typNodeType;
typedef struct idNodeType AST_idNodeType;

typedef struct expNodeType AST_expNodeType;
typedef struct varNodeType AST_varNodeType;
typedef union litNodeType AST_litNodeType;
typedef union declNodeType AST_declNodeType;
typedef union cmdNodeType AST_cmdNodeType;

typedef struct nodeType AST_nodeType;

extern AST_nodeType * prog;


/*
 * mode = 1 print a formatted code
 * mode = 2 ptint in DOT
 * */
void AST_draw(AST_nodeType *p,int mode);

extern void yyerror(char *);

/* Literal */
AST_nodeType * AST_litInt(int value);
AST_nodeType * AST_litFloat(float value);
AST_nodeType * AST_litString(char * value);

/* Identificador */
AST_nodeType * AST_id(char * name);

/* Tipo da linguagem */
AST_nodeType * AST_type(AST_typeEnum type, int indirections);

/* Expressão */
AST_nodeType * AST_exp_opr(int oper, AST_nodeType * exp1, AST_nodeType * exp2);
AST_nodeType * AST_exp_new(int type, AST_nodeType * exp);
AST_nodeType * AST_exp_var(AST_nodeType * var);
AST_nodeType * AST_exp_call(AST_nodeType * exp1, AST_nodeType * exp2);

/* Var */
AST_nodeType * AST_var_array(AST_nodeType * exp1, AST_nodeType * exp2);
AST_nodeType * AST_var_simple(AST_nodeType * id);


AST_nodeType * AST_decl_var(int type, AST_nodeType * id);
AST_nodeType * AST_decl_func(int type, AST_nodeType * id, AST_nodeType * param, AST_nodeType * block);

/* Comando */
AST_nodeType * AST_cmd_if(AST_nodeType * exp, AST_nodeType * cmd1, AST_nodeType * cmd2);
AST_nodeType * AST_cmd_while(AST_nodeType * exp, AST_nodeType * cmd);
AST_nodeType * AST_cmd_attr(AST_nodeType * var, AST_nodeType * exp);
AST_nodeType * AST_cmd_ret(AST_nodeType * exp);
AST_nodeType * AST_cmd_exp(AST_nodeType * exp);
AST_nodeType * AST_cmd_block(AST_nodeType * decl, AST_nodeType * cmd);

/* Lida com listas de não terminais */
AST_nodeType * AST_handleList(AST_nodeType * list, AST_nodeType * element);
AST_nodeType * AST_incInd(AST_nodeType * node);

#endif
