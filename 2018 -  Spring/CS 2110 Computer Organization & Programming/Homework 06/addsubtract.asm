;;===============================
;; Name: 
;;===============================

.orig x3000

;;Filled R3 with a counter starting at 6000
AND R3,R3,0
LD R3, ARRAY

;;Filled in R5 with 0, this will be the SUM counter
AND R5,R5,0

;;BELOW IT FILLS R1 with the LENGTH OF THE ARRAY
AND R1,R1,0
LD R1,LENGTH
LOOP BRNZ END

;;Filled R4, with the value of the ARRAY and incriments the COUNTER
AND R4,R4,0
LDR R4,R3,0
ADD R3,R3,1

;;BELOW CHECK if R3 contains an Even or ODD location
AND R6,R3,1
BRP EVEN

;;Below is the checker for ODD
NOT R4,R4
ADD R4,R4,1
ADD R5,R5,R4
ADD R1,R1,-1
BRNZP LOOP


;;Below is the checker for EVEN
EVEN ADD R5,R5,R4 
ADD R1,R1,-1
BRNZP LOOP
END ST R5,RESULT
HALT


ARRAY .fill x6000
LENGTH .fill 7
RESULT .fill 0
.end

.orig x6000
.fill 3 ; +
.fill 5 ; -
.fill 1 ; +
.fill 4 ; -
.fill 6 ; +
.fill 8 ; -
.fill 12; +
.end
