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

/*
 * Função que gera o arquivo .s de saída a partir da AST.
 *
 * @param tree Árvore de sintaxe abstrata.
 * @param fileName Nome do arquivo a ser criado.
 */
void ASS_generateMachineCode(AST_nodeType * tree, char * fileName);

// Funções internas do módulo
// Declarações
static void generateDeclCode(AST_nodeType * node);
// Subdivisões de declarações
static void generateDeclVarCode(AST_nodeType * node);
static void generateDeclFuncCode(AST_nodeType * node);

// TODO Será necessário para tipos? A ver.

// Comandos
static void generateCmdCode(AST_nodeType * node);
// Subdivisões de comandos
static void generateCmdIfCode(AST_nodeType * node);
static void generateCmdWhileCode(AST_nodeType * node);
static void generateCmdAttrCode(AST_nodeType * node);
static void generateCmdRetCode(AST_nodeType * node);
static void generateCmdExpCode(AST_nodeType * node);
static void generateCmdBlockCode(AST_nodeType * node);

// Subsubdivisões de comandos
// TODO

// Expressões
static void generateExpCode(AST_nodeType * node);
// Subdivisões de expressões
static void generateExpOprCode(AST_nodeType * node);
static void generateExpLitCode(AST_nodeType * node);
static void generateExpVarCode(AST_nodeType * node);
static void generateExpParenCode(AST_nodeType * node);
static void generateExpCallCode(AST_nodeType * node);
static void generateExpNewCode(AST_nodeType * node);

// Subsubdivisões de expressões
// TODO

// Variáveis
static void generateVarCode(AST_nodeType * node);
// Subdivisões de variáveis
static void generateVarSimpleCode(AST_nodeType * node);
static void generateVarArrayCode(AST_nodeType * node);

static void generateIdCode(AST_nodeType * node);
// TODO Ver como gerar as listas de coisas, provavelmente pelos próprios nós

#endif
