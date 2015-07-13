#include "assembler.h"
#include "decls.h"
#include <stdio.h>
#include <string.h>

void ASS_generateMachineCode(AST_nodeType * tree) {
    declCode(tree);
}

void declCode(AST_nodeType * p) {
    switch(p->tag) {
        case DEC_VAR:
            declVarCode(p);
            break;
        case DEC_FUNC:
            declFuncCode(p);
            break;
        default:
            yyerror("Erro: tipo de declaracao invalido.");
    }
    // listas de declarações
    if (p->nextElem != NULL) {
        declCode(p);
    }
}

void declVarCode(AST_nodeType * p) {
    // declaração de variável não precisa de código.
    // o acesso ao espaço alocado por ela é dado pela check_slot
    // que fica em decls.h
}

void declFuncCode(AST_nodeType * p) {
    beginFunction(p->node.decl.funcdecl.id->node.id.name);
    // TODO tratar id e parâmetros etc
    cmdCode(p->node.decl.funcdecl.block);
}

void cmdCode(AST_nodeType * p) {
    switch(p->tag) {
        case CMD_IF:
            cmdIfCode(p);
            break;
        case CMD_WHILE:
            cmdWhileCode(p);
            break;
        case CMD_ATTR:
            cmdAttrCode(p);
            break;
        case CMD_EXP:
            cmdExpCode(p);
            break;
        case CMD_BLOCK:
            cmdBlockCode(p);
            break;
        case CMD_RET:
            cmdRetCode(p);
            break;
        default:
            yyerror("Erro: tipo de comando invalido.");
    }

    // lidar com listas de comandos
    if (p->nextElem != NULL) {
        cmdCode(p->nextElem);
    }
}

void cmdIfCode(AST_nodeType * p) {
    int labelEndThen = currentLabel++,
        labelEndElse = currentLabel++;

    expCode(p->node.cmd.ifcmd.exp);
    printf("    cmp $0, %%eax");
    printf("    je  L%d\n", labelEndThen);
    
    cmdCode(p->node.cmd.ifcmd.cmd1);

    printf("    jmp L%d\n", labelEndElse);
    printf("L%d:\n", labelEndThen);

    cmdCode(p->node.cmd.ifcmd.cmd2);
    
    printf("L%d:\n", labelEndElse);
}

void cmdWhileCode(AST_nodeType * p) {
    int labelBeginWhile = currentLabel++,
        labelEndWhile = currentLabel++;

    printf("L%d:\n", labelBeginWhile);
    
    expCode(p->node.cmd.whilecmd.exp);
    printf("    cmp $0, %%eax");
    printf("    je L%d\n", labelEndWhile);

    cmdCode(p->node.cmd.whilecmd.cmd);

    printf("    jmp L%d\n", labelBeginWhile);
    printf("L%d:\n", labelEndWhile);
}

void cmdAttrCode(AST_nodeType * p) {
    expCode(p->node.cmd.attrcmd.exp);
    puts("  push    %%eax\n");
    varCode(p->node.cmd.attrcmd.var);
    puts("  pop     %%ecx\n");
    switch(p->node.cmd.attrcmd.exp->node.exp.type) {
        case CHAR:
            puts("  movb    %%ecx, (%%eax)\n");
            break;
        default:
            puts("  movl    %%ecx, (%%eax)\n");
            break;
    }
}

void cmdExpCode(AST_nodeType * p) {
    expCode(p->node.cmd.expcmd.exp);
}

void cmdBlockCode(AST_nodeType * p) {
    declCode(p->node.cmd.blockcmd.decl);
    cmdCode(p->node.cmd.blockcmd.cmd);
}

void cmdRetCode(AST_nodeType * p) {
    AST_nodeType * expression = p->node.cmd.retcmd.exp;
    if ((expression->node.exp.type == INT) || (expression->node.exp.type == BOOL)) {
        if (expression->tag == VAR_SIMPLE) {
            printf("    movl    -%d(%%ebp), %%eax\n", 4*check_slot(expression->node.exp.content.varexp->node.var.id));
        }
        else /* Constante */ {
            printf("    movl    $%d, %%eax\n", expression->node.exp.content.lit.ivalue);
        }
    }
    else if (expression->node.exp.type == FLOAT) {
        if (expression->tag == VAR_SIMPLE) {
            printf("    fstpl   -%d(%%ebp)\n", 4*check_slot(expression->node.exp.content.varexp->node.var.id));
        }
        else /* Constante */ {
            printf("    fstpl   $%f\n", expression->node.exp.content.lit.fvalue);
        }
    }
    endFunction();
}

