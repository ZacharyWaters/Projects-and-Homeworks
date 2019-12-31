;;===================================
;; CS 2110 Spring 2018
;; Homework 7 - Functions & Recursion
;; Name:
;;===================================

; See the PDF for details on each function

.orig x3000
; Don't try to run this code directly, since only contains subroutines
; that need to be invoked using the LC-3 calling convention. Use Debug >
; Setup Test or Simulate Subroutine Call in complx instead
;
; Do not remove this line or you will break Simulate Subroutine Call
halt

; mult(a,b): multiply two numbers a and b
;
; pseudocode:
; mult(a, b) {
;     // do work only when a >= b
;     if a < b {
;         mult(b, a)
;     }
;     if b == 0 {
;         return 0
;     }
;     if b == 1 {
;         return a
;     }
;
;     remainder = 0
;     if b & 1 != 0 {
;         remainder = a
;     }
;
;     a = a << 1
;     b = right_shift(b, 1, 1)
;     return mult(a, b) + remainder
; }
mult
    ; callee setup
    add  r6, r6, -1 ; allocate space for return value
    add  r6, r6, -1 ; allocate space for return address
    str  r7, r6, 0  ; back up return address
    add  r6, r6, -1 ; allocate space for old frame pointer
    str  r5, r6, 0  ; back up old frame pointer
    add  r5, r6, -1 ; set frame pointer
    ; back up r0, r1, r2
    add  r6, r6, -2 ; allocate space for 2 local variables, a and remainder
    add  r6, r6, -4 ; allocate space for 4 saved registers
    str  r0, r6, 0  ; back up r0
    str  r1, r6, 1  ; back up r1
    str  r2, r6, 2  ; back up r2
    str  r3, r6, 3  ; back up r3

    ; if a < b {
    ; notice that a < b <-> a - b < 0
    ldr  r0, r5, 4  ; r0 <- a
    ldr  r1, r5, 5  ; r1 <- b
    not  r2, r1     ; r2 <- ~b
    add  r2, r2, 1  ; r2 <- -b
    add  r2, r0, r2 ; r2 <- a - b
    brzp mult_no_swap_args
    ; mult(b, a)
    ; caller setup
    add  r6, r6, -2 ; allocate space for args on stack
    str  r0, r6, 1  ; arg b <- a
    str  r1, r6, 0  ; arg a <- b
    jsr  mult
    ; caller teardown
    ldr  r0, r6, 0  ; r0 <- mult(b,a)
    add  r6, r6, 3  ; pop return value and 2 args off stack
    br   mult_teardown
    ; }
    mult_no_swap_args

    ; if b == 0 {
    add  r1, r1, 0  ; update CC for b
    brnp mult_b_nonzero
    ; return 0
    and  r0, r0, 0  ; r0 <- 0
    br   mult_teardown
    ; }
    mult_b_nonzero

    ; if b == 1 {
    add  r2, r1, -1 ; r2 <- b-1
    ; return a
    ; (r0 already contains a)
    brz  mult_teardown
    ; }

    ; if b & 1 != 0 {
    and  r2, r1, 1  ; r2 <- b & 1
    ; in this case, remainder (r2) == 0
    brz  mult_no_rem
    ; remainder = a
    add  r2, r0, 0  ; r2 <- a
    ; }
    mult_no_rem
    str  r2, r5, -1 ; save remainder to the local variable remainder on the stack

    ; a = a << 1
    add  r0, r0, r0 ; a <- a << 1
    str  r0, r5, 0  ; save a to the local variable a on the stack

    ; b = right_shift(b, 1, 1)
    and  r3, r3, 0  ; r3 <- 0
    add  r3, r3, 1  ; r3 <- 1
    ; caller setup
    add  r6, r6, -3 ; allocate space for args on stack
    str  r1, r6, 0  ; arg n <- b
    str  r3, r6, 1  ; arg k <- 1
    str  r3, r6, 2  ; arg sext <- 1
    jsr  right_shift
    ; caller teardown
    ldr  r1, r6, 0  ; r1 <- right_shift(b, 1, 1)
    add  r6, r6, 4  ; pop return value and 4 args off stack
    ldr  r0, r5, 0  ; reload a from the local variable slot on stack
    ldr  r2, r5, -1 ; reload remainder from the local variable slot on stack

    ; return mult(a, b) + remainder
    ; caller setup
    add  r6, r6, -2 ; allocate space for args on stack
    str  r0, r6, 0  ; arg a <- a
    str  r1, r6, 1  ; arg b <- b
    jsr  mult
    ; caller teardown
    ldr  r0, r6, 0  ; r0 <- mult(a,b)
    add  r6, r6, 3  ; pop return value and 2 args off stack
    add  r0, r0, r2 ; r0 <- mult(a,b) + remainder

    ; callee teardown
    mult_teardown
    str  r0, r5, 3  ; return value <- r0
    ; restore saved registers
    ldr  r0, r6, 0  ; restore r0
    ldr  r1, r6, 1  ; restore r1
    ldr  r2, r6, 2  ; restore r2
    ldr  r3, r6, 3  ; restore r2
    ; restore r5, r7
    add  r6, r6, 4  ; pop r0,r1,r2,r3 off stack
    add  r6, r6, 2  ; pop local variables a and remainder off stack
    ldr  r5, r6, 0  ; restore r5 (old frame pointer)
    add  r6, r6, 1  ; pop old frame pointer
    ldr  r7, r6, 0  ; restore r7, return address
    add  r6, r6, 1  ; pop return address
    ; top of stack is now return value
    ret

