#include "abstractsyntaxtree.h"
#include "types.h"
//retorna um inteiro correspondente ao tipo base
int basetype(AST_typeEnum type) {
	return type;
}
//dado um tipo, retorna um inteiro correspondente a um array do tipo dado. é composivel.
int array(int type){
    return type + NUM_BASETYPES;
}
//dado um tipo, array ou nao, retorna o tipo basico associado
int array2basetype(int type){
    return type % NUM_BASETYPES;
}
int type2size(int type){
    return 4;    
}

/*
 * for preety error messages
*/
char* type2string(int type){
    if(type == INT)   return "int";    
    if(type == FLOAT) return "float";    
    if(type == BOOL)  return "bool";    
    if(type == VOID)  return "void";    
    if(type == -1)    return "error";
}
