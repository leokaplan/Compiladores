#! /bin/bash
make
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
