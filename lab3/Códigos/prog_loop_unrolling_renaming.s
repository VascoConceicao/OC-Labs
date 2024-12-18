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

loop:       lw $10, 0($2)   ; A[i]
            lw $11, 0($3)   ; B[i]
            lw $20, 8($2)   ; A[i+1]
            lw $21, 8($3)   ; B[i+1]

            daddi $11, $11, 1
            daddi $21, $21, 1

            dmul $12, $10, $11
            dmul $22, $20, $21

            daddi $4, $4, 16

            daddi $1, $1, 2
            daddi $2, $2, 16
            daddi $3, $3, 16

            sw $12, -16($4)
            sw $22, -8($4)

            bne $1, $5, loop

            halt
