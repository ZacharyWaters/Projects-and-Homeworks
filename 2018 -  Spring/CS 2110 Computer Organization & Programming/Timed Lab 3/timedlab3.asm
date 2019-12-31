;; Spring 2018 - Timed Lab 3
;; ======================================
;;  Name: 
;; ======================================

.orig x3000

;; 	if (LENGTH == 0) {
;;		RANGE = 0
;;	} else {
;;		int max = ARRAY[0];
;;		int min = ARRAY[0];
;;		for (int i = 0; i < LENGTH; i++) {
;;			int x = ARRAY[i];
;;			if (x < min) {
;;				min = x;
;;				}
;;			if (x > max) {
;;				max = x;
;;				}
;;			}
;;		RANGE = max - min;
;;		}

;;BELOW it FILLS R0 with the RANGE of the ARRAy
AND R0,R0,0
;;BELOW IT FILLS R1 with the LENGTH OF THE ARRAY
AND R1,R1,0
LD R1,LENGTH
BRNZ END

;;BELOW IT FILLS R3, WITH THE LOCATION OF THE ARRAY(x6000)
AND R3,R3,0
LD R3, ARRAY

;;BELOW IT FILLS R4, WITH THE VALUE OF MIN
AND R4,R4,0
LDR R4,R3,0
;;BELOW IT FILLS R5, WITH THE VALUE OF MAX
AND R5,R5,0
LDR R5,R3,0

;;FOR LOOP(int i = 0; i < LENGTH; i++) {

;;BELOW IT FILLS R6, WITH THE VALUE OF I
AND R6,R6,0

;;BELOW IT CHECKS IF I(R6) IS LESS THAN LENGTH
LOOP
AND R1,R1,0
LD R1,LENGTH
AND R7,R7,0
ADD R7,R7,R6
NOT R7,R7
ADD R7,R7,1
ADD R7,R7,R1
BRNZ LESSTHANLENGTH

;;enters the FOR LOOP
;;			int x = ARRAY[i];
;;			if (x < min) {
;;				min = x;
;;				}
;;			if (x > max) {
;;				max = x;
;;				}
;;			
;;			}

;;ADDS I TO THE LOCATION OF THE ARRAY Sets it equal to value x(R8)
LD R3, ARRAY
ADD R3,R3,R6
AND R7,R7,0
LDR R7,R3,0

;;BELOW IT CHECKS IF MIN IS GREATER THAN X
AND R1,R1,0
NOT R7,R7
ADD R7,R7,1
ADD R1,R7,R4
NOT R7,R7
ADD R7,R7,1
ADD R1,R1,0
BRNZ XBIGGERTHANMIN

;;BELOW SETS MIN=x
AND R4,R4,0
ADD R4,R4,R7

;;BELOW IT CHECKS IF MAX IS LESS THAN X
XBIGGERTHANMIN
AND R1,R1,0
NOT R5,R5
ADD R5,R5,1
ADD R1,R7,R5
NOT R5,R5
ADD R5,R5,1
ADD R1,R1,0
BRNZ XLESSTHANMAX

;;BElOW IT SETS MAX = X
AND R5,R5,0
ADD R5,R5,R7

XLESSTHANMAX
;;ADDS ONE TO I AND LOOPS BACK
ADD R6,R6,1
BRNZP LOOP

;;CALCULATED RANGE AND ENDS
LESSTHANLENGTH
NOT R4,R4
ADD R4,R4,1
ADD R0,R4,R5

;END Fills RANGE WITH THE CURRENT VALUE OF R0
END
ST R0,RANGE


HALT

ARRAY .fill x6000
LENGTH .fill 7
RANGE .fill -1

.end

.orig x6000
.fill 10
.fill 7
.fill 67
.fill 90
.fill 3
.fill 2
.fill 45
.end
