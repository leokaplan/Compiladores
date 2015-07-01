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
  Não aceitamos declarações de variáveis com atribuições, devem ser comandos separados.
  Para a geração de código, supomos arquitetura IA-32.

Observações:

    A geração de código se encontra incompleta.
