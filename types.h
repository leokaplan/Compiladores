
#ifndef TYPES_H
#define TYPES_H
#define NUMUNOP 2
#define NUMBOP 11

int binop_key(int op) {
    switch(op) {
        case '+':     return 0;
        case '-':     return 1;
        case '/':     return 2;
        case '*':     return 3;
        case '%':     return 4;
        case '<':     return 5;
        case '>':     return 6;
        case TK_EQ:   return 7;
        case TK_NEQ:  return 8;
        case TK_LEQ:  return 9;
        case TK_GEQ:  return 10;
        default: return -1;
    }
}

int unop_key(int op) {
    switch(op) {
        case '!': return 0;
        case '-': return 1;
        default: return -1;
    }
}
//melhorar isso
int op_unop_type[NUMUNOP][NUM_BASETYPES] = {
    //char,int,float,void,bool
    {0,0,0,0,1},// !
    {1,1,1,0,0} // -
};
int op_arithm_left[NUMBOP][NUM_BASETYPES] = {
    //char,int,float,void,bool
    {1,1,1,0,0},// +
    {1,1,1,0,0},// -
    {1,1,1,0,0},// /
    {1,1,1,0,0},// *
    {1,1,1,0,0},// %
    {1,1,1,0,0},// <
    {1,1,1,0,0},// >
    {0,0,0,0,1},// EQ
    {0,0,0,0,1},// NEQ
    {0,0,0,0,1},// LEQ
    {0,0,0,0,1} // GEQ
};
int op_arithm_right[NUMBOP][NUM_BASETYPES] = {
    //char,int,float,void,bool
    {1,1,1,0,0},// +
    {1,1,1,0,0},// -
    {1,1,1,0,0},// /
    {1,1,1,0,0},// *
    {1,1,1,0,0},// %
    {1,1,1,0,0},// <
    {1,1,1,0,0},// >
    {0,0,0,0,1},// EQ
    {0,0,0,0,1},// NEQ
    {0,0,0,0,1},// LEQ
    {0,0,0,0,1} // GEQ
};
int op_arithm_result[NUMBOP][NUM_BASETYPES][NUM_BASETYPES] = {
    
    //+
    { //vertical = left; horizontal = right;
        {CHAR,INT,FLOAT,-1,-1},
        {INT,INT,FLOAT,-1,-1},
        {FLOAT,FLOAT,FLOAT,-1,-1},
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1}
    },
    //-
    { //vertical = left; horizontal = right;
        {CHAR,INT,FLOAT,-1,-1},
        {INT,INT,FLOAT,-1,-1},
        {FLOAT,FLOAT,FLOAT,-1,-1},
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1}
    },
    // /
    { //vertical = left; horizontal = right;
        {CHAR,INT,FLOAT,-1,-1},
        {INT,INT,FLOAT,-1,-1},
        {FLOAT,FLOAT,FLOAT,-1,-1},
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1}
    },
    // *
    { //vertical = left; horizontal = right;
        {CHAR,INT,FLOAT,-1,-1},
        {INT,INT,FLOAT,-1,-1},
        {FLOAT,FLOAT,FLOAT,-1,-1},
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1}
    },
    // %
    { //vertical = left; horizontal = right;
        {INT,INT,INT,-1,-1},
        {INT,INT,INT,-1,-1},
        {INT,INT,INT,-1,-1},
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1}
    },
    // <
    { //vertical = left; horizontal = right;
        {BOOL,BOOL,BOOL,-1,-1},
        {BOOL,BOOL,BOOL,-1,-1},
        {BOOL,BOOL,BOOL,-1,-1},
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1}
    },
    // >
    { //vertical = left; horizontal = right;
        {BOOL,BOOL,BOOL,-1,-1},
        {BOOL,BOOL,BOOL,-1,-1},
        {BOOL,BOOL,BOOL,-1,-1},
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1}
    },
    // EQ
    { //vertical = left; horizontal = right;
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,BOOL}
    },
    // NEQ
    { //vertical = left; horizontal = right;
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,BOOL}
    },
    // LEQ
    { //vertical = left; horizontal = right;
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,BOOL}
    },
    // GEQ
    { //vertical = left; horizontal = right;
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,BOOL}
    },
};

int basetype(AST_typeEnum type);
int array(int type);
int array2basetype(int type);
int type2size(int type);
#ifndef
