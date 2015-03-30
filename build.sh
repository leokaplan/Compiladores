#! /bin/bash
make
for i in testes/*.in
do
    filename=$(basename "$i")
    filename="${filename%.*}"
    echo "=== testando $filename"
    ./main < testes/$filename.in > testes/$filename.out
    diff testes/$filename.out testes/$filename.gab
    echo "=== acabou   $filename"

done
make clean
