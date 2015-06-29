#! /bin/bash
make
echo "===== testando AST"
for i in testes_sem/*.in
do
    filename=$(basename "$i")
    filename="${filename%.*}"
    echo "=== testando $filename"
    ./main < testes_sem/$filename.in > testes_sem/$filename.out
    diff testes_sem/$filename.out testes_sem/$filename.in
    echo "=== acabou   $filename"

done
rm testes_sem/*.out
make clean
