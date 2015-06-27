/*
 * Módulo de geração de código
 * assembler.h
 *
 * INF1715
 * Bernardo Alkmim
 * Leonardo Kaplan
 */

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "abstractsyntaxtree.h"

// Número máximo de slots, exportado pelo semântico
extern int maxslot;

/*
 * Função que gera o arquivo .s de saída a partir da AST.
 * Escrevemos na saída padrão, que será redirecionada.
 *
 * @param tree Árvore de sintaxe abstrata.
 */
void ASS_generateMachineCode(AST_nodeType * tree);

// Número do label corrente
static int currentLabel = 0;
// Número do label de string corrente
static int currStrLabel = 0;

// Funções internas do módulo
// Declarações
static void declCode(AST_nodeType * p);
// Subdivisões de declarações
static void declVarCode(AST_nodeType * p);
static void declFuncCode(AST_nodeType * p);

// TODO Será necessário para tipos? A ver.

// Comandos
static void cmdCode(AST_nodeType * p);
// Subdivisões de comandos
static void cmdIfCode(AST_nodeType * p);
static void cmdWhileCode(AST_nodeType * p);
static void cmdAttrCode(AST_nodeType * p);
static void cmdRetCode(AST_nodeType * p);
static void cmdExpCode(AST_nodeType * p);
static void cmdBlockCode(AST_nodeType * p);

// Subsubdivisões de comandos
// TODO

// Expressões
static void expCode(AST_nodeType * p);
// Subdivisões de expressões
static void expBinopCode(AST_nodeType * p);
static void expUnopCode(AST_nodeType * p);
static void expLitCode(AST_nodeType * p);
static void expVarCode(AST_nodeType * p);
static void expParenCode(AST_nodeType * p);
static void expCallCode(AST_nodeType * p);
static void expNewCode(AST_nodeType * p);

// Subsubdivisões de expressões
// TODO

// Variáveis
static void varCode(AST_nodeType * p);
// Subdivisões de variáveis
static void varSimpleCode(AST_nodeType * p);
static void varArrayCode(AST_nodeType * p);

static void idCode(AST_nodeType * p);
// será necesário ?
static void typeCode(AST_nodeType * p);
static void litIntCode(int ivalue);
static void litFloatCode(float fvalue);
static void litStringCode(char * svalue);

// Funções auxiliares de assembly
static void beginFunction(char * name);
static void endFunction();

// Imprime o label e incrementa o valor de currentLabel para o próximo
static void printLabel();

#endif