void expCode(AST_nodeType * p) {
    
    switch(p->tag) {
        case EXP_BINOP:
            expBinopCode(p);
            break;
        case EXP_UNOP:
            expUnopCode(p);
            break;
        case EXP_NEW:
            expNewCode(p);
            break;
        case EXP_CALL:
            expCallCode(p);
            break;
        case EXP_VAR:
            expVarCode(p);
            break;
        case LIT_INT:
            litIntCode(p->node.exp.content.lit.ivalue);
            break;
        case LIT_FLOAT:
            litFloatCode(p->node.exp.content.lit.fvalue);
            break;
        case LIT_STRING:
            litStringCode(p->node.exp.content.lit.svalue);
            break;
        default:
            yyerror("Erro: tipo de expressao invalido.");
    }

    // listas de expressões, somente utilizado em passagem de parâmetros de chamadas de funções
    if (p->nextElem != NULL) {
        expCode(p->nextElem);
    }
}

void expBinopCode(AST_nodeType * p) {
    expCode(p->node.exp.content.operexp.exp1);
    puts("  movl    %%eax, %%ecx\n");
    expCode(p->node.exp.content.operexp.exp2);
    switch(p->node.exp.content.operexp.opr) {
        case '+':
            puts("  addl    %%ecx, %%eax\n");
            break;
        case '-':
            puts("  subl    %%eax, %%ecx\n");
            puts("  movl    %%ecx, %%eax\n");
            break;
        case '*':
            puts("  imul    %%ecx, %%eax\n");
            break;
        defaul:
            yyerror("Casos não tratados.");

    }
}

void expUnopCode(AST_nodeType * p) {
    expCode(p->node.exp.content.operexp.exp1);
    switch(p->node.exp.content.operexp.opr) {
        case '-':
            // -eax == eax - 2*eax
            puts("  movl    %%eax, %%ecx\n");
            puts("  shll    $1, %%ecx\n");
            puts("  subl    %%ecx, %%eax\n");
            break;
        default:
            yyerror("Casos não tratados.");
    }
}

void expNewCode(AST_nodeType * p) {
    // TODO
}

void expCallCode(AST_nodeType * p) {
    int amountPushed = 0;
    // TODO ir dando push nos parâmetros recursivamente
    printf("    call    %s\n", p->node.exp.content.callexp.exp1->node.exp.content.varexp->node.var.id->node.id.name);
    printf("    subl    $%d, %%esp\n", amountPushed);
}

void expVarCode(AST_nodeType * p) {
    varCode(p->node.exp.content.varexp);
    switch(p->node.exp.type) {
        case CHAR:
            puts("  movzbl  (%%eax), %%eax\n");
            break;
        default:
            puts("  movl    (%%eax), %%eax\n");
            break;
    }
}

void varCode(AST_nodeType * p) {
    switch(p->tag) {
        case VAR_SIMPLE:
            varSimpleCode(p);
            break;
        case VAR_ARRAY:
            varArrayCode(p);
            break;
        yyerror("Erro: tipo invalido de variavel.");
    }
}

void varSimpleCode(AST_nodeType * p) {
    // TODO
}

void varArrayCode(AST_nodeType * p) {
    // TODO
}

void idCode(AST_nodeType * p) {
    // TODO
}

void typeCode(AST_nodeType * p) {
    // TODO
}

void litIntCode(int ivalue) {
    printf("    movl    $%d, %%eax\n", ivalue);
}

void litFloatCode(float fvalue) {
    printf("    movl    $%f, %%eax\n", fvalue);
}

void litStringCode(char * svalue) {
    // Cria uma linha com a constante string junto com seu label
    printf(".data S%d:  .string \"%s\"\n.text\n", currStrLabel, svalue);
    printf("  movl  S%d, %%eax\n", currStrLabel);
    currStrLabel++;
}

void beginFunction(char * name) {
    printf(".text\n.globl    %s\n%s:\n", name, name);
    puts("  push %%ebp\n");
    puts("  movl %%esp, %%ebp\n");
    // aloca as variáveis locais
    printf("    movl    $%d, %%eax\n", maxslot);
    puts("  shll    $2, %%eax\n");
    puts("  subl    %%eax, %%esp\n");
}

void endFunction() {
    puts("  movl %%ebp, %%esp\n");
    puts("  pop %%ebp\n");
    puts("  ret\n");
}
