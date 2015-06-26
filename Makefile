all: main

lex: monga.lex
	    flex monga.lex
monga: monga.y
		bison -d monga.y
abstractsyntaxtree: abstractsyntaxtree.c
		gcc -Wall -m32 -c abstractsyntaxtree.c 
types: types.c
		gcc -Wall -m32 types.c -c
decls: decls.c
		gcc -Wall -m32 decls.c -c
assembler: assembler.c
		gcc -Wall -m32 -c assembler.c

main: lex monga abstractsyntaxtree types decls assembler 
	    gcc -Wall -m32 -g -o main lex.yy.c monga.tab.c abstractsyntaxtree.o types.o decls.o assembler.o -I. 
			
clean:
		$(RM) main lex.yy.c monga.tab.c monga.tab.h decls.o types.o abstracsyntaxtree.o assembler.o
