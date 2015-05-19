#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "abstractsyntaxtree.h"
#include "monga.tab.h"

#define MAKE_NODE(p,TYPE,TAG); \
	if ((p = (AST_nodeType *) malloc(sizeof(AST_nodeType))) == NULL) \
		yyerror("Falta de memoria"); \
	p->nextElem = NULL; \
	p->lastElem = p; \
	p->type = TYPE; \
	p->tag = TAG; 

/* Tipos */
struct AST_typNodeType {
	AST_typeEnum type;
	/* Numero de ponteiros */
	int indirections;
};

/* Literais */
union AST_litNodeType {
	int ivalue;
	float fvalue;
	char * svalue;
};

/* Identificadores */
struct AST_idNodeType {
	char * name;
};

/* Variaveis */
union AST_varNodeType {
	AST_nodeType * id;
	/* Array */
	struct {
		AST_nodeType * exp1;
		AST_nodeType * exp2;
	} indexed;
};

/* Expressoes */
union AST_expNodeType {
	
	AST_nodeType * varexp;

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
		AST_nodeType * type;
		AST_nodeType * exp;
	} newexp;
};

/* Declaracoes */
union AST_declNodeType {
	struct {
		AST_nodeType * type;
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
		AST_litNodeType lit;
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
        MAKE_NODE(p,TYPE_LIT,LIT_INT);
	p->node.lit.ivalue = value;
	return p;
}

AST_nodeType * AST_litFloat(float value) {
	AST_nodeType * p;
        MAKE_NODE(p,TYPE_LIT,LIT_FLOAT);
	p->node.lit.fvalue = value;
	return p;
}

AST_nodeType * AST_litString(char * value) {
	AST_nodeType * p;
        MAKE_NODE(p,TYPE_LIT,LIT_STRING);
	p->node.lit.svalue = strdup(value);
	return p;
}

AST_nodeType * AST_id(char * name) {
	AST_nodeType * p;
    MAKE_NODE(p,TYPE_ID,ID);
	p->node.id.name = strdup(name);
	return p;
}

AST_nodeType * AST_type(AST_typeEnum type, int indirections) {
	AST_nodeType * p;
    MAKE_NODE(p,TYPE_TYP,TYP);

	p->node.typ.type = type;
	p->node.typ.indirections = indirections;

	return p;
}

AST_nodeType * AST_exp_opr(int oper, AST_nodeType * exp1, AST_nodeType * exp2) {
	AST_nodeType * p;
	if (exp2 == NULL)
        {
            MAKE_NODE(p,TYPE_EXP,EXP_UNOP);
        }
        else
        {
            MAKE_NODE(p,TYPE_EXP,EXP_BINOP);
        }
    p->node.exp.operexp.opr = oper;
	p->node.exp.operexp.exp1 = exp1;
	p->node.exp.operexp.exp2 = exp2;

	return p;
}
AST_nodeType * AST_exp_new(AST_nodeType * type, AST_nodeType * exp){
	AST_nodeType * p;
    MAKE_NODE(p,TYPE_EXP,EXP_NEW);
	
    p->node.exp.newexp.type = type;
	p->node.exp.newexp.exp = exp;

        return p;

}
AST_nodeType * AST_exp_var(AST_nodeType * var){
	AST_nodeType * p;

    MAKE_NODE(p,TYPE_EXP,EXP_VAR);
	p->node.exp.varexp = var;

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
AST_nodeType * AST_var_simple(AST_nodeType * id){
	AST_nodeType * p;

    MAKE_NODE(p,TYPE_VAR,VAR_SIMPLE);
	p->node.var.id = id;

	return p;
}
AST_nodeType * AST_decl_var(AST_nodeType * type, AST_nodeType * id){
	AST_nodeType * p;

    MAKE_NODE(p,TYPE_DECL,DEC_VAR);
	
	p->node.decl.vardecl.type = type;
	p->node.decl.vardecl.id = id;

	return p;
}
AST_nodeType * AST_decl_func(AST_nodeType * type, AST_nodeType * id, AST_nodeType * param, AST_nodeType * block){
	AST_nodeType * p;
    MAKE_NODE(p,TYPE_DECL,DEC_FUNC);

	p->node.decl.funcdecl.type = type;
	p->node.decl.funcdecl.id = id;
	p->node.decl.funcdecl.param = param;
	p->node.decl.funcdecl.block = block;

	return p;

}

AST_nodeType * AST_cmd_if(AST_nodeType * exp, AST_nodeType * cmd1, AST_nodeType * cmd2) {
	AST_nodeType * p;

    MAKE_NODE(p,TYPE_CMD,CMD_IF);
	p->node.cmd.ifcmd.exp = exp;
	p->node.cmd.ifcmd.cmd1 = cmd1;
	p->node.cmd.ifcmd.cmd2 = cmd2;	

	return p;
}

AST_nodeType * AST_cmd_while(AST_nodeType * exp, AST_nodeType * cmd) {
	AST_nodeType * p;

    MAKE_NODE(p,TYPE_CMD,CMD_WHILE);

	p->node.cmd.whilecmd.exp = exp;
	p->node.cmd.whilecmd.cmd = cmd;

	return p;
}

AST_nodeType * AST_cmd_attr(AST_nodeType * var, AST_nodeType * exp) {
	AST_nodeType * p;
    MAKE_NODE(p,TYPE_CMD,CMD_ATTR);

	p->node.cmd.attrcmd.var = var;
	p->node.cmd.attrcmd.exp = exp;

	return p;
}

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
AST_nodeType * AST_incInd(AST_nodeType * node){
	node->node.typ.indirections++;	
}
void newline(int ident){
    printf("\n");
    int spaces = ident;
    while(spaces--) printf("\t");
}
void GRAPH_drawNode(AST_nodeType *p,int ident){
    if (p != NULL) {
		switch(p->tag){
			case LIT_INT:
				printf("%d",p->node.lit.ivalue);
				break;
			case LIT_FLOAT:
				printf("%f",p->node.lit.fvalue);
				break;
			case LIT_STRING:
				printf("%s",p->node.lit.svalue);
				break;
			case ID: 
				printf("%s",p->node.id.name);
				break;
			case TYP:
				switch(p->node.typ.type){
					case INT:
						printf("int");
						break;
					case FLOAT:
						printf("float");
						break;
					case CHAR:
						printf("char");
						break;
					case VOID:
						printf("void");
						break;
				}
				int k;
				for(k=0;k<p->node.typ.indirections;k++){
					printf("[]");
				}
				break;
			case DEC_VAR:
				printf("");
                GRAPH_drawNode(p->node.decl.vardecl.type,ident);
				printf(" ");
                GRAPH_drawNode(p->node.decl.vardecl.id,ident);
				break;
			case DEC_FUNC:
				GRAPH_drawNode(p->node.decl.funcdecl.type,ident);
				printf(" ");
                GRAPH_drawNode(p->node.decl.funcdecl.id,ident);
                printf("(");
				GRAPH_drawNode(p->node.decl.funcdecl.param,ident);
                printf("){");
                newline(ident+1);
				GRAPH_drawNode(p->node.decl.funcdecl.block,ident+1);
                newline(ident);
                printf("}");
				break;
			case VAR_SIMPLE:
				GRAPH_drawNode(p->node.var.id,ident);
				break;
			case VAR_ARRAY:
				GRAPH_drawNode(p->node.var.indexed.exp1,ident);
				printf("[");
				GRAPH_drawNode(p->node.var.indexed.exp1,ident);
				printf("]");
				break;
			case EXP_BINOP:
				GRAPH_drawNode(p->node.exp.operexp.exp1,ident);
				switch(p->node.exp.operexp.opr){
					case TK_AND:
						printf(" && ");
						break;
					case TK_OR:
						printf(" || ");
						break;
					case TK_EQ:
						printf(" == ");
						break;
					case TK_NEQ:
						printf(" != ");
						break;
					case TK_LEQ:
						printf(" <= ");
						break;
					case TK_GEQ:
						printf(" >= ");
						break;
					default:
						printf(" %c ",p->node.exp.operexp.opr);
						break;
				}
				GRAPH_drawNode(p->node.exp.operexp.exp2,ident);
				break;
			case EXP_UNOP:
				printf("%c",p->node.exp.operexp.opr);
				GRAPH_drawNode(p->node.exp.operexp.exp1,ident);
				break;
			case EXP_NEW:
				printf("new ");
				GRAPH_drawNode(p->node.exp.newexp.type,ident);
				printf("[");
				GRAPH_drawNode(p->node.exp.newexp.exp,ident);
				printf("]");
				break;
			case EXP_CALL:
				GRAPH_drawNode(p->node.exp.callexp.exp1,ident);
				printf("(");
				GRAPH_drawNode(p->node.exp.callexp.exp2,ident);
				printf(")");
				break;
			case EXP_VAR:
				GRAPH_drawNode(p->node.exp.varexp,ident);
				break;
			case CMD_WHILE:
				printf("%p->%p [weight=4]",p,p->node.cmd.whilecmd.exp);
				GRAPH_drawNode(p->node.cmd.whilecmd.exp,ident);
				printf("%p->%p",p,p->node.cmd.whilecmd.cmd);
				GRAPH_drawNode(p->node.cmd.whilecmd.cmd,ident+1);
                printf("%p [label=\"while\"]",p);
				break;
			case CMD_IF:
				printf("%p->%p [weight=4]",p,p->node.cmd.ifcmd.exp);
				GRAPH_drawNode(p->node.cmd.ifcmd.exp,ident);
				printf("%p->%p",p,p->node.cmd.ifcmd.cmd1);
				GRAPH_drawNode(p->node.cmd.ifcmd.cmd1,ident+1);
				if(p->node.cmd.ifcmd.cmd2 != NULL){
				    printf("%p->%p",p,p->node.cmd.ifcmd.cmd2);
					GRAPH_drawNode(p->node.cmd.ifcmd.cmd2,ident+1);
				}
                printf("%p [label=\"if\"]",p);
				break;
			case CMD_ATTR:
				printf("%p->%p",p,p->node.cmd.attrcmd.var);
                GRAPH_drawNode(p->node.cmd.attrcmd.var,ident);
				printf("%p->%p",p,p->node.cmd.attrcmd.exp);
				GRAPH_drawNode(p->node.cmd.attrcmd.exp,ident);
                printf("%p [label=\"attr\"]",p);
				break;
			case CMD_EXP:
				GRAPH_drawNode(p->node.cmd.expcmd.exp,ident);
				break;
			case CMD_BLOCK:
				GRAPH_drawNode(p->node.cmd.blockcmd.decl,ident);
				GRAPH_drawNode(p->node.cmd.blockcmd.cmd,ident);
				break;
			case CMD_RET:
				printf("return");
				if(p->node.cmd.retcmd.exp != NULL){
					printf(" ");
                    GRAPH_drawNode(p->node.cmd.retcmd.exp,ident);
				}
                printf(";");
                newline(ident);
				break;
		}
		if(p->nextElem != NULL){
            switch(p->tag){
                case ID:
                    printf(",");
                    break;
                default:
                    break;
            }
            GRAPH_drawNode(p->nextElem,ident);
        }
        else{
            switch(p->tag){
                case DEC_VAR:
                    printf(";");
                    newline(ident);
                    break;
            }
        }
	}
}
void CODE_drawNode(AST_nodeType *p,int ident){
    if (p != NULL) {
		switch(p->tag){
			case LIT_INT:
				printf("%d",p->node.lit.ivalue);
				break;
			case LIT_FLOAT:
				printf("%f",p->node.lit.fvalue);
				break;
			case LIT_STRING:
				printf("%s",p->node.lit.svalue);
				break;
			case ID: 
				printf("%s",p->node.id.name);
				break;
			case TYP:
				switch(p->node.typ.type){
					case INT:
						printf("int");
						break;
					case FLOAT:
						printf("float");
						break;
					case CHAR:
						printf("char");
						break;
					case VOID:
						printf("void");
						break;
				}
				int k;
				for(k=0;k<p->node.typ.indirections;k++){
					printf("[]");
				}
				break;
			case DEC_VAR:
				CODE_drawNode(p->node.decl.vardecl.type,ident);
				printf(" ");
                CODE_drawNode(p->node.decl.vardecl.id,ident);
				break;
			case DEC_FUNC:
				CODE_drawNode(p->node.decl.funcdecl.type,ident);
				printf(" ");
                CODE_drawNode(p->node.decl.funcdecl.id,ident);
                printf("(");
				CODE_drawNode(p->node.decl.funcdecl.param,ident);
                printf("){");
                newline(ident+1);
				CODE_drawNode(p->node.decl.funcdecl.block,ident+1);
                newline(ident);
                printf("}");
				break;
			case VAR_SIMPLE:
				CODE_drawNode(p->node.var.id,ident);
				break;
			case VAR_ARRAY:
				CODE_drawNode(p->node.var.indexed.exp1,ident);
				printf("[");
				CODE_drawNode(p->node.var.indexed.exp1,ident);
				printf("]");
				break;
			case EXP_BINOP:
				CODE_drawNode(p->node.exp.operexp.exp1,ident);
				switch(p->node.exp.operexp.opr){
					case TK_AND:
						printf(" && ");
						break;
					case TK_OR:
						printf(" || ");
						break;
					case TK_EQ:
						printf(" == ");
						break;
					case TK_NEQ:
						printf(" != ");
						break;
					case TK_LEQ:
						printf(" <= ");
						break;
					case TK_GEQ:
						printf(" >= ");
						break;
					default:
						printf(" %c ",p->node.exp.operexp.opr);
						break;
				}
				CODE_drawNode(p->node.exp.operexp.exp2,ident);
				break;
			case EXP_UNOP:
				printf("%c",p->node.exp.operexp.opr);
				CODE_drawNode(p->node.exp.operexp.exp1,ident);
				break;
			case EXP_NEW:
				printf("new ");
				CODE_drawNode(p->node.exp.newexp.type,ident);
				printf("[");
				CODE_drawNode(p->node.exp.newexp.exp,ident);
				printf("]");
				break;
			case EXP_CALL:
				CODE_drawNode(p->node.exp.callexp.exp1,ident);
				printf("(");
				CODE_drawNode(p->node.exp.callexp.exp2,ident);
				printf(")");
				break;
			case EXP_VAR:
				CODE_drawNode(p->node.exp.varexp,ident);
				break;
			case CMD_WHILE:
				printf("while(");
				CODE_drawNode(p->node.cmd.whilecmd.exp,ident);
				printf("){");
                newline(ident+1);
				CODE_drawNode(p->node.cmd.whilecmd.cmd,ident+1);
                newline(ident);
				printf("}");
                newline(ident);
				break;
			case CMD_IF:
				printf("if(");
				CODE_drawNode(p->node.cmd.ifcmd.exp,ident);
				printf("){");
                newline(ident+1);
				CODE_drawNode(p->node.cmd.ifcmd.cmd1,ident+1);
                newline(ident);
				printf("}");
				if(p->node.cmd.ifcmd.cmd2 != NULL){
					printf("else{");
                    newline(ident+1);
					CODE_drawNode(p->node.cmd.ifcmd.cmd2,ident+1);
                    newline(ident);
					printf("}");
				}
                newline(ident);
				break;
			case CMD_ATTR:
				CODE_drawNode(p->node.cmd.attrcmd.var,ident);
				printf(" = ");
				CODE_drawNode(p->node.cmd.attrcmd.exp,ident);
                printf(";");
                newline(ident);
				break;
			case CMD_EXP:
				CODE_drawNode(p->node.cmd.expcmd.exp,ident);
				break;
			case CMD_BLOCK:
				CODE_drawNode(p->node.cmd.blockcmd.decl,ident);
				CODE_drawNode(p->node.cmd.blockcmd.cmd,ident);
				break;
			case CMD_RET:
				printf("return");
				if(p->node.cmd.retcmd.exp != NULL){
					printf(" ");
                    CODE_drawNode(p->node.cmd.retcmd.exp,ident);
				}
                printf(";");
                newline(ident);
				break;
		}
		if(p->nextElem != NULL){
            switch(p->tag){
                case ID:
                    printf(",");
                    break;
                default:
                    break;
            }
            CODE_drawNode(p->nextElem,ident);
        }
        else{
            switch(p->tag){
                case DEC_VAR:
                    printf(";");
                    newline(ident);
                    break;
            }
        }
	}
}
void AST_draw(AST_nodeType *p,int mode){
	if(mode == 1) CODE_drawNode(p,0);
	if(mode == 2) GRAPH_drawNode(p,0);
}
