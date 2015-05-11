#include "abstractsyntaxtree.h"
#include <stdlib.h>
#include <string.h>

#define SIZEOF_NODETYPE ((char *)&p->lit - (char *)p)

/*
   O nó é um literal int.
 */
AST_nodeType * AST_litInt(int value) {
	AST_nodeType *p;

	if ((p = malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");

	p->type = TYPE_LIT;
	p->lit.ivalue = value;
	return p;
}

/*
   O nó é um literal float.
 */
AST_nodeType * AST_litFloat(float value) {
	AST_nodeType *p;

	if ((p = malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");

	p->type = TYPE_LIT;
	p->lit.fvalue = value;
	return p;
}

/*
   O nó é um literal string.
 */
AST_nodeType * AST_litString(char * value) {
	AST_nodeType *p;

	if ((p = malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");

	p->type = TYPE_LIT;
	strcmp(p->lit.svalue, value);
	return p;
}

/*
   O nó é um identificador
 */
AST_nodeType * AST_id(char * name) {
	AST_nodeType *p;
	/* allocate node */
	if ((p = malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");
	/* copy information */
	p->type = TYPE_ID;
	strcmp(p->id.name, name);
	return p;
}

AST_nodeType * AST_opr(int oper, int nops, ...) {
	va_list ap;
	AST_nodeType *p;
	int i;

	if ((p = malloc(sizeof(AST_nodeType))) == NULL)
		yyerror("Falta de memoria");
	if ((p->opr.op = malloc(nops * sizeof(AST_nodeType *))) == NULL)
		yyerror("Falta de memoria");

	p->type = TYPE_OPR;
	p->opr.oper = oper;
	p->opr.nops = nops;
	va_start(ap, nops);
	for (i = 0; i < nops; i++) {
		p->opr.op[i] = va_arg(ap, AST_nodeType*);
	}
	va_end(ap);
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
