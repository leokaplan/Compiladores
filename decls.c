
#include "abstractsyntaxtree.h"
#include <stdlib.h>
//lista de declaracoes
struct decl{
    struct decl* next;
    int type;
    AST_nodeType* id;
};
typedef struct decl decl;
decl* head = NULL;//root
decl* tail = NULL;

struct _scope{
    decl* declaration;
    struct _scope* last;
};
typedef struct _scope Scope;
Scope* scope = NULL;

void push_scope(){
   Scope* new = malloc(sizeof(Scope)); 
   new->last = scope;
   new->declaration = tail;
   scope = new;
}
void pop_scope(){
    Scope* temp = scope->last;
    decl* it = temp->declaration;
    decl* it_temp;
    while(it->next != NULL){
        it_temp = it->next;
        free(it);
        it = it_temp;
    }
    free(scope);
    scope = temp;
}
decl* get_scope(){
    return scope->declaration;    
}
//retorna o tipo se achar, -1 se nao achar 
int check_var_decl_scope(AST_nodeType* id){
    decl* it = get_scope();
    while(it->next != NULL){
        if(it->id == id){
            return it->type;
        } 
        it = it->next;
    }
    return -1;
}
int check_var_decl(AST_nodeType* id){
    decl* it = head;
    while(it->next != NULL){
        if(it->id == id){
            return it->type;
        } 
        it = it->next;
    }
    return -1;
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
        if(check_var_decl_scope(id)!=-1){
           error("variavel ja declarada");
        }
        if(check_var_decl(id)!=-1){
           warning("shadowing");
        }
        tail->next = new;
    }
    tail = new;
}


struct _func_decl
{
    int ret_type;
    AST_nodeType* id;
    int* arg_types;
    int n_args;
    struct _func_decl* next;
};
typedef struct _func_decl func_decl;

func_decl* func_head;
func_decl* func_tail;
int return_type = -1;

void set_return_type(int type){
    return_type = type;
}
int check_return_type(){
    return return_type;
}
//retorna tipo de retorno da funcao ou -1 caso nao exista esse par (id,{params})
int check_call(AST_nodeType* id,int* args, int n_args){
    func_decl* it = func_head;
    while(it->next != NULL){
        if(it->id == id){
            int flag = 0;
            int i;
            if(it->n_args == n_args){
                for(i=0;i<n_args;i++){
                    if(args[i] != it->args_types[i]) 
                        flag = 1;
                }
            }
            if(!flag)
                return it->ret_type;
        } 
        it = it->next;
    }
    return -1;
}
//permite funcoes com mesmo id e assinaturas diferentes
void new_func_decl(AST_nodeType* id, int ret_type, int* arg_types,int n_args){
    func_decl* new = malloc(sizeof(func_decl));
    new->ret_type = ret_type;
    set_return_type(ret_type);
    new->arg_types = arg_types;
    new->n_args = n_args;
    new->id = id;
    new->next = NULL;
    if(func_head==NULL){//lista vazia
        func_head = new;
    }
    else{
        if(check_call(id,arg_types,n_args)==ret_type){
           error("funcao ja declarada");
        }
        func_tail->next = new;
    }
    func_tail = new;
    
}
