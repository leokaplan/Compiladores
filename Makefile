all: main

lex.yy.c: monga.lex
	    lex monga.lex

main: lex.yy.c main.c 
	    gcc -o main main.c -I.  
