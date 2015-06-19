#include "assembler.h"
#include <stdio.h>
#include <string.h>

void ASS_generateMachineCode(AST_nodeType * tree) {

    // TODO talvez criar alguma função auxiliar de adicionar aos labels lá de cima (.globl ou algo assim)
    declCode(node);
    
    endProgram();
}


void declCode(AST_nodeType * node) {
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
    // TODO
}

void declFuncCode(AST_nodeType * node) {
    // TODO
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
    // TODO
}

void cmdWhileCode(AST_nodeType * node) {
    // TODO
}

void cmdAttrCode(AST_nodeType * node) {
    expCode(node->exp);
    // TODO push %eax
    puts("\tpush %eax");
    varCode(node->var);
    // TODO pop %ecx; mov(?) %ecx, (%eax)
}

void cmdExpCode(AST_nodeType * node) {
    expCode(node->exp);
}

void cmdBlockCode(AST_nodeType * node) {
    declCode(node->decl);
    cmdCode(node->cmd);
}

void cmdRetCode(AST_nodeType * node) {
    // TODO
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
            litIntCode(node);
            break;
        case LIT_FLOAT:
            litFloatCode(node);
            break;
        case LIT_STRING:
            litStringCode(node);
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
    // TODO mov(?) (%eax), %eax
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

void litIntCode(AST_nodeType * node) {
    // TODO
}

void litFloatCode(AST_nodeType * node) {
    // TODO
}

void litStringCode(AST_nodeType * node) {
    // TODO
}

void beginProgram() {
    // TODO ver como separar as seções, como .glodl, .data, .text etc.
}

void endProgram() {
    puts("\tmovl %ebp, %esp\n");
    puts("\tret\n");
}

