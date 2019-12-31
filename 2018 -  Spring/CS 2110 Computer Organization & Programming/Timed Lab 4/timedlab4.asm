;; ====================================================
;; CS 2110 Spring 2018
;; Timed Lab 4 - Reverse Array and Count Odd Elements
;; ====================================================
;; Name:
;; ====================================================

.orig x3000

;; Don't try to run this code directly, since only contains subroutines
;; that need to be invoked using the LC-3 calling convention. Use Debug >
;; Setup Test or Simulate Subroutine Call in complx instead
;;
;; Do not remove this line or you will break Simulate Subroutine Call

HALT

;; The pseudocode is posted here for your reference:
;;
;; reverse_odd(arr, i, j)
;; {
;;   odd_count = (arr[i] & 0x1)
;;
;;   if (i > j) {
;;     return 0;
;;   } else if (i < j) {
;;
;;     temp   = arr[i];
;;     arr[i] = arr[j];
;;     arr[j] = temp;
;;
;;     odd_count = odd_count + (arr[i] & 0x1);
;;   }
;;
;;   return reverse_odd(arr, i + 1, j - 1) + odd_count;
;; }

reverse_odd

;; YOUR CODE HERE! :D
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
LDR R1,R5,4		; Load args 1 (arr) in R1
AND R2,R2,0	
LDR R2,R5,5		; Load args 2 (i) in R2
AND R3,R2,0	
LDR R3,R5,6		; Load args 2 (j) in R3

;LOGIC
AND R0,R0,0		;R0 will be Oddcount

ADD R2,R1,R2		;R2 is now equal to array+i
AND R4,R4,0			
LDR R4,R2,0		;R4 is now equal to array[i]

AND R0,R4,1		;ODDCOUNT=array[i]&1

NOT R3,R3
ADD R3,R3,1		;Makes -j in R3
LDR R2,R5,5		; Load args 2 (i) in R2   //****adding this line improves grade to 70
ADD R3,R3,R2		
BRNZ Jgreater		;checks if (i > j)

AND R1,R1,0		
STR R1,R5,3		; Stores 0 into the return slot.
BRNZP FINISH

Jgreater


ADD R2,R1,R2					//****adding this line improves grade to 90
AND R3,R3,0	
LDR R3,R5,6		; Load args 2 (j) in R3
ADD R3,R3,R1		; R3 is now equal to array+j
LDR R3,R3,0		; R3 is now equal to array[j]
STR R3,R2,0		; puts array[j] at array+i
	
AND R3,R3,0	
LDR R3,R5,6		; Load args 2 (j) in R3
ADD R3,R3,R1		; R3 is now equal to array+j
STR R4,R3,0		; puts array[i] at array+j

AND R4,R4,1		; ands array[i]&1
ADD R0,R4,R0		; ODDCOUNT = ODDCOUNT + array[i]&1

AND R2,R2,0	
LDR R2,R5,5		
ADD R2,R2,1		;Load param (i+1) in R2
AND R3,R2,0	
LDR R3,R5,6		
ADD R3,R3,-1		; Load param (j-1) in R3


;Calling odd_count
ADD R6, R6, -1		; decrements the stack pointer
STR R3, R6, 0 		; puts j-1 on the stack
ADD R6, R6, -1		; decrements the stack pointer
STR R2, R6, 0 		; puts i+1 on the stack
ADD R6, R6, -1		; decrements the stack pointer
STR R1, R6, 0		; puts arr on the stack
JSR reverse_odd		; calls reverseodd(arr,i+1,j-1)
AND R4,R4,0
LDR R4, R6, 0 		; stores the answer in R4(odd_count)
ADD R6,R6,4		; pops the 3 arguments and return value off the stack	
ADD R0,R4,R0		; ODDCOUNT = ODDCOUNT + ODDCOUNT
FINISH

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










STACK .fill xF000
ARRAY .blkw 1

.end

;; ====================================================
;; The array to be reversed. USED ONLY FOR DEBUGGING.
;; ====================================================

.orig x6000
.fill 9
.fill 12
.fill 5
.fill 3
.fill 17
.fill 8
.fill 2
.fill 78
.fill 34
.fill 13
.end
