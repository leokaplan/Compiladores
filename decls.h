
#ifndef DECLS_H
#define DECLS_H
void new_var_decl(int type, AST_nodeType* id);
int check_var_decl(AST_nodeType* id);
int check_call(AST_nodeType* id,int* args, int n_args);
void new_func_decl(AST_nodeType* id, int ret_type, int* arg_types,int n_args);
int check_return_type();
void push_scope();
void pop_scope();
#ifndef
