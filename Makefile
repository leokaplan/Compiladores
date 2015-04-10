all: main

lex.yy.c: monga.lex
	    flex monga.lex
monga.tab.c: monga.y
		bison -d monga.y
main: lex.yy.c monga.tab.c 
	    gcc -Wall -o main lex.yy.c monga.tab.c -I.  
			
clean:
		$(RM) main lex.yy.c monga.tab.c monga.tab.h 
		$(RM) testes/*.out
