#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "monga.tab.h"
#include "abstractsyntaxtree.h"

#define SIZEOF_NODETYPE ((char *)&p->lit - (char *)p)

AST_nodeType * AST_litInt(int value) {
	AST_nodeType * p;

	if ((p = (AST_nodeType *) malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");

	p->type = TYPE_LIT;
	p->tag = LIT_INT;
	p->nextElem = NULL;
	p->lastElem = p;
	p->node.lit.ivalue = value;
	return p;
}

AST_nodeType * AST_litFloat(float value) {
	AST_nodeType * p;

	if ((p = (AST_nodeType *) malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");

	p->type = TYPE_LIT;
	p->tag = LIT_FLOAT;
	p->nextElem = NULL;
	p->lastElem = p;
	p->node.lit.fvalue = value;
	return p;
}

AST_nodeType * AST_litString(char * value) {
	AST_nodeType * p;

	if ((p = (AST_nodeType *) malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");

	p->type = TYPE_LIT;
	p->tag = LIT_STRING;
	p->nextElem = NULL;
	p->lastElem = p;
	strcmp(p->node.lit.svalue, value);
	return p;
}

AST_nodeType * AST_id(char * name) {
	AST_nodeType * p;
	
	if ((p = (AST_nodeType *) malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");
	
	p->type = TYPE_ID;
	p->tag = ID;
	p->nextElem = NULL;
	p->lastElem = p;
	strcmp(p->node.id.name, name);
	return p;
}

AST_nodeType * AST_type(AST_typeEnum type, int indirections) {
	AST_nodeType * p;

	if ((p = (AST_nodeType *) malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");

	p->type = TYPE_TYP;
	p->tag = TYP;
	p->nextElem = NULL;
	p->lastElem = p;
	p->node.typ.type = type;
	p->node.typ.indirections = indirections;
	
	return p;
}

AST_nodeType * AST_exp_opr(int oper, AST_nodeType * exp1, AST_nodeType * exp2) {
	AST_nodeType * p;

	if ((p = (AST_nodeType *) malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");

	p->type = TYPE_EXP;
	p->tag = (exp2 == NULL)? EXP_UNOP: EXP_BINOP;
	p->nextElem = NULL;
	p->lastElem = p;
	p->node.exp.oprexp.opr = oper;
	p->node.exp.oprexp.exp1 = exp1;
	p->node.exp.oprexp.exp2 = exp2;
	
	return p;
}

AST_nodeType * AST_cmd_if(AST_nodeType * exp, AST_nodeType * cmd1, AST_nodeType * cmd2) {
	AST_nodeType * p;
	
	if ((p = (AST_nodeType *) malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");

	p->type = TYPE_CMD;
	p->tag = CMD_IF;
	p->nextElem = NULL;
	p->lastElem = p;
	p->node.cmd.ifcmd.exp = exp;
	p->node.cmd.ifcmd.cmd1 = cmd1;
	p->node.cmd.ifcmd.cmd2 = cmd2;	

	return p;
}

AST_nodeType * AST_cmd_while(AST_nodeType * exp, AST_nodeType * cmd) {
	AST_nodeType * p;
	
	if ((p = (AST_nodeType *) malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");

	p->type = TYPE_CMD;
	p->tag = CMD_WHILE;
	p->nextElem = NULL;
	p->lastElem = p;
	p->node.cmd.whilecmd.exp = exp;
	p->node.cmd.whilecmd.cmd = cmd;

	return p;
}


void AST_freeNode(AST_nodeType *p) {
	int i;
	if (!p)
		return;

	if (p->type == TYPE_OPR) {
		for (i = 0; i < p->opr.nops; i++) {
			AST_freeNode(p->opr.op[i]);
		}
		free(p->opr.op);
	}
	free (p);
}

AST_nodeType * AST_handleList(AST_nodeType * list, AST_nodeType * element) {
	if (list == NULL) {
		return element;
	}
	else {
		list->lastElem->nextElem = element;
		list->lastElem = element->lastElem;
		return list;
	}
}

void drawNode(nodeType *p){
    switch(p->type){
        case TYPE_LIT: 
            printf("lit"); 
            break;
        case TYPE_ID: 
            printf("%s",p->id.name);
            break;
        case TYPE_OPR:
            switch(p->opr.oper){
                case TK_WHILE:
                    printf("while");
                    break;
                case TK_IF:
                    printf("if");
                    break;
                case '=':
                    printf("=");
                    break;
                case TK_RETURN:
                    printf("return");
                    break;
            }
            int k;
            for (k = 0; k < p->opr.nops; k++) {
             drawNode (p->opr.op[k]);
            }
            break;
    }

}
void draw(nodeType *p){
    drawNode(p);
}
