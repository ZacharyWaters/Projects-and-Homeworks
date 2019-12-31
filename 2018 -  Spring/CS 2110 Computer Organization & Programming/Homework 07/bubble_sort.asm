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

bubble_sort
    ; TODO: Implement bubble_sort() here
;	bubble_sort(array, n) {
;		if (n <= 1) {
;			return 0
;			}
;		swapCount = 0
;		for (i = 0; i < n-1; i++) {
;			if (array[i] > array[i + 1]) {
;				temp = array[i];
;				array[i] = array[i + 1]
;				array[i + 1] = temp
;				swapCount++
;				}
;			}
;	return swapCount + bubble_sort(array, n-1)
;	}
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
LDR R1,R5,4	; Load args 1 (array) in R1
AND R2,R2,0	
LDR R2,R5,5	; Load args 2 (n) in R2

;LOGIC

;First CONDITIONAL
ADD R0,R2,-1
BRP NGREATER
AND R0,R0,0
STR R0,R5,3	; return 0
BRNZP FINALE

NGREATER
AND R0,R0,0	; swapCount = 0
AND R3,R3,0	; i = 0
ADD R2,R2,-1	; R2 = n - 1
NOT R2,R2	
ADD R2,R2,1	; R2 = - (n - 1)
AND R4,R4,0
ADD R4,R2,R3	; i < n-1
BRZP LOOPFAIL

LOOPFAIL
;	return swapCount + bubble_sort(array, n-1)
;CALLING bubble_sort
ADD R6, R6, -1		; decrements the stack pointer
NOT R2,R2
ADD R2,R2,1		;Converts -(n-1) to (n-1)
STR R2, R6, 0 		; puts n-1 on the stack
ADD R6, R6, -1		; decrements the stack pointer
STR R1, R6, 0 		; puts array on the stack
JSR bubble_sort		; calls bubble_sort(array, n-1)
LDR R0, R6, 0 		; stores the answer in R0
STR R0,R5,3		; puts the answer on the stack
BRNZP FINALE

;RETURN

FINALE


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


; Needed by Simulate Subroutine Call in complx
STACK .fill xF000
ARRAY .blkw 1
.end

;;=====================================
;; The array to be sorted. USED ONLY FOR DEBUGGING
;; When using Simulate Subroutine Call, set params
;; to "x6000, 10" to execute the function with this
;; array
;;=====================================

.orig x6000
.fill 10
.fill -8
.fill 105
.fill 4
.fill 0
.fill -54
.fill 12
.fill 76
.fill -1
.fill 34
.end
-----------------------------------------
;;===================================
;; CS 2110 Spring 2018
;; Homework 7 - Functions & Recursion
;; Name: Kevin Vu Nguyen GTID: 903128265
;;===================================

; See the PDF for details on each function

.orig x3000
; Don't try to run this code directly, since only contains subroutines
; that need to be invoked using the LC-3 calling convention. Use Debug >
; Setup Test or Simulate Subroutine Call in complx instead
;
; Do not remove this line or you will break Simulate Subroutine Call
halt

bubble_sort
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
;ADD R6,R6,3	; Pop ra,fp,lv1
;RET
	;Beginning of Code
    LDR R0, R5, 4		;Beginning of Array
    LDR R4, R5, 4
    ADD R4, R4, 1		;Beginning of Array + 1

    LDR R1, R5, 5		;n = value
    ADD R1, R1, -1
    BRP SWAPS

    AND R1, R1, 0		;n <= 1
    STR R1, R5, 0		;0 Swaps
    BR END

SWAPS

	AND R2, R2, 0		;Swap Counter = 0
	STR R2, R5, 0		;Store counter in Local var

	LDR R1, R5, 5
	ADD R1, R1, -1		;Take N and subtract 1
	STR R1, R5, -1		;N - 1 in memory

	WHILE
		LDR R1, R5, -1  ;LD N-1 and leave if (N - 1) = 0
		BRNZ LEAVELOOP

		ADD R1, R1, -1	;Decrement and Store n
		STR R1, R5, -1	

		AND R1, R1, 0	;Clear R1, R2 for Use
		AND R2, R2, 0

		LDR R1, R0, 0 	;Current i Index
		LDR R2, R0, 1	;i + 1 Index

		NOT R2, R2
		ADD R2, R2, 1	;2's complement of Array[i + 1]
		ADD R2, R1, R2	;Check if Array[i] > [i + 1]
		BRNZ INCREMENT

		LDR R2, R4, 0	;Reload i + 1 index
		LDR R1, R0, 0	;Temp Array[i]
		STR R2, R0, 0
		STR R1, R4, 0	;Swap

		AND R3, R3, 0	;Clear R3
		LDR R3, R5, 0 	;Increment Swap
		ADD R3, R3, 1
		STR R3, R5, 0

		INCREMENT
			ADD R0, R0, 1
			ADD R4, R4, 1
			BR WHILE

LEAVELOOP
	LDR R1, R5, 5		;Decrement N = N-1
	ADD R1, R1, -1
	ADD R6, R6, -2		;Make room on Stack for Args
	AND R2, R2, 0
	LDR R2, R5, 4
	STR R2, R6, 0		;bubbleSort(r3.array, r1.n-1)
	STR R1, R6, 1	
	JSR bubble_sort

	LDR R0, R6, 0
	ADD R6, R6, 3
	LDR R1, R5, 0
	ADD R0, R1, R0		;Swapcount + bubblesort(array, n - 1)
	STR R0, R5, 0

END NOP

RETURN
	;Cleaning up and return Answer
	LDR R0, R5, 0		;Return Final Answer
	STR R0, R5, 3		

	;Restore Old Reg Values
	LDR R0, R5, -4      ; ReStore old R0
    LDR R1, R5, -5      ; ReStore old R1
    LDR R2, R5, -6		; ReStore old R2
    LDR R3, R5, -7		; ReStore old R3
    LDR R4, R5, -8		; ReStore old R4

    ADD R6, R5, 0		;R6 = Move SP to Current FP
    LDR R5, R6, 1		;R5 = Restoring FP (to Old FP)
    LDR R7, R6, 2		;R7 = Restore Returning Add
    ADD R6, R6, 3		;Move SP To return Value
    RET




; Needed by Simulate Subroutine Call in complx
STACK .fill xF000
ARRAY .blkw 1
.end

;;=====================================
;; The array to be sorted. USED ONLY FOR DEBUGGING
;; When using Simulate Subroutine Call, set params
;; to "x6000, 10" to execute the function with this
;; array
;;=====================================

.orig x6000
.fill 10
.fill -8
.fill 105
.fill 4
.fill 0
.fill -54
.fill 12
.fill 76
.fill -1
.fill 34
.end

