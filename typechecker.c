
    
void checktypes(AST_nodeType *p){
    if (p != NULL) {
		switch(p->tag){
			case LIT_INT:	
				break;
			case LIT_FLOAT:
				break;
			case LIT_STRING:
				break;
			case ID: 
				break;
			case TYP:
				switch(p->node.typ.type){
					case INT:
						break;
					case FLOAT:
						break;
					case CHAR:
						break;
					case VOID:
						break;
				}
				break;
			case DEC_VAR:
				int type = p->node.decl.vardecl.type;
                AST_nodeType* id = p->node.decl.vardecl.id;
                new_var_decl(type,id);
				break;
			case DEC_FUNC:
                int type = p->node.decl.funcdecl.type;
                AST_nodeType* id = p->node.decl.funcdecl.id;
				AST_nodeType* param = p->node.decl.funcdecl.param;
                new_func_decl(id,type,unpack(param),size(param));
				checktype(p->node.decl.funcdecl.block);
                pop_scope();
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
