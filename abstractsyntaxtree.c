#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "abstractsyntaxtree.h"
#include "monga.tab.h"
#include "decls.c"
#include "types.c"
#define MAKE_NODE(p,TYPE,TAG); \
	if ((p = (AST_nodeType *) malloc(sizeof(AST_nodeType))) == NULL) \
		yyerror("Falta de memoria"); \
	p->nextElem = NULL; \
	p->lastElem = p; \
	p->type = TYPE; \
	p->tag = TAG; 

/* Tipos */
struct AST_typNodeType {
	int size;
};

/* Identificadores */
struct AST_idNodeType {
	char * name;
};
struct AST_varNodeType {
    int type;
    AST_nodeType * id;
    AST_nodeType * index;
};

struct AST_expNodeType {
/* Expressoes */
    int type;
	union {
    
    
        AST_nodeType * varexp;
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
union AST_declNodeType {
	struct {
		int type;
		AST_nodeType * id;
	} vardecl;
	struct {
		AST_nodeType * type;
		AST_nodeType * id;
		AST_nodeType * param;
		AST_nodeType * block;
	} funcdecl;
};

/* Comandos */
union AST_cmdNodeType {
	
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

struct AST_nodeType {
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


AST_nodeType * AST_litInt(int value) {
	AST_nodeType * p;
        MAKE_NODE(p,TYPE_EXP,LIT_INT);
	p->node.exp.content.lit.ivalue = value;
	p->node.exp.type = INT;
	return p;
}

AST_nodeType * AST_litFloat(float value) {
	AST_nodeType * p;
        MAKE_NODE(p,TYPE_EXP,LIT_FLOAT);
	p->node.exp.content.lit.fvalue = value;
	p->node.exp.type = FLOAT;
	return p;
}

AST_nodeType * AST_litString(char * value) {
	AST_nodeType * p;
        MAKE_NODE(p,TYPE_EXP,LIT_STRING);
	p->node.exp.content.lit.svalue = strdup(value);
	p->node.exp.type = array(CHAR);
	return p;
}

AST_nodeType * AST_id(char * name) {
	AST_nodeType * p;
    MAKE_NODE(p,TYPE_ID,ID);
	p->node.id.name = strdup(name);
	return p;
}


AST_nodeType * AST_exp_opr(int oper, AST_nodeType * exp1, AST_nodeType * exp2) {
	AST_nodeType * p;
    int op = binop_key(oper);
	if (exp2 == NULL)
        {
            MAKE_NODE(p,TYPE_EXP,EXP_UNOP);
	        if(op_bool_type[op]!=exp1->node.exp.type) 
                error("type");
            p->node.exp.type = exp1->node.exp.type;
        }
        else
        {
            MAKE_NODE(p,TYPE_EXP,EXP_BINOP);
	        if(op_arithm_left[op] != exp1->node.exp.type && op_arithm_right[op] != exp2->node.exp.type) 
                error("type");
            p->node.exp.type = op_arithm_result[op][exp1->node.exp.type][exp2->node.exp.type];
        }
    p->node.exp.content.operexp.opr = oper;
    p->node.exp.content.operexp.exp1 = exp1;
	p->node.exp.content.operexp.exp2 = exp2;

	return p;
}
AST_nodeType * AST_exp_new(int type, AST_nodeType * exp){
	AST_nodeType * p;
    MAKE_NODE(p,TYPE_EXP,EXP_NEW);
	
    p->node.exp.type = type;
	if(exp->node.exp.type != INT) 
        error("type");
    p->node.exp.content.newexp.exp = exp;

        return p;

}
AST_nodeType * AST_exp_var(AST_nodeType * var){
	AST_nodeType * p;

    MAKE_NODE(p,TYPE_EXP,EXP_VAR);
	p->node.exp.content.varexp = var;
	p->node.exp.type = var->node.var.type;

	return p;

}
AST_nodeType * AST_exp_call(AST_nodeType * exp1, AST_nodeType * exp2){
	AST_nodeType * p;

    MAKE_NODE(p,TYPE_EXP,EXP_CALL);
	p->node.exp.callexp.exp1 = exp1;
	p->node.exp.callexp.exp2 = exp2;

	return p;


}
AST_nodeType * AST_var_array(AST_nodeType * exp1, AST_nodeType * exp2){
	AST_nodeType * p;

    MAKE_NODE(p,TYPE_VAR,VAR_ARRAY);
	p->node.var.indexed.exp1 = exp1;
	p->node.var.indexed.exp2 = exp2;

	return p;

}
//TODO botar tipo
AST_nodeType * AST_var_simple(AST_nodeType * id){
	AST_nodeType * p;

    MAKE_NODE(p,TYPE_VAR,VAR_SIMPLE);
	p->node.exp.var.id = id;

	return p;
}
AST_nodeType * AST_decl_var(int type, AST_nodeType * id){
	AST_nodeType * p;

    MAKE_NODE(p,TYPE_DECL,DEC_VAR);
	
	p->node.decl.vardecl.type = type;
	p->node.decl.vardecl.id = id;
    new_decl(type,id);
	return p;
}
//TODO mudar escopo
AST_nodeType * AST_decl_func(int type, AST_nodeType * id, AST_nodeType * param, AST_nodeType * block){
	AST_nodeType * p;
    MAKE_NODE(p,TYPE_DECL,DEC_FUNC);

	p->node.decl.funcdecl.type = type;
	p->node.decl.funcdecl.id = id;
	p->node.decl.funcdecl.param = param;
	p->node.decl.funcdecl.block = block;

	return p;

}
//TODO tipo
AST_nodeType * AST_cmd_if(AST_nodeType * exp, AST_nodeType * cmd1, AST_nodeType * cmd2) {
	AST_nodeType * p;

    MAKE_NODE(p,TYPE_CMD,CMD_IF);
	p->node.cmd.ifcmd.exp = exp;
	p->node.cmd.ifcmd.cmd1 = cmd1;
	p->node.cmd.ifcmd.cmd2 = cmd2;	

	return p;
}
//TODO tipo
AST_nodeType * AST_cmd_while(AST_nodeType * exp, AST_nodeType * cmd) {
	AST_nodeType * p;

    MAKE_NODE(p,TYPE_CMD,CMD_WHILE);
	p->node.cmd.whilecmd.exp = exp;
	p->node.cmd.whilecmd.cmd = cmd;

	return p;
}
//TODO checar tipo
AST_nodeType * AST_cmd_attr(AST_nodeType * var, AST_nodeType * exp) {
	AST_nodeType * p;
    MAKE_NODE(p,TYPE_CMD,CMD_ATTR);
	p->node.cmd.attrcmd.var = var;
	p->node.cmd.attrcmd.exp = exp;

	return p;
}
//TODO tipo com retorno da funcao
AST_nodeType * AST_cmd_ret(AST_nodeType * exp) {
	AST_nodeType * p;
    
    MAKE_NODE(p,TYPE_CMD,CMD_RET);
    p->node.cmd.retcmd.exp = exp;
	return p;
}
AST_nodeType * AST_cmd_exp(AST_nodeType * exp) {
	AST_nodeType * p;


    MAKE_NODE(p,TYPE_CMD,CMD_EXP);
	p->node.cmd.expcmd.exp = exp;

	return p;
}
//TODO mudar escopo
AST_nodeType * AST_cmd_block(AST_nodeType * decl, AST_nodeType * cmd){
	AST_nodeType * p;
    MAKE_NODE(p,TYPE_CMD,CMD_BLOCK);

	p->node.cmd.blockcmd.decl = decl;
	p->node.cmd.blockcmd.cmd = cmd;
	return p;
}

AST_nodeType * AST_handleList(AST_nodeType * list, AST_nodeType * element) {
	AST_nodeType * ret = NULL;
	if (list == NULL) {
		ret = element;
	}
	else {
		list->lastElem->nextElem = element;
		list->lastElem = element->lastElem;
		ret = list;
	}
	return ret;
}
void newline(int ident){
    printf("\n");
    int spaces = ident;
    while(spaces--) printf("\t");
}
