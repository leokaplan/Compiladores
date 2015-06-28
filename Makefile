FLAGS = -m32 -g 
+FLAGS = -Wall

all: main

lex: monga.lex
	    flex monga.lex
monga: monga.y
		bison -d monga.y
abstractsyntaxtree: abstractsyntaxtree.c
		gcc $(FLAGS) -c abstractsyntaxtree.c 
types: types.c
		gcc  $(FLAGS) -c types.c 
decls: decls.c
		gcc  $(FLAGS) -c decls.c 
assembler: assembler.c
		gcc  $(FLAGS) -c assembler.c

main: lex monga abstractsyntaxtree types decls assembler 
	    gcc $(FLAGS) -o main lex.yy.c monga.tab.c abstractsyntaxtree.o types.o decls.o assembler.o 
			
clean:
		$(RM) main lex.yy.c monga.tab.c monga.tab.h decls.o types.o abstractsyntaxtree.o assembler.o
