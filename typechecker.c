#include <stdio.h>
#include <stdlib.h>
#include "abstractsyntaxtree.h"
#include "types.h"
#include "decls.h"
#define ERROR(...) printf(__VA_ARGS__);printf("\n\n");exit(0);
#define DEBUG(...) printf(__VA_ARGS__);
//retorna o tamanho da lista 
int size(AST_nodeType* list){
    if(list == NULL) return 0;
    int i;
    for(i=1;list->nextElem;i++)
        list = list->nextElem;
    return i;
}
//retorna um array ordenado de tipos correspondentes à lista de expressoes
//em caso de falta de memoria retorna NULL
int* unpack(AST_nodeType* list){
    if(list == NULL) return NULL;
    int s = size(list);
    int* ret = malloc(sizeof(int)*s);
    int i;
    for(i=0;list->nextElem;i++)
    {
        ret[i] = list->node.exp.type;
        list = list->nextElem;
    }
    return ret;
}

void checktypes(AST_nodeType *p){
    int type,opr,op,type1,type2;
    AST_nodeType* id;
    AST_nodeType* args;
    AST_nodeType* param;
    if (p != NULL) {
        switch(p->tag){
            case LIT_INT:
            case LIT_BOOL:
            case LIT_FLOAT:
            case LIT_STRING:
                DEBUG("lit: %s\n",type2string(p->node.exp.type));
                break;
            case DEC_VAR:
                type = p->node.decl.vardecl.type;
                id = p->node.decl.vardecl.id;
                new_var_decl(type,id);
                break;
            case DEC_FUNC:
                type = p->node.decl.funcdecl.type;
                id = p->node.decl.funcdecl.id;
                param = p->node.decl.funcdecl.param;
                //tem um push scope embutido
                new_func_decl(id,type,unpack(param),size(param));
                //declara as variaveis do cabeçalho como locais na função
                push_scope();
                checktypes(param);
                
                checktypes(p->node.decl.funcdecl.block);

                pop_scope();

                break;
            case VAR_SIMPLE:
            case VAR_ARRAY:
                DEBUG("var\n");
                type = check_var_decl(p->node.var.id);
                if(type == -1){
                    ERROR("undeclared variable");
                }
                else{
                    p->node.var.type = type; 
                }
                //with constant folding type could be checked
                break;
            case EXP_BINOP:
                opr = p->node.exp.content.operexp.opr;
                checktypes(p->node.exp.content.operexp.exp1);
                checktypes(p->node.exp.content.operexp.exp2);
                type1 = p->node.exp.content.operexp.exp1->node.exp.type;
                type2 = p->node.exp.content.operexp.exp2->node.exp.type;
                DEBUG("binop before: %s %s\n",type2string(type1),type2string(type2));
                int rtype = -1;
                if(opr == '+' || opr == '-' || opr == '/' || opr == '*' || opr == '%'){
                    if(type1 == type2 && (type1 == INT || type1 == FLOAT)){
                        rtype = type1;
                    }
                    else if(type1 == FLOAT && type2 == INT){
                        AST_typecast(FLOAT,p->node.exp.content.operexp.exp2);
                        rtype = FLOAT;
                    } 
                    else if(type2 == FLOAT && type1 == INT){
                        AST_typecast(FLOAT,p->node.exp.content.operexp.exp1);
                        rtype = FLOAT;
                    }
                    else{
                        ERROR("type error on arithmetic expression");
                    }
                }
                else if(opr == '<' || opr == '>' || opr == TK_EQ || opr == TK_NEQ || opr == TK_LEQ || opr == TK_GEQ){
                    if(type1 == type2){ 
                        rtype = BOOL;
                    }
                    else{
                        ERROR("type error on comparative expression");
                    }
                }
                else if(opr == TK_AND || opr == TK_OR){
                    if(type1 == type2 && type1 != BOOL){
                        rtype = BOOL;
                    }
                    else{
                        ERROR("type error on logic expression");
                    }
                }
                DEBUG("binop r: %d\n",rtype); 
                p->node.exp.type = rtype;
                DEBUG("&exp_binop %p\n",p);
                
                break;
            case EXP_UNOP:
                opr = p->node.exp.content.operexp.opr;
                checktypes(p->node.exp.content.operexp.exp1);
                int type = p->node.exp.content.operexp.exp1->type;
                if(opr == '!'){
                    if(type == BOOL){
                        p->node.exp.type = type;
                    }
                    else{
                        ERROR("type error on unary negation");
                    }
                }
                else if(opr == '-'){
                    if(type == INT || type == FLOAT){
                        p->node.exp.type = type;
                    }
                    else{
                        ERROR("type error on unary minus");
                    }
                }
                
                break;
            case EXP_NEW:
                checktypes(p->node.exp.content.newexp.exp);
                if(p->node.exp.content.newexp.exp->type != INT){
                    ERROR("non integer size in array definition");
                }	
                break;
            case EXP_CALL:
                id = p->node.exp.content.callexp.exp1;
                args = p->node.exp.content.callexp.exp2;
                if(check_call(id,unpack(args),size(args))==-1)
                {
                    ERROR("undeclared function");
                }
                break;
            case EXP_VAR:
                type = check_var_decl(p->node.exp.content.varexp->node.var.id);
                DEBUG("exp_var: %s,%d\n",p->node.exp.content.varexp->node.var.id->node.id.name,type);
                if(type == -1){
                    ERROR("undeclared variable: %s",p->node.exp.content.varexp->node.var.id->node.id.name);
                }
                p->node.exp.type = type;
                DEBUG("&exp_var %p\n",p);
                break;
            case CMD_WHILE:
                checktypes(p->node.cmd.whilecmd.exp);
                type = p->node.cmd.whilecmd.exp->node.exp.type;
                if( type != BOOL){
                    ERROR("expected logic (bool) expression, got (%s)",type2string(type));
                }
                else{
                    push_scope();
                    checktypes(p->node.cmd.whilecmd.cmd);
                    pop_scope();
                }
                break;
            case CMD_IF:
                checktypes(p->node.cmd.ifcmd.exp);
                type = p->node.cmd.ifcmd.exp->node.exp.type; 
                if(type != BOOL){
                    ERROR("expected logic (bool) expression, got (%s)",type2string(type));
                }
                else{
                    push_scope();
                    checktypes(p->node.cmd.ifcmd.cmd1);
                    pop_scope();
                    if(p->node.cmd.ifcmd.cmd2 != NULL){
                        push_scope();
                        checktypes(p->node.cmd.ifcmd.cmd2);
                        pop_scope();
                    }
                }
                break;
            case CMD_ATTR:
                DEBUG("attr: before %d\n",p->node.cmd.attrcmd.exp->type);
                DEBUG("&exp %p\n",p->node.cmd.attrcmd.exp);
                checktypes(p->node.cmd.attrcmd.exp);
                DEBUG("attr: after %d\n",p->node.cmd.attrcmd.exp->node.exp.type);
                //checktypes(p->node.cmd.attrcmd.var);
                type = check_var_decl(p->node.cmd.attrcmd.var->node.var.id);
                if(type == -1){
                    ERROR("undeclared variable");
                }

                if(type != p->node.cmd.attrcmd.exp->node.exp.type){
                    ERROR("conflicting types on assignment:\n\t(%s) = (%s)",type2string(type),type2string(p->node.cmd.attrcmd.exp->node.exp.type));
                }
                break;
            case CMD_EXP:
                DEBUG("cmd_exp\n");
                checktypes(p->node.cmd.expcmd.exp);
                break;
            case CAST:
                DEBUG("cast %s\n",type2string(p->node.exp.type));
                break;
            case CMD_BLOCK:
                push_scope();
                checktypes(p->node.cmd.blockcmd.decl);
                checktypes(p->node.cmd.blockcmd.cmd);
                pop_scope();
                break;
            case CMD_RET:
                if(p->node.cmd.retcmd.exp != NULL){
                    type1 = p->node.cmd.retcmd.exp->node.exp.type;
                    type2 = check_return_type(); 
                    if(type1 != type2)
                        ERROR("return type is not correct:\n\t expected (%s), got (%s)",type2string(type2),type2string(type1));
                }
                break;
        }
        if(p->nextElem != NULL){
            checktypes(p->nextElem);
        }
    }
}