;--------------------------------------------------------------------------------------------
rotate_bits
    ; TODO: Implement rotate_bits() here
    ; PSUEDOCODE
    ; n is Saved to Stack 0 
    ; k is Saved to Stack 1 
    ;rotate_bits(n, k) {
    ;    if k <= 0 {
            ;return n
    ;    } else {
            ;msb = n & x8000
            ;n = n + n // n = n << 1
            ;if msb != 0 {
                ;n = n + 1
                ;}
            ;return rotate_bits(n, k-1)
            ;}
    ;}
;HEADER
ADD R6,R6,-4    ; make room for LV,OFP,RA,RV on the stack
STR R7,R6,2     ; Store the Return address
STR R5,R6,1     ; Store the Old frame Pointer
ADD R5,R6,0	; Copy SP to FP

;SAVE REGISTERS
ADD R6,R6,-5    ; makes room for R0,R1,R2,R3, and R4 on the stack
STR R4,R5,-5	; Stores old R4
STR R3,R5,-4	; Stores old R3
STR R2,R5,-3    ; Stores old R2
STR R1,R5,-2    ; Stores old R1
STR R0,R5,-1    ; Stores old R0

Load Parameters to Registers
AND R1,R1,0
LDR R1,R5,4	; Load args 1 (n) in R1
AND R2,R2,0	
LDR R2,R5,5	; Load args 2 (k) in R2

;LOGIC
ADD R2,R2,0
BRP KGREATER	; if k <= 0
STR R1,R5,3	; return n
STR R1, R5, 3	; Stores the answer into the return slot in the stack for future calls
BRNZP CLEANUP

KGREATER 
;CREATING LOCAL VARIABLE MSB
AND R3,R3,0   	; R3 = 0
LD R3,MASK
AND R3,R3,R1	; R3 = msb = n & x8000

ADD R1,R1,R1
ADD R3,R3,0
BRZ MSBISZERO
ADD R1,R1,1

;RECURSE
MSBISZERO ;return rotate_bits(n, k-1)
ADD R2,R2,-1 	; k = k - 1
ADD R6,R6,-1	; Decrement the stack pointer
STR R2,R6,0	; Puts K on the Stack
ADD R6,R6,-1	; Decrement the stack Pointer
STR R1,R6,0	; Puts N on the Stack
JSR rotate_bits	; recalls rotate bits with k-1
LDR R0,R6,0	; Answer is stored in R0
ADD R6,R6,3	; Pops answer, n, and k off the stack
STR R0, R5, 3	; Stores the answer into the return slot in the stack for future calls


