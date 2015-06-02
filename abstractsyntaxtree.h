/*
   abstractsyntaxtree.h
   Bernardo Alkmim - 1210514
   Leonardo Kaplan - 1212509
 */

#ifndef ABSTRACTSYNTAXTREE_H
#define ABSTRACTSYNTAXTREE_H


// TODO Mudar depois para o trabalho 4. Tabela de símbolos de mais de um caracter
extern int sym[26];



/* Tipos da linguagem */
enum AST_typeEnum {
	CHAR,
	INT,
	FLOAT,
	VOID,
    BOOL,
    NUM_BASETYPES 
};
int size_types = NUM_BASETYPES;
void ** types;

#define NUMUNOP 2
#define NUMBOP 11
int binop_key(int op){
    switch(op){
        case '+':     return 0;
        case '-':     return 1;
        case '/':     return 2;
        case '*':     return 3;
        case '%':     return 4;
        case '<':     return 5;
        case '>':     return 6;
        case TK_EQ:   return 7;
        case TK_NEQ:  return 8;
        case TK_LEQ:  return 9;
        case TK_GEQ:  return 10;
    }
}
int unop_key(int op){
    switch(op){
        case '!': return 0;
        case '-': return 1;
    }
}_
int op_bool_type[NUMUNOP] = {BOOL,BOOL};
int op_arithm_left[NUMBOP] = {INT,INT,FLOAT,FLOAT,INT,};
int op_arithm_right[NUMBOP];
int op_arithm_result[NUMBOP][NUM_BASETYPES][NUM_BASETYPES];



/* Tipos dos nos da ast */
enum AST_nodeEnum {
	TYPE_ID,
	TYPE_TYP,
	TYPE_EXP,
	TYPE_DECL,
	TYPE_CMD
};

/* Marcacoes para as unions */
enum AST_unionTag {
	
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
	
	DEC_VAR,
	DEC_FUNC,

	CMD_IF,
	CMD_WHILE,
	CMD_ATTR,
	CMD_EXP,
	CMD_BLOCK,
	CMD_RET

};

typedef enum AST_typeEnum AST_typeEnum;
typedef enum AST_nodeEnum AST_nodeEnum;
typedef enum AST_unionTag AST_unionTag;

typedef struct AST_typNodeType AST_typNodeType;
typedef struct AST_idNodeType AST_idNodeType;

typedef union AST_expNodeType AST_expNodeType;
typedef union AST_varNodeType AST_varNodeType;
typedef union AST_litNodeType AST_litNodeType;
typedef union AST_declNodeType AST_declNodeType;
typedef union AST_cmdNodeType AST_cmdNodeType;

typedef struct AST_nodeType AST_nodeType;

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
AST_nodeType * AST_exp_new(AST_nodeType * type, AST_nodeType * exp);
AST_nodeType * AST_exp_var(AST_nodeType * var);
AST_nodeType * AST_exp_call(AST_nodeType * exp1, AST_nodeType * exp2);

/* Var */
AST_nodeType * AST_var_array(AST_nodeType * exp1, AST_nodeType * exp2);
AST_nodeType * AST_var_simple(AST_nodeType * id);


AST_nodeType * AST_decl_var(AST_nodeType * type, AST_nodeType * id);
AST_nodeType * AST_decl_func(AST_nodeType * type, AST_nodeType * id, AST_nodeType * param, AST_nodeType * block);

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
