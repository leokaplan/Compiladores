----------------------------------
Trabalho de Compiladores INF1715
----------------------------------
Bernardo Pinto de Alkmim - 1210514
Leonardo Kaplan - 1212509

Arquivo README
--------------

Dependências:
  gcc
  flex
  diff
  make
  bash
  

Como utilizar:

  Para compilar
    make 
  Para limpar os arquivos executaveis e temporarios
    make clean
  Para compilar e testar e limpar
    $./build


Decisões tomadas:

  Tratamos caracteres entre plics (ex.: 'o') com inteiros, assim como C. Também damos suporte a
floats e inteiros em hexadecimal. Os comentários funcionam como em C também, ou seja, não podem
ser encadeados. Como está dito na definição da linguagem Monga, os únicos escapes válidos em strings
são \t, \n e \"; os outros, portanto, gerarão erro.
  Tokens de um caracter só têm como valor o próprio caracter (ex.: { é representado por '{').
  As expressões na gramática têm prioridade, da seguinte ordem
  "simples" > multiplicativas > aditivas > comparativas > lógicas
  Permitimos sobrecarga de funções.
  Temos shadowing de variáveis.
  Para a geração de código, supomos arquitetura IA-32.

Observações:

Mudanças após revisão do Roberto (Issues):

AST_litNodeType -> Colocar em exp.

Usar o intval dos %token e %type para a linha
    OK no lex, falta mudar o cabecalho de todas as funcoes e passar o comando como parametro em cada chamada


TODO:
    criar decls, a lista de declaracoes
        se for populando em ordem, ja garante shadowing

    onde uma declaracao eh um nome, um tipo e uma linha 


    refazer os tipos
    criar o types (array de tipos) pra gente poder costurar
    criar a tabela de simbolos (dar p cada uso um numero p fazer um mapa int->int) ai da p fazer 
        //acha associa o _nome_ de uma var com uma declaracao
        decl = decls.root
        achou = false
        while decl->next
            if nome==decl.sym//igual de string 
                //shadowing:
                if achou 
                    --> shadowing
                else
                    //p ver se o tipo ta ok
                    if tipo_esperado == decl.tipo 
                        //ok
                        nothing
                    else 
                        --> type error
                    end
                    achou = true
                    //break;//sem warning de shadowing
            else
                decl = decl->next
            end
        end
        if !achou 
            --> variavel nao declarada


        usar 2 'pilhas' fazer escopo
            ai inves de ser shadowing é dupla declaracao, erro.
            shadowing eh qnd vc redeclara uma variavel de um escopo acima.


TODO pratico:

    coisas tipadas sao expressoes e toda expressao tem um campo tipo int
        ok
    conta do tipo mudar -> tipo = tipobase + numtipos*indirecoes 
        ok 
    void push_scope()
        ok
        muda escopo, na abertura de funcoes e blocos, promovendo shadowing 
        usar nos comecos de blocos e de declaracao de funcao 
    void pop_scope()
        ok
        usar no fim dos blocos e no return
    
    void set_return_type(int type)
        armazena o tipo do retorno da funcao sendo analizada
        ok
        
    int  check_return_type()
        le o tipo do retorno da funcao sendo analizada
        ok
    
    void new_func_decl(char* id, int ret_type, int* arg_types,int n_args)
        ok
        quando for chamar, tratar a lista parametros
    void new_var_decl(char* id,int type, int line)
        pode dar erro(retornar um int error talvez?)
        ok
    int check_var_decl(char* id) 
        retorna o tipo do id para qualquer escopo acima
        ok
    int check_var_decl_scope(char* id) 
        retorna o tipo do id ja resolvido(isso é, dentro do escopo e tal)
        ok
    int check_call(char* id, int* args_types, int n_args)
        retorna o tipo de retorno da funcao, ou erro(-1, talvez)
        ok
