
#ifndef TYPES_H
#define TYPES_H
#define NUMUNOP 2
#define NUMBOP 11

int binop_key(int op);

int unop_key(int op);
int op_unop_type[NUMUNOP][NUM_BASETYPES];
int op_arithm_left[NUMBOP][NUM_BASETYPES];
int op_arithm_right[NUMBOP][NUM_BASETYPES];
int op_arithm_result[NUMBOP][NUM_BASETYPES][NUM_BASETYPES];

int basetype(AST_typeEnum type);
int array(int type);
int array2basetype(int type);
int type2size(int type);
#endif
