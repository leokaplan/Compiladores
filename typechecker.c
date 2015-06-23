#include "abstractsyntaxtree.h"
#define ERROR(...) printf(__VA_ARGS__);exit(0);
//retorna o tamanho da lista 
int size(AST_nodetype* list){
    int i;
    for(i=1;list->nextElem;i++)
        list = list->nextElem;
    return i;
}
//retorna um array ordenado de tipos correspondentes à lista de expressoes
//em caso de falta de memoria retorna NULL
int* unpack(AST_nodetype* explist){
    int size = size(explist);
    int* ret = malloc(sizeof(int)*size);
    int i;
    for(i=0;list->nextElem;i++)
    {
        ret[i] = list->node.exp.type;
        list = list->nextElem;
    }
    return ret;
}

void checktypes(AST_nodeType *p){
    if (p != NULL) {
        switch(p->tag){
            case DEC_VAR:
                int type = p->node.decl.vardecl.type;
                AST_nodeType* id = p->node.decl.vardecl.id;
                new_var_decl(type,id);
                break;
            case DEC_FUNC:
                int type = p->node.decl.funcdecl.type;
                AST_nodeType* id = p->node.decl.funcdecl.id;
                AST_nodeType* param = p->node.decl.funcdecl.param;
                //tem um push scope embutido
                new_func_decl(id,type,unpack(param),size(param));
                //declara as variaveis do cabeçalho como locais na função
                checktype(p->node.decl.funcdecl.param);
                checktype(p->node.decl.funcdecl.block);

                pop_scope();

                break;
            case VAR_SIMPLE:
            case VAR_ARRAY:
                int type = check_var_decl(p->node.var.id);
                if(type == -1)
                    ERROR("undeclared variable");
                else
                    p->node.var.type = type; 
                //with constant folding type could be checked
                break;
            case EXP_BINOP:
                int opr = p->node.exp.operexp.opr;
                int op = binop_key(opr);
                checktype(p->node.exp.operexp.exp1.type);
                checktype(p->node.exp.operexp.exp2.type);
                int type1 = p->node.exp.operexp.exp1.type;
                int type2 = p->node.exp.operexp.exp2.type;
                if(op_arithm_left[op][type1] == 1 && op_arithm_right[op][type2] == 1) 
                {
                    ERROR("type error on arithmetic expression");
                }
                int rtype = op_arithm_result[op][type1][type2];
                //if lit in both sizes, fold
                if(rtype != -1){
                    if(opr == '+' || opr == '-' || opr == '/' || opr == '*'){
                        if(type1 != rtype) 
                            AST_typecast(rtype,p->node.exp.operexp.exp1);
                        if(type2 != rtype) 
                            AST_typecast(rtype,p->node.exp.operexp.exp2);
                    }
                    if(opr == '%'){
                        if(type1 != INT) 
                            AST_typecast(INT,p->node.exp.operexp.exp1);
                        if(type2 != INT) 
                            AST_typecast(INT,p->node.exp.operexp.exp2);
                    }
                    if(opr == '<' || opr == '>'){
                        if(type1 != type2) 
                            ERROR("type error on comparative expression");
                    }
                }
                p->node.exp.type = rtype;
                break;
            case EXP_UNOP:
                checktype(p->node.exp.type);
                if(op_unop_type[op] != exp1->node.exp.type) 
                    ERROR("expected logic expression");
                break;
            case EXP_NEW:
                checktype(p->node.exp.type);
                if(p->node.exp.content.newexp.exp.type != INT){
                    ERROR("non integer size in array definition");
                }	
                break;
            case EXP_CALL:
                AST_nodeType* id = p->node.exp.callexp.exp1.id;
                AST_nodeType* args = p->node.exp.callexp.exp2;
                if(check_call(id,unpack(args),size(args))==-1)
                {
                    ERROR("undeclared function");
                }
                break;
            case EXP_VAR:
                break;
            case CMD_WHILE:
                checktype(p->node.exp.type);
                if(p->node.cmd.whilecmd.exp.type != BOOL){
                    ERROR("expected logic expression");
                }
                else{
                    push_scope();
                    checktype(p->node.cmd.whilecmd.cmd);
                    pop_scope();
                }
                break;
            case CMD_IF:
                checktype(p->node.exp.type);
                if(p->node.cmd.ifcmd.exp.type != BOOL){
                    ERROR("expected logic expression");
                }
                else{
                    push_scope();
                    checktype(p->node.cmd.ifcmd.cmd1);
                    pop_scope();
                    if(p->node.cmd.ifcmd.cmd2 != NULL){
                        push_scope();
                        checktype(p->node.cmd.ifcmd.cmd2);
                        pop_scope();
                    }
                }
                break;
            case CMD_ATTR:
                typecheck(p->node.cmd.attrcmd.exp.type);
                typecheck(p->node.cmd.attrcmd.var.type);
                if(p->node.cmd.attrcmd.var.type != p->node.cmd.attrcmd.exp.type){
                    ERROR("conflicting types on assignment");
                }
                break;
            case CMD_EXP:
                typecheck(p->node.cmd.expcmd.exp);
                break;
            case CAST:
                break;
            case CMD_BLOCK:
                push_scope();
                typecheck(p->node.cmd.blockcmd.decl);
                typecheck(p->node.cmd.blockcmd.cmd);
                pop_scope();
                break;
            case CMD_RET:
                if(p->node.cmd.retcmd.exp != NULL){
                    if(p->node.cmd.retcmd.exp.type != check_return_type() )
                        ERROR("return type is not correct");
                }
                break;
        }
        if(p->nextElem != NULL){
            typecheck(p->nextElem);
        }
    }
}
