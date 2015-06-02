
#include "abstractsyntaxtree.h"
#include <stdlib.h>
//lista de declaracoes
typedef struct {
    struct decl* next;
    int type;
    AST_nodeType* id;
}decl;
decl* head;//root
decl* tail;

//retorna o tipo se achar, -1 se nao achar 
int check_var_decl(AST_nodeType* id){
    while(1){}
}

//cria uma declaracao na lista de declaracoes
void new_var_decl(int type, AST_nodeType* id){
    decl* new = malloc(sizeof(decl));
    new->type = type;
    new->id = id;
    new->next = NULL;
    if(head==NULL){//lista vazia
        head = new;
    }
    else{
        if(check_var_decl(id)!=-1) error(variavel ja declarada);
        tail->next = new;
    }
    tail = new;
}
