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

// TODO fazer função para lidar com listas (de parâmetros, declarações etc.)

/*
 * Função que gera o arquivo .s de saída a partir da AST.
 * Escrevemos na saída padrão, que será redirecionada.
 *
 * @param tree Árvore de sintaxe abstrata.
 */
void ASS_generateMachineCode(AST_nodeType * tree);

// Funções internas do módulo
// Declarações
static void declCode(AST_nodeType * node);
// Subdivisões de declarações
static void declVarCode(AST_nodeType * node);
static void declFuncCode(AST_nodeType * node);

// TODO Será necessário para tipos? A ver.

// Comandos
static void cmdCode(AST_nodeType * node);
// Subdivisões de comandos
static void cmdIfCode(AST_nodeType * node);
static void cmdWhileCode(AST_nodeType * node);
static void cmdAttrCode(AST_nodeType * node);
static void cmdRetCode(AST_nodeType * node);
static void cmdExpCode(AST_nodeType * node);
static void cmdBlockCode(AST_nodeType * node);

// Subsubdivisões de comandos
// TODO

// Expressões
static void expCode(AST_nodeType * node);
// Subdivisões de expressões
static void expBinopCode(AST_nodeType * node);
static void expUnopCode(AST_nodeType * node);
static void expLitCode(AST_nodeType * node);
static void expVarCode(AST_nodeType * node);
static void expParenCode(AST_nodeType * node);
static void expCallCode(AST_nodeType * node);
static void expNewCode(AST_nodeType * node);

// Subsubdivisões de expressões
// TODO

// Variáveis
static void varCode(AST_nodeType * node);
// Subdivisões de variáveis
static void varSimpleCode(AST_nodeType * node);
static void varArrayCode(AST_nodeType * node);

static void idCode(AST_nodeType * node);
// será necesário ?
static void typeCode(AST_nodeType * node);
static void litIntCode(AST_nodeType * node);
static void litFloatCode(AST_nodeType * node);
static void litStringCode(AST_nodeType * node);

// Funções auxiliares de assembly
static void beginProgram();
// TODO adicionar funções
static void endProgram();

#endif
