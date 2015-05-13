#! /bin/bash
make
echo "===== testando AST"
for i in testes_sin/*.in
do
    filename=$(basename "$i")
    filename="${filename%.*}"
    echo "=== testando $filename"
    ./main < testes_sin/$filename.in > testes_sin/$filename.out
    diff testes_sin/$filename.out testes_sin/$filename.gab
    echo "=== acabou   $filename"

done
rm testes_sin/*.out
make clean
