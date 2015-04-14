#! /bin/bash
make
echo "===== testando lexico"
for i in testes_lex/*.in
do
    filename=$(basename "$i")
    filename="${filename%.*}"
    echo "=== testando $filename"
    ./main < testes_lex/$filename.in > testes_lex/$filename.out
    diff testes_lex/$filename.out testes_lex/$filename.gab
    echo "=== acabou   $filename"

done
echo "===== testando sintatico"
for i in testes_sin/*.in
do
    filename=$(basename "$i")
    filename="${filename%.*}"
    echo "=== testando $filename"
    ./main < testes_sin/$filename.in > testes_sin/$filename.out
    diff testes_sin/$filename.out testes_sin/$filename.gab
    echo "=== acabou   $filename"

done
make clean
