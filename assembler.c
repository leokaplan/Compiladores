#include "assembler.h"
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
    // TODO talvez não se faça nada, pois sabemos o spot da variável
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
    // TODO fazer comparação e tals com
    // p->node.cmd.ifcmd.exp
    // jum condicional pra parte else
    // parte then
    cmdCode(p->node.cmd.ifcmd.cmd1);
    // jump pro fim da parte else
    // label da parte else
    // parte else
    cmdCode(p->node.cmd.ifcmd.cmd2);
    // label do fim da parte else
}

void cmdWhileCode(AST_nodeType * p) {
    // label antes da comparação
    // TODO fazer comparação e tals com
    // p->node.cmd.whilecmd.exp
    // jump condicional pro fim do loop
    cmdCode(p->node.cmd.whilecmd.cmd);
    // jump de volta pro label do inicio
    // label de fim do loop
}

void cmdAttrCode(AST_nodeType * p) {
    expCode(p->node.cmd.attrcmd.exp);
    puts("\tpush %%eax\n");
    varCode(p->node.cmd.attrcmd.var);
    // TODO pop %ecx; mov(?) %ecx, (%eax)
    puts("\tpop %%ecx\n");
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
    // temos que o padrão de retorno já se encontra ou em eax ou em st(0)
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

    // listas de esxpressões, somente utilizado em passagem de parâmetros de chamadas de funções
    if (p->nextElem != NULL) {
        expCode(p->nextElem);
    }
}

void expBinopCode(AST_nodeType * p) {
    // TODO
}

void expUnopCode(AST_nodeType * p) {
    // TODO
}

void expNewCode(AST_nodeType * p) {
    // TODO
}

void expCallCode(AST_nodeType * p) {
    // TODO
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
    printf("$%d", ivalue);
}

void litFloatCode(float fvalue) {
    printf("$%f", fvalue);
}

void litStringCode(char * svalue) {
    // Cria uma linha com a constante string junto com seu label
    printf(".data S%d:  .string \"%s\"\n.text\n", currStrLabel++, svalue);
}

void beginFunction(char * name) {
   printf(".text\n.globl    %s\n%s:\n", name);
   puts("   push %%ebp\n    movl %%esp, %%ebp\n");
}

void endFunction() {
    puts("  movl %%ebp, %%esp\n");
    puts("  pop %%ebp");
    puts("  ret\n");
}
