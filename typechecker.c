
#define ERROR(...) printf(__VA_ARGS__);exit(0);
//retorna o tamanho da lista
int size(AST_nodetype* list){
    return 0;
}
//retorna um array ordenado de tipos correspondentes à lista
int* unpack(AST_nodetype* list){
    return 0;
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
                int type = check_var_decl(p->node.var.id);
                if(type == -1)
                    ERROR("undeclared variable");
                else
                    p->node.var.type = type; 
				break;
			case VAR_ARRAY:
                
                break;
			case EXP_BINOP:
				switch(p->node.exp.operexp.opr){
					case TK_AND:
						break;
					case TK_OR:
						break;
					case TK_EQ:
						break;
					case TK_NEQ:
						break;
					case TK_LEQ:
						break;
					case TK_GEQ:
						break;
					default:
						//aritmetica
						break;
				}
				break;
			case EXP_UNOP:
                //if(op_bool_type[op] != exp1->node.exp.type) 
                if(BOOL != exp1->node.exp.type) 
                    ERROR("expected logic expression");
				break;
			case EXP_NEW:
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
                else{
				break;
			case EXP_VAR:
	            
                break;
			case CMD_WHILE:
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
				if(p->node.cmd.attrcmd.var.type != p->node.cmd.attrcmd.exp,ident){
                    ERROR("conflicting types on assignment");
                }
				break;
			case CMD_EXP:
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
