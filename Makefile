all: main.exe

lex.yy.c: monga.lex
	    lex monga.lex

main.exe: lex.yy.c main.c 
	    gcc -o main.exe main.c -I.  
test: test1 
			
test1:	main.exe 
		@./main.exe < testes/1.in > testes/1.out 
		@diff testes/1.out testes/1.gab
		@echo "teste 1 OK"
clean:
		$(RM) main.exe lex.yy.c 
		$(RM) testes/*.out
