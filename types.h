
#ifndef TYPES_H
#define TYPES_H

int basetype(AST_typeEnum type);
int array(int type);
int array2basetype(int type);
int type2size(int type);
char* type2string(int type);
#endif
