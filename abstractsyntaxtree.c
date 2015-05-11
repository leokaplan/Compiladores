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
AST_nodeType * AST_exp_new(AST_nodeType * type, AST_nodeType * exp){
	AST_nodeType * p;

	if ((p = (AST_nodeType *) malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");
	
	p->type = TYPE_EXP;
	p->tag = EXP_NEW;
	p->nextElem = NULL;
	p->lastElem = p;
	p->node.exp.newexp.type = type;
	p->node.exp.newexp.exp = exp;

	return p;

}
AST_nodeType * AST_exp_var(AST_nodeType * var){
	AST_nodeType * p;

	if ((p = (AST_nodeType *) malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");
	
	p->type = TYPE_EXP;
	p->tag = EXP_VAR;
	p->nextElem = NULL;
	p->lastElem = p;
	p->node.exp.varexp.var = var;

	return p;

}
AST_nodeType * AST_exp_call(AST_nodeType * id, AST_nodeType * exp){
	AST_nodeType * p;

	if ((p = (AST_nodeType *) malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");
	
	p->type = TYPE_EXP;
	p->tag = EXP_CALL;
	p->nextElem = NULL;
	p->lastElem = p;
	p->node.exp.callexp.id = id;
	p->node.exp.callexp.exp = exp;

	return p;


}
AST_nodeType * AST_var_array(AST_nodeType * exp1, AST_nodeType * exp2){
	AST_nodeType * p;

	if ((p = (AST_nodeType *) malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");
	
	p->type = TYPE_VAR;
	p->tag = VAR_ARRAY;
	p->nextElem = NULL;
	p->lastElem = p;
	p->node.var.exp1 = exp1;
	p->node.var.exp2 = exp2;

	return p;

}
AST_nodeType * AST_var_simple(AST_nodeType * id){
	AST_nodeType * p;

	if ((p = (AST_nodeType *) malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");
	
	p->type = TYPE_VAR;
	p->tag = VAR_SIMPLE;
	p->nextElem = NULL;
	p->lastElem = p;
	p->node.var.id = id;

	return p;
}
AST_nodeType * AST_decl_var(AST_nodeType * type, AST_nodeType * id){
	AST_nodeType * p;

	if ((p = (AST_nodeType *) malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");
	
	p->type = TYPE_DECL;
	p->tag = DEC_VAR;
	p->nextElem = NULL;
	p->lastElem = p;
	p->node.vardecl.type = type;
	p->node.vardecl.id = id;

	return p;
}
AST_nodeType * AST_decl_func(AST_nodeType * type, AST_nodeType * id, AST_nodeType * param, AST_nodeType * block){
	AST_nodeType * p;

	if ((p = (AST_nodeType *) malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");
	
	p->type = TYPE_DECL;
	p->tag = DEC_FUNC;
	p->nextElem = NULL;
	p->lastElem = p;
	p->node.vardecl.type = type;
	p->node.vardecl.id = id;
	p->node.vardecl.param = param;
	p->node.vardecl.block = block;

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

AST_nodeType * AST_cmd_attr(AST_nodeType * var, AST_nodeType * exp) {
	AST_nodeType * p;

	if ((p = (AST_nodeType *) malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");

	p->type = TYPE_CMD;
	p->tag = CMD_ATTR;
	p->nextElem = NULL;
	p->lastElem = p;
	p->node.cmd.attrcmd.var = var;
	p->node.cmd.attrcmd.exp = exp;

	return p;
}

AST_nodeType * AST_cmd_ret(AST_nodeType * exp) {
	AST_nodeType * p;

	if ((p = (AST_nodeType *) malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");

	p->type = TYPE_CMD;
	p->tag = CMD_RET;
	p->nextElem = NULL;
	p->lastElem = p;
	p->node.cmd.retcmd.exp = exp;

	return p;
}
AST_nodeType * AST_cmd_exp(AST_nodeType * exp) {
	AST_nodeType * p;

	if ((p = (AST_nodeType *) malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");

	p->type = TYPE_CMD;
	p->tag = CMD_EXP;
	p->nextElem = NULL;
	p->lastElem = p;
	p->node.cmd.expcmd.exp = exp;

	return p;
}
AST_nodeType * AST_cmd_block(AST_nodeType * decl, AST_nodeType * exp){
	AST_nodeType * p;

	if ((p = (AST_nodeType *) malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");

	p->type = TYPE_CMD;
	p->tag = CMD_BLOCK;
	p->nextElem = NULL;
	p->lastElem = p;
	p->node.cmd.blockcmd.decl = decl;
	p->node.cmd.blockcmd.exp = exp;

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
		case VAR_SIMPLE:
			break;
		case VAR_ARRAY:
			drawNode(p->node.exp.operexp.exp1);
			printf("[");
			drawNode(p->node.exp.operexp.exp1);
			printf("]");
			break;
		case EXP_BINOP:
			drawNode(p->node.exp.operexp.exp1);
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
			drawNode(p->node.exp.operexp.exp2);
			break;
		case EXP_UNOP:
			printf(" %c ",p->node.exp.operexp.opr);
			drawNode(p->node.exp.operexp.exp2);
			break;
		case EXP_NEW:
			printf("new ");
			drawNode(p->node.exp.newexp.type);
			printf("[");
			drawNode(p->node.exp.newexp.exp);
			printf("]");
			break;
		case EXP_CALL:
			break;
		case CMD_WHILE:
			printf("while(");
			drawNode(p->node.cmd.whilecmd.exp);
			printf("){");

			drawNode(p->node.cmd.whilecmd.cmd);
			printf("}");
			break;
		case CMD_IF:
			printf("if(");
			drawNode(p->node.cmd.ifcmd.exp);
			printf("){");
			drawNode(p->node.cmd.ifcmd.cmd1);
			printf("}");
			if(p->node.cmd.ifcmd.cmd2 != NULL){
				printf("else{");
				drawNode(p->node.cmd.ifcmd.cmd2);
				printf("}");
			}
			break;
		case CMD_ATTR:
			drawNode(p->node.cmd.attrcmd.var);
			printf("=");
			drawNode(p->node.cmd.attrcmd.exp);
			break;
		case CMD_RET:
			printf("return");
			if(p->node.cmd.retcmd.exp != NULL){
				drawNode(p->node.cmd.retcmd.exp);
			}
			break;
	}

}
void draw(nodeType *p){
	drawNode(p);
}
