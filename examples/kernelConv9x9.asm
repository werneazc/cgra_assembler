#Addresses of configuration bitstreams
CONST pcaddr1 0x00
CONST pcaddr2 0x40
CONST ccaddr1 0x80
CONST ccaddr2 0x120

#Addresses for temporary values
CONST temp1 0x160
CONST temp2 0x162

#Addresses for coefficients
CONST coeff0 0x170
CONST coeff1 0x172
CONST coeff2 0x174
CONST coeff3 0x176
CONST coeff4 0x178
CONST coeff5 0x17a
CONST coeff6 0x17c
CONST coeff7 0x17e
CONST coeff8 0x180

#Pixel input region
CONST inputBegin 0x200

#Pixel output region
CONST outputBegin 0x300

#Constants
CONST pixelSize 2
CONST coeffSize 2
CONST pixWidth 12
CONST pixHeight 12

#Prepare configuration prefetcher
SLCT_PECC_LINE 1
SLCT_CHCC_LINE 1
LOADCC ccaddr1 0
LOADPC pcaddr1 0
SLCT_PECC_LINE 0
SLCT_CHCC_LINE 0
LOADCC ccaddr2 1
LOADPC pcaddr2 1

#Load coefficients to VCGRA data input cache
VAR caddr coeff0
SLCT_DIC_LINE 1
VAR place 0
LOOP 0 4 1 
    LOADD caddr 0 place
    ADDI place 2
    ADD caddr coeffSize
POOL
SLCT_DIC_LINE 0
VAR place 0
VAR caddr coeff4
LOOP 0 4 1 
    LOADD caddr 1 place
    ADDI place 2
    ADD caddr coeffSize
POOL

VAR outPixWidth pixWidth
SUBI outPixWidth 1
VAR endPixWidth pixWidth
SUBI endPixWidth 2
VAR outPixHeight pixHeight
SUBI outPixHeight 1
VAR endPixHeight pixWidth
SUBI endPixHeight 2

VAR startIn pixWidth
ADDI startIn 1

#variable definition for variables within the loop
VAR oaddr outputBegin
VAR rowOutCount 0
VAR rowOut 0
VAR columnOutCount 0
VAR filterpoint pixWidth
VAR rowInputCount 1
VAR count -1
VAR temp 0
VAR outOffset 0

LOOP 1 endPixHeight 1
    VAR columnOutCount 0
    VAR filterpoint pixWidth
    MUL filterpoint rowInputCount

    LOOP 1 endPixWidth 1
        SLCT_DIC_LINE 1
        SLCT_DOC_LINE 0
        SLCT_CHCC_LINE 0
        SLCT_PECC_LINE 0

        #Update filter coefficients
        VAR caddr coeff0
        VAR place 0
        LOOP 0 4 1 
            LOADD caddr 0 place
            ADDI place 2
            ADD caddr coeffSize
        POOL

        ADDI filterpoint 1
        VAR place 1
        VAR count -1
        #Pix 0 ... Pix 2
        VAR temp 0
        LOOP 0 3 1
            VAR temp filterpoint
            SUB temp pixWidth
            ADD temp count
            MUL temp pixelSize
            ADD temp inputBegin
            LOADD temp 0 place
            ADDI count 1
            ADDI place 2
        POOL
        #Pix 3
        VAR temp filterpoint
        SUBI temp 1
        MUL temp pixelSize
        ADD temp inputBegin
        LOADD temp 0 place
        SLCT_DIC_LINE 0
        SLCT_DOC_LINE 0
        START
        #Pix 4
        VAR place 1
        VAR temp filterpoint
        MUL temp pixelSize
        ADD temp inputBegin
        LOADD temp 1 place
        ADDI place 2
        #Pix 5
        VAR temp filterpoint
        ADDI temp 1
        MUL temp pixelSize
        ADD temp inputBegin
        LOADD temp 1 place
        ADDI place 2
        #Pix 6 ... Pix 7
        VAR count -1
        LOOP 0 2 1
            VAR temp filterpoint
            ADD temp pixWidth
            ADD temp count
            MUL temp pixelSize
            ADD temp inputBegin
            LOADD temp 1 place
            ADDI count 1
            ADDI place 2
        POOL
        WAIT_READY
        SLCT_DIC_LINE 1
        SLCT_DOC_LINE 1
        START
        STORED temp1 0 2
        LOADD coeff8 0 0
        #Pix 8
        VAR temp filterpoint
        ADD temp pixWidth
        ADDI temp 1
        MUL temp pixelSize
        ADD temp inputBegin
        LOADD temp 0 1
        LOADD temp1 0 2
        WAIT_READY
        STORED temp2 1 2
        LOADD temp2 0 3
        SLCT_DIC_LINE 0
        SLCT_DOC_LINE 0
        SLCT_CHCC_LINE 1
        SLCT_PECC_LINE 1
        START
        WAIT_READY

        VAR oaddr outputBegin
        VAR outOffset rowOut
        ADD outOffset columnOutCount
        MUL outOffset pixelSize
        ADD oaddr outOffset
        STORED oaddr 0 2

        ADDI columnOutCount 1
    POOL

    ADDI rowOutCount 1
    ADDI rowInputCount 1
    VAR rowOut rowOutCount
    MUL rowOut outPixWidth

POOL
FINISH

