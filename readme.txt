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

Observações:

  Known issue: precedência do menos unário. Quando colocamos a espressão unária entre parênteses não
há erros, porém, sem os parênteses há erro de sintaxe. Tentamos usar %nonassoc, alterar a gramática,
mas não se alterou o resultado. CONSERTADO NO TRABALHO 3.

Mudanças após revisão do Roberto (Issues):

Indirections (tipo) -> Criar subníveis para cada subtipo, ao invés de um nó para o tipão.
AST_litNodeType -> Colocar em exp.
Retirar o nonassoc de '!' e UN_MINUS
Usar o intval dos %token e %type para a linha
Macros nas funções de criação de nó
Precedência de TK_AND e TK_OR
Retirar o "$$ = $1;" do programa -> fim do bottom up, não faz sentido.




