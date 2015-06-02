#include "abstractsyntaxtree.h"
//retorna um inteiro correspondente ao tipo base
int basetype(AST_typeEnum type) {
	return type;
}
//dado um tipo, retorna um inteiro correspondente a um array do tipo dado. é composivel.
int array(int type){
    return type + NUM_BASETYPES;
}
