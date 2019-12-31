;;======================================================================
;; CS2110                    Lecitation #10                  Spring 2018
;;======================================================================
;; Author:
;; Date:
;; Assignment: Recursive Function in Assembly
;; Description: An assembly program that sums digits in a decimal number
;;======================================================================

;@plugin filename=lc3_udiv vector=x80

;; Main
;; Do not edit this function!

.orig x3000

;; Initialize stack
LD R6, STACK

;; Call digisum(n)
LD R0, N
ADD R6, R6, -1
STR R0, R6, 0
JSR DIGISUM

;; Pop return value and arg off stack
LDR R0, R6, 0
ADD R6, R6, 2

;; Save the answer
ST R0, ANSWER

HALT

STACK    .fill xF000
N        .fill 45
ANSWER   .blkw 1

;;=====================================
;; Pseudocode
;;=====================================
;;int digisum(int n) {
;;    int div = n / 10;
;;    int mod = n % 10;
;;    if(n == 0) {
;;        return 0;
;;    } else {
;;        return mod + digisum(div);
;;    }
;;}

;; To call UDIV, use TRAP x80
;; Preconditions:
;;    R0 = X
;;    R1 = Y
;; Postconditions:
;;    R0 = X / Y
;;    R1 = X % Y

DIGISUM

;; CODE GOES HERE :D

RET

.end
