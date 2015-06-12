#include "assembler.h"
#include <stdio.h>
#include <string.h>

#define OPEN_FILE(file, fileName) \
            if((file = fopen(fileName, "w")) == NULL) \
                yyerror("Nao foi possivel abrir o arquivo da linguagem objeto.")

void ASS_generateMachineCode(AST_nodeType * tree, char * fileName) {
    FILE * file;
    OPEN_FILE(file, fileName);

    // TODO talvez criar alguma função auxiliar de adicionar aos labels lá de cima (.globl ou algo assim
    generateDeclCode(node);
    
    // TODO criar uma função auxiliar para escrever o fim do arquivo .s
    fclose(file);
}

// TODO colocar os corpos das funções definidas no header
