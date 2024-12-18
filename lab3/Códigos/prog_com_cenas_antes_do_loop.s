            .data
A:          .word 1,2,3,4,5
            .word 6,7,8,9,10
            .word 11,12,13,14,15
            .word 16
B:          .word 11,22,33,44,55
            .word 66,77,88,99,100
            .word 111,122,133,144,155
            .word 166
C:          .word 0,0,0,0
            .word 0,0,0,0
            .word 0,0,0,0
            .word 0,0,0,0

            .code
            daddi $1, $zero, 0 ; i = 0
            daddi $5, $zero, 16 ; value of N
            daddi $2, $zero, A
            daddi $3, $zero, B
            daddi $4, $zero, C

            lw $10, 0($2)   ; A[i]
            lw $11, 0($3)   ; B[i]

            daddi $4, $4, 16
            daddi $11, $11, 1

loop:       dmul $12, $10, $11

            daddi $1, $1, 2
            daddi $2, $2, 16
            daddi $3, $3, 16

            lw $10, -8($2)
            lw $11, -8($3)

            sw $12, -16($4)

            daddi $11, $11, 1

            dmul $12, $10, $11

            lw $10, 0($2)   ; A[i]
            lw $11, 0($3)   ; B[i]
            
            daddi $4, $4, 16
            daddi $11, $11, 1

            sw $12, -24($4)

            bne $1, $5, loop

            halt
