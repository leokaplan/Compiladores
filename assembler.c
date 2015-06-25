#include "assembler.h"
#include <stdio.h>
#include <string.h>

void ASS_generateMachineCode(AST_nodeType * tree) {
    declCode(node);
}

void declCode(AST_nodeType * node) {
    beginFunction();
    switch(node->tag) {
        case DEC_VAR:
            declVarCode(node);
            break;
        case DEC_FUNC:
            declFuncCode(node);
            break;
        default:
            yyerror("Erro: tipo de declaracao invalido.");
    }
    // listas de declarações
    if (node->nextElem != NULL) {
        declCode(node);
    }
}

void declVarCode(AST_nodeType * node) {
    // TODO talvez não se faça nada, pois sabemos o spot da variável
}

void declFuncCode(AST_nodeType * node) {
    // TODO tratar id e parâmetros etc
    cmdCode(node->node.decl.funcdecl.block);
}

void cmdCode(AST_nodeType * node) {
    switch(node->tag) {
        case CMD_IF:
            cmdIfCode(node);
            break;
        case CMD_WHILE:
            cmdWhileCode(node);
            break;
        case CMD_ATTR:
            cmdAttrCode(node);
            break;
        case CMD_EXP:
            cmdExpCode(node);
            break;
        case CMD_BLOCK:
            cmdBlockCode(node);
            break;
        case CMD_RET:
            cmdRetCode(node);
            break;
        default:
            yyerror("Erro: tipo de comando invalido.");
    }

    // lidar com listas de comandos
    if (node->nextElem != NULL) {
        cmdCode(node->nextElem);
    }
}

void cmdIfCode(AST_nodeType * node) {
    // TODO fazer comparação e tals com
    // node->node.cmd.ifcmd.exp
    // jum condicional pra parte else
    // parte then
    cmdCode(node->node.cmd.ifcmd.cmd1);
    // jump pro fim da parte else
    // label da parte else
    // parte else
    cmdCode(node->node.cmd.ifcmd.cmd2);
    // label do fim da parte else
}

void cmdWhileCode(AST_nodeType * node) {
    // label antes da comparação
    // TODO fazer comparação e tals com
    // node->node.cmd.whilecmd.exp
    // jump condicional pro fim do loop
    cmdCode(node->node.cmd.whilecmd.cmd);
    // jump de volta pro label do inicio
    // label de fim do loop
}

void cmdAttrCode(AST_nodeType * node) {
    expCode(node->exp);
    puts("\tpush %%eax\n");
    varCode(node->var);
    // TODO pop %ecx; mov(?) %ecx, (%eax)
    puts("\tpop %%ecx\n");
    switch(node->exp->node.exp.type) {
        case CHAR:
            puts("  movb    %%ecx, (%%eax)\n");
            break;
        default:
            puts("  movl    %%ecx, (%%eax)\n");
            break;
    }
}

void cmdExpCode(AST_nodeType * node) {
    expCode(node->exp);
}

void cmdBlockCode(AST_nodeType * node) {
    declCode(node->decl);
    cmdCode(node->cmd);
}

void cmdRetCode(AST_nodeType * node) {
    // temos que o padrão de retorno já se encontra ou em eax ou em st(0)
    endFunction();
}

void expCode(AST_nodeType * node) {
    
    switch(node->tag) {
        case EXP_BINOP:
            expBinopCode(node);
            break;
        case EXP_UNOP:
            expUnopCode(node);
            break;
        case EXP_NEW:
            expNewCode(node);
            break;
        case EXP_CALL:
            expCallCode(node);
            break;
        case EXP_VAR:
            expVarCode(node);
            break;
        case LIT_INT:
            litIntCode(node->node.exp.lit.ivalue);
            break;
        case LIT_FLOAT:
            litFloatCode(node->node.exp.lit.fvalue);
            break;
        case LIT_STRING:
            litStringCode(node->node.exp.lit.svalue);
            break;
        default:
            yyerror("Erro: tipo de expressao invalido.");
    }

    // listas de esxpressões, somente utilizado em passagem de parâmetros de chamadas de funções
    if (node->nextElem != NULL) {
        expCode(node->nextElem);
    }
}

void expBinopCode(AST_nodeType * node) {
    // TODO
}

void expUnopCode(AST_nodeType * node) {
    // TODO
}

void expNewCode(AST_nodeType * node) {
    // TODO
}

void expCallCode(AST_nodeType * node) {
    // TODO
}

void expVarCode(AST_nodeType * node) {
    varCode(node->var);
    switch(node->node.exp.type) {
        case CHAR:
            puts("  movzbl  (%%eax), %%eax\n");
            break;
        default:
            puts("  movl    (%%eax), %%eax\n");
            break;
    }
}

void varCode(AST_nodeType * node) {
    switch(node->tag) {
        case VAR_SIMPLE:
            varSimpleCode(node);
            break;
        case VAR_ARRAY:
            varArrayCode(node);
            break;
        yyerror("Erro: tipo invalido de variavel.");
    }
}

void varSimpleCode(AST_nodeType * node) {
    // TODO
}

void varArrayCode(AST_nodeType * node) {
    // TODO
}

void idCode(AST_nodeType * node) {
    // TODO
}

void typeCode(AST_nodeType * node) {
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
    printf(".data S%d:  .string \"%s\"\n.text\n", currStrLabel++, svalue;
}

void beginFunction(char * name) {
   printf(".text\n.globl    %s\n%s:\n", name);
   puts("   push %%ebp\n    movl %%esp, %%ebp\n");
}

void endFunction() {
    puts("  movl %%ebp, %%esp\n");i
    puts("  pop %%ebp");
    puts("  ret\n");
}