;RETURN
CLEANUP


;RESTORING Registers
LDR R0,R5,-1	; Restoring old R0 (#-1) where # is the number of local varaibles
LDR R1,R5,-2    ; Restoring old R1
LDR R2,R5,-3    ; Restoring old R2
LDR R3,R5,-4	; Restoring old R3
LDR R4,R5,-5    ; Restoring old R4

;ClEAN-Up POINTERS
ADD R6,R5,0	; Restore SP
LDR R5,R6,1	; Restore FP
LDR R7,R6,2	; Restore RA
ADD R6,R6,3	; Pop ra,fp,lv1
RET


;--------------------------------------------------------------------------------------------
right_shift_once
;	TODO: Implement right_shift_once() here
;	right_shift_once(n, sext) {
;		msb = n & x8000 // back up old MSB
;		n = rotate_bits(n, 16-1)
;		n = n & x7FFF // clear out MSB
;		if sext != 0 { // set MSB to old MSB if we're sign extending
;			n = n + msb
;			}
;		return n
;		}
;HEADER
ADD R6,R6,-4    	; make room for LV,OFP,RA,RV on the stack
STR R7,R6,2     	; Store the Return address
STR R5,R6,1     	; Store the Old frame Pointer
ADD R5,R6,0    		; Copy SP to FP

;SAVE REGISTERS
ADD R6,R6,-5    	; makes room for R0,R1,R2,R3, and R4 on the stack
STR R4,R5,-5		; Stores old R4
STR R3,R5,-4		; Stores old R3
STR R2,R5,-3    	; Stores old R2
STR R1,R5,-2    	; Stores old R1
STR R0,R5,-1    	; Stores old R0

LOADING PARAMETER N FROM STACK
LDR R1, R5, 4 		;Load args 1 (n) in R1

CREATING LOCAL VARIABLE MSB
LD R2, MASK 		;load R2 with x8000 
AND R3, R1, R2 		;R3 = n & x8000
STR R3, R5, 0 		;puts msb on the stack 
	
;Calling Rotate Bits
AND R4, R4, 0 		;Clear out R4 
ADD R4, R4, 15 		;R4=15 	
ADD R6, R6, -1		;decrements the stack pointer
STR R4, R6, 0 		;puts 15 on the stack 
ADD R6, R6, -1 		;decrements the stack pointer
STR R1, R6, 0 		;puts n on the stack 
JSR rotate_bits 	;calls rotate bits with rotatebits(n,15)
LDR R1, R6, 0 		;stores the answer in R1

;LOGIC
LD R2, MASK2 		;fills in R0 with x7FFF 
AND R1, R2, R1		;n=n & x7FFF
LDR R2, R5, 5 		;fills R2 with sext
BRZ LAST		;if sext != 0
LDR R3, R5, 0 		;fillss R3 with msb
ADD R1, R1, R3 		;add n and msb 

;RETURN
LAST STR R1, R5, 3 	;load answer to rv

;RESTORING Registers
LDR R0,R5,-1		; Restoring old R0 (#-1) where # is the number of local varaibles
LDR R1,R5,-2    	; Restoring old R1
LDR R2,R5,-3    	; Restoring old R2
LDR R3,R5,-4		; Restoring old R3
LDR R4,R5,-5    	; Restoring old R4

;ClEAN-Up POINTERS
ADD R6,R5,0		; Restore SP
LDR R5,R6,1		; Restore FP
LDR R7,R6,2		; Restore RA
ADD R6,R6,3		; Pop ra,fp,lv1
RET
;--------------------------------------------------------------------------------------------
right_shift
    ; TODO: Implement right_shift() here
	;right_shift(n, k, sext) {
		;if k <= 0 {
			;return n
		;} 
		;else {
			;shifted_once = right_shift_once(n, sext)
			;return right_shift(shifted_once, k-1, sext)
		;}
	;}

;HEADER
ADD R6,R6,-4    	; make room for LV,OFP,RA,RV on the stack
STR R7,R6,2     	; Store the Return address
STR R5,R6,1     	; Store the Old frame Pointer
ADD R5,R6,0    		; Copy SP to FP

;SAVE REGISTERS
ADD R6,R6,-5    	; makes room for R0,R1,R2,R3, and R4 on the stack
STR R4,R5,-5		; Stores old R4
STR R3,R5,-4		; Stores old R3
STR R2,R5,-3    	; Stores old R2
STR R1,R5,-2    	; Stores old R1
STR R0,R5,-1    	; Stores old R0

;LOADING PARAMETER k FROM STACK
AND R1,R1,0
LDR R1,R5,4		; Load args 1 (n) in R1
AND R2,R2,0	
LDR R2,R5,5		; Load args 2 (k) in R2

;LOGIC

;FIRST CONDITIONAL
ADD R2,R2,0
BRP KPOSITIVE
STR R1,R5,3	; return n
STR R1, R5, 3	; Stores the answer into the return slot in the stack for future calls
BRNZP FINISH

;SECOND CONDITIONAL
;CALLING RIGHTSHIFT_ONCE
KPOSITIVE
AND R4,R4,0		; Clears out Register 4
LDR R4,R5,6		; Loads args 3 (sext) into R4
ADD R6, R6, -1		; decrements the stack pointer
STR R4, R6, 0 		; puts sext on the stack 
ADD R6, R6, -1 		; decrements the stack pointer
STR R1, R6, 0 		; puts n on the stack 
JSR right_shift_once 	; calls rotate bits with rightshift once with(n,sext)
LDR R3, R6, 0 		; stores the answer(shifted_once) in R3

;CALLING RIGHTSHIFT
ADD R6, R6, -1		; decrements the stack pointer
STR R4, R6, 0 		; puts sext on the stack
ADD R6, R6, -1		; decrements the stack pointer
ADD R2,R2,-1		; k = k - 1
STR R2, R6, 0		; puts k on the stack
ADD R6, R6, -1		; decrements the stack pointer
STR R3,R6,0		; puts shifted_once on the stack
JSR right_shift		; calls right shift with(shifted_once, k-1, sext)
LDR R1, R6, 0 		; stores the answer in R1
;RETURN
FINISH STR R1, R5, 3 	;load answer to rv

;RESTORING Registers
LDR R0,R5,-1		; Restoring old R0 (#-1) where # is the number of local varaibles
LDR R1,R5,-2    	; Restoring old R1
LDR R2,R5,-3    	; Restoring old R2
LDR R3,R5,-4		; Restoring old R3
LDR R4,R5,-5    	; Restoring old R4

;ClEAN-Up POINTERS
ADD R6,R5,0		; Restore SP
LDR R5,R6,1		; Restore FP
LDR R7,R6,2		; Restore RA
ADD R6,R6,3		; Pop ra,fp,lv1
RET
;--------------------------------------------------------------------------------------------
pow
    ; TODO: Implement pow() here
	;pow(n,k) {
		;if k == 0 {
			;return 1
			;}
		;k_halved = right_shift(k, 1, 1) // k_halved = k >> 2
		;halfpow = pow(n, k_halved) // find n^(k/2)
		;product = mult(halfpow, halfpow) // find n^(k/2) * n^(k/2)
		;if (k & 1 != 0) { // if k is odd
			;product = mult(product, n) // find n^(k/2) * n^(k/2) * n
			;}
		;return product
		;}

;HEADER
ADD R6,R6,-4    	; make room for LV,OFP,RA,RV on the stack
STR R7,R6,2     	; Store the Return address
STR R5,R6,1     	; Store the Old frame Pointer
ADD R5,R6,0    		; Copy SP to FP

;SAVE REGISTERS
ADD R6,R6,-5    	; makes room for R0,R1,R2,R3, and R4 on the stack
STR R4,R5,-5		; Stores old R4
STR R3,R5,-4		; Stores old R3
STR R2,R5,-3    	; Stores old R2
STR R1,R5,-2    	; Stores old R1
STR R0,R5,-1    	; Stores old R0

;Load Parameters to Registers
AND R1,R1,0
LDR R1,R5,4		; Load args 1 (n) in R1
AND R2,R2,0	
LDR R2,R5,5		; Load args 2 (k) in R2

;LOGIC
;FIRST CONDITIONAL STATEMENT
ADD R2,R2,0
BRNP NOTZERO		; if(k==0)
AND R0,R0,0
ADD R0,R0,1		; R0 = 1
STR R0,R5,3		; return 1
BRNZP ENDING

NOTZERO

;Right SHift
;CALLING RIGHTSHIFT
AND R0,R0,0
ADD R0,R0,1		; R0 = 1
ADD R6, R6, -1		; decrements the stack pointer
STR R0, R6, 0 		; puts 1 on the stack
ADD R6, R6, -1		; decrements the stack pointer
STR R0, R6, 0 		; puts 1 on the stack
ADD R6, R6, -1		; decrements the stack pointer
STR R2, R6, 0		; puts k on the stack
JSR right_shift		; calls right shift with(k, 1, 1)
LDR R3, R6, 0 		; stores the answer in R3(k_halved)
ADD R6,R6,4		; pops the 3 arguments and return value off the stack

;Calling POW
ADD R6, R6, -1		; decrements the stack pointer
STR R3, R6, 0 		; puts k_halved on the stack
ADD R6, R6, -1		; decrements the stack pointer
STR R1, R6, 0 		; puts n on the stack
JSR pow			; calls pow(n, k_halved)
LDR R4, R6, 0 		; stores the answer in R4(halfpow)
ADD R6,R6,3

;Calling MULT
ADD R6, R6, -1		; decrements the stack pointer
STR R4, R6, 0 		; puts halfpow on the stack
ADD R6, R6, -1		; decrements the stack pointer
STR R4, R6, 0 		; puts halfpow on the stack
JSR mult		; calls mult(halfpow, halfpow)
LDR R4, R6, 0 		; stores the answer in R4(product)
ADD R6,R6,3

;SECOND CONDITIONAL	
;AND R2,R2,1		; Checking if k is ODD
BRZ NOTODD

;CALLING MULT AGAIN
ADD R6, R6, -1		; decrements the stack pointer
STR R1, R6, 0 		; puts n on the stack
ADD R6, R6, -1		; decrements the stack pointer
STR R4, R6, 0 		; puts product on the stack
JSR mult		; calls mult(product, n)
LDR R4, R6, 0 		; stores the answer in R1(product)
ADD R6,R6,3

NOTODD


;RETURN
AND R1,R1,0
ADD R1,R4,R1
STR R1, R5, 3 		;load answer to rv

ENDING 
;RESTORING Registers
LDR R0,R5,-1		; Restoring old R0 (#-1) where # is the number of local varaibles
LDR R1,R5,-2    	; Restoring old R1
LDR R2,R5,-3    	; Restoring old R2
LDR R3,R5,-4		; Restoring old R3
LDR R4,R5,-5    	; Restoring old R4

;ClEAN-Up POINTERS
ADD R6,R5,0		; Restore SP
LDR R5,R6,1		; Restore FP
LDR R7,R6,2		; Restore RA
ADD R6,R6,3		; Pop ra,fp,lv1
RET
;------------------------------------------------------------------------------------------
; Needed by Simulate Subroutine Call in complx
MASK  .fill x8000
MASK2 .fill x7FFF
STACK .fill xF000
.end

;--------------------------------------------------------------------------------------
;;STARTING
    ; TODO: Implement bubble_sort() here
    ADD R6, R6, -4       ; Make room for RV, RA, and OFP on STACK
    STR R7, R6, 2       	; Store RA (return address)
    STR R5, R6, 1       	; Store OFP (old frame pointer)
    ADD R5, R6, 0       	; Set new FP
    ADD R6, R5, 0		; SP = FP
    
;pick one of these depending on the amount of variables
    ;ADD R6, R6, -5      ; Make room for Local Var (1 Locals)
    ;ADD R6, R6, -6      ; Make room for Local Var (2 Locals)
    ;ADD R6, R6, -7      ; Make room for Local Var (3 Locals)
    ;ADD R6, R6, -8      ; Make room for Local Var (4 Locals)

;pick one of these when storing registers in local variables 
    ;STR BR, R5, 0	   ; Stores Br in Local Variable 1
    ;STR BR, R5, -1	   ; Stores Br in Local Variable 2
    ;STR BR, R5, -2	   ; Stores Br in Local Variable 3
    ;STR BR, R5, -3	   ; Stores Br in Local Variable 4


;pick one of these when extracting local variables
    ;LDR DR, R5, 0	   ; Gets the very first local variable 								;Stores it in DR
    ;LDR DR, R5, -1	   ; Gets the second local variable 
    ;LDR DR, R5, -2	   ; Gets the third local variable 
    ;LDR DR, R5, -3	   ; Gets the fourth local variable 


    ;Save registers to make room to use reg
    ;STR R0, R5, -4      	; Store old R0
    ;STR R1, R5, -5      	; Store old R1
    ;STR R2, R5, -6		; Store old R2
    ;STR R3, R5, -7		; Store old R3
    ;STR R4, R5, -8		; Store old R4

    ;Storing Parameters in Registers
    ;STR R0, R5, 4      	; Store first parameter in R0 Left most parameter
    ;STR R0, R5, 5      	; Store first parameter in R0 ect

	
	;Clearing registers to begin code
    ;AND R0,R0,0			;clears R0
    ;AND R1,R1,0			;clears R1
    ;AND R2,R2,0			;clears R2
    ;AND R3,R3,0			;clears R3
    ;AND R4,R4,0			;clears R4

	;Calling Another Method
    ;CALLING RIGHTSHIFT
    ;AND R0,R0,0
    ;ADD R0,R0,1			; R0 = 1
    ;ADD R6, R6, -1		; decrements the stack pointer
    ;STR R0, R6, 0 		; puts 1 on the stack
    ;ADD R6, R6, -1		; decrements the stack pointer
    ;STR R0, R6, 0 		; puts 1 on the stack
    ;ADD R6, R6, -1		; decrements the stack pointer
    ;STR R2, R6, 0		; puts k on the stack
    ;JSR right_shift		; calls right shift with(k, 1, 1)
    ;LDR R3, R6, 0 		; stores the answer in R3(k_halved)
    ;ADD R6,R6,4			; pops the 3 arguments and return 	
						value off the stack		
	
	;Example for loop
	;;;;		for (int i = 0; i < LENGTH; i++) {
	;;BELOW IT FILLS R1, WITH 0, THE VALUE OF I
		;AND R1,R1,0

	;;BELOW IT CHECKS IF I(R6) IS LESS THAN LENGTH
		;LOOP
	;;BELOW IT FILLS R2 WITH THE VALUE OF LENGTH
		;AND R2,R2,0
		;LD R2,LENGTH
	;;BELOW IT CALCULATES THE ANSWER IN R3
		;AND R3,R3,0
		;ADD R3,R3,R1
		;NOT R3,R3
		;ADD R3,R3,1
		;ADD R3,R1,R2
		;BRNZ LESSTHANLENGTH
	
	;;Ending Code
	
	;RETURN
;LAST STR R1, R5, 3 	;load answer to rv
	
	;RESTORING Registers
;LDR R0,R5,-1	; Restoring old R0 (#-1) where # is the number of 			; local varaibles
;LDR R1,R5,-2    ; Restoring old R1
;LDR R2,R5,-3    ; Restoring old R2
;LDR R3,R5,-4	; Restoring old R3
;LDR R4,R5,-5    ; Restoring old R4

;ClEAN-Up POINTERS
;ADD R6,R5,0	; Restore SP
;LDR R5,R6,1	; Restore FP
;LDR R7,R6,2	; Restore RA
;ADD R6,R6,3	; Pop ra,fp,lv1(add one to 3 for every local variable past the first the you use)
;RET

