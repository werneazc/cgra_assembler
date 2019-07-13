CONST myConst 10
VAR myVar 1
LOOP 0 5 1
    VAR myVar2 myConst
    LOOP 0 myConst myVar
        ADDI myVar2 2
    POOL
POOL
