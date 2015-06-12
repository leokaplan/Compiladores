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
 */
void generateMachineCode(AST_nodeType * tree);

#endif
