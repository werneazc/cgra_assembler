#Kernel of a 3x3 Convolution
VAR IAddr 0
VAR OAddr 2048
VAR place 1
#Load coefficients to VCGRA data input cache
LOOP 0 9 1 
    LOADD IAddr 0 place
    ADDI IAddr 2
    ADDI place 2
POOL
VAR IAddr 0
VAR place 0
LOOP 0 9 1 
    LOADD IAddr 1 place
    ADDI IAddr 2
    ADDI place 2
POOL
#Load first nine input values to data input cache
VAR place 1
LOOP 0 9 1 
    LOADD IAddr 0 place
    ADDI IAddr 2
    ADDI place 2
POOL
#Iterate over left input values
LOOP 9 512 18
    SLT_DOC_LINE 0
    START
#Load values to VCGRA
    VAR place 1
    SLT_DIC_LINE 1
    LOOP 0 9 1
        LOADD IAddr 1 place
        ADDI IAddr 2
        ADDI place 2
    POOL
    WAIT_READY
    STORED OAddr 0 0
    SLT_DOC_LINE 1
    START
    ADDI OAddr 2
    SLT_DIC_LINE 0
    VAR place 1
    LOOP 0 9 1
        LOADD IAddr 0 place
        ADDI IAddr 2
        ADDI place 2
    POOL
    WAIT_READY
    STORED OAddr 1 0
    ADDI OAddr 2
POOL
FINISH
