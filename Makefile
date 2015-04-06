all: main

lex.yy.c: monga.lex
	    lex monga.lex

main: lex.yy.c main.c 
	    gcc -Wall -o main main.c -I.  
			
clean:
		$(RM) main lex.yy.c 
		$(RM) testes/*.out
