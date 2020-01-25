CONST myConst 10
VAR myVar 1
LOOP 0 5 1
    VAR myVar2 myConst
    LOOP 0 myConst myVar
        ADDI myVar2 2
        SLCT_DIC_LINE 0
        LOADPC myConst 2
        LOADCC 80 myVar
    POOL
POOL
LOADD myVar 0 181
