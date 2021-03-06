/*
   abstractsyntaxtree.h
   Bernardo Alkmim - 1210514
   Leonardo Kaplan - 1212509
 */

#ifndef ABSTRACTSYNTAXTREE_H
#define ABSTRACTSYNTAXTREE_H

/* Tipos da linguagem */
enum typeEnum {
	CHAR,
	INT,
	FLOAT,
	VOID,
    BOOL,
    NUM_BASETYPES 
};

//int size_types = NUM_BASETYPES;
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
    CAST,

    EXP_BINOP,
    EXP_UNOP,
    EXP_NEW,
    EXP_CALL,
    EXP_VAR,
    EXP_PAREN,

    LIT_INT,
    LIT_BOOL,
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

/* Tipos */
struct typNodeType {
	int size;
};

/* Identificadores */
struct idNodeType {
	char * name;
};
struct varNodeType {
    int type;
    AST_nodeType * id;
    AST_nodeType * index;
};

struct expNodeType {
/* Expressoes */
    int type;
    union {
        
        AST_nodeType * varexp;
        AST_nodeType * cast;
        AST_nodeType * parenexp;
        /* Literais */
        union {
            int ivalue;
            float fvalue;
            char * svalue;
        } lit;
     
        struct {
            int opr;
            AST_nodeType * exp1;
            AST_nodeType * exp2;
        } operexp;

        struct {
            AST_nodeType * exp1;
            AST_nodeType * exp2;
        } callexp;

        /* New (para arrays) */
        struct {
            int type;
            AST_nodeType * exp;
        } newexp;
    } content;
};

/* Declaracoes */
union declNodeType {
	struct {
		int type;
		AST_nodeType * id;
    } vardecl;
	struct {
		int type;
		AST_nodeType * id;
		AST_nodeType * param;
		AST_nodeType * block;
	} funcdecl;
};

/* Comandos */
union cmdNodeType {
	
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

	struct {
		AST_nodeType * decl;
		AST_nodeType * cmd;
	} blockcmd;
	
	struct {
		AST_nodeType * exp;
	} expcmd;
};

struct nodeType {
	AST_nodeEnum type;
	AST_unionTag tag;
	int line;

	/* Utilizados para listas */
	AST_nodeType * nextElem;
	AST_nodeType * lastElem;		

	union {
		AST_typNodeType typ;
		AST_idNodeType id;
		AST_expNodeType exp;
		AST_varNodeType var;
		AST_declNodeType decl;
		AST_cmdNodeType cmd;
	} node;
};

extern AST_nodeType * prog;

#include "monga.tab.h"

/*
 * mode = 1 print a formatted code
 * mode = 2 ptint in DOT
 * */
void AST_draw(AST_nodeType *p,int mode);


/* Literal */
AST_nodeType * AST_litInt(int value);
AST_nodeType * AST_litBool(int value);
AST_nodeType * AST_litFloat(float value);
AST_nodeType * AST_litString(char * value);

/* Identificador */
AST_nodeType * AST_id(char * name);

/* Tipo da linguagem */
AST_nodeType * AST_typecast(int type, AST_nodeType* exp);

/* Expressão */
AST_nodeType * AST_exp_opr(int oper, AST_nodeType * exp1, AST_nodeType * exp2);
AST_nodeType * AST_exp_new(int type, AST_nodeType * exp);
AST_nodeType * AST_exp_var(AST_nodeType * var);
AST_nodeType * AST_exp_call(AST_nodeType * exp1, AST_nodeType * exp2);
AST_nodeType * AST_exp_paren(AST_nodeType * exp);

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
