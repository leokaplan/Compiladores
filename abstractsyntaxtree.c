#include "abstractsyntaxtree.h"
#include <stdlib.h>
#define SIZEOF_NODETYPE ((char *)&p->con - (char *)p)
nodeType *con(int value) {
 nodeType *p;
 /* allocate node */
 if ((p = malloc(sizeof(nodeType))) == NULL)
 yyerror("out of memory");
 /* copy information */
 p->type = typeCon;
 p->con.value = value;
 return p;
}
nodeType *id(int i) {
 nodeType *p;
 /* allocate node */
 if ((p = malloc(sizeof(nodeType))) == NULL)
 yyerror("out of memory");
 /* copy information */
 p->type = typeId;
 p->id.i = i;
 return p;
}
nodeType *opr(int oper, int nops, ...) {
 va_list ap;
 nodeType *p;
 int i;
 /* allocate node */
 if ((p = malloc(sizeof(nodeType))) == NULL)
 yyerror("out of memory");
 if ((p->opr.op = malloc(nops * sizeof(nodeType *))) == NULL)
 yyerror("out of memory");
 /* copy information */
 p->type = typeOpr;
 p->opr.oper = oper;
 p->opr.nops = nops;
 va_start(ap, nops);
 for (i = 0; i < nops; i++)
 p->opr.op[i] = va_arg(ap, nodeType*);
 va_end(ap);
 return p;
}
void freeNode(nodeType *p) {
 int i;
 if (!p) return;
 if (p->type == typeOpr) {
 for (i = 0; i < p->opr.nops; i++)
 freeNode(p->opr.op[i]);
 free(p->opr.op);
 }
 free (p);
}
