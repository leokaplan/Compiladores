all: main

lex.yy.c: monga.lex
	    flex monga.lex
monga.tab.c: monga.y
		bison -d monga.y
abstractsyntaxtree.o: abstractsyntaxtree.c
		gcc -Wall -m32 abstractsyntaxtree.c -c
types.o: types.c
		gcc -Wall -m32 types.c -c
decls.o: decls.c
		gcc -Wall -m32 decls.c -c
main: lex.yy.c monga.tab.c abstractsyntaxtree.o types.o decls.o 
	    gcc -Wall -m32 -g -o main lex.yy.c monga.tab.c abstractsyntaxtree.o -I.  
			
clean:
		$(RM) main lex.yy.c monga.tab.c monga.tab.h 
