;;===============================
;; Name:Zachary Waters 
;;===============================
.orig x3000
;;Filled R3 with a counter starting at 6000
AND R3,R3,0
LD R3, ARRAY

;;BELOW IT FILLS R1 with the LENGTH OF THE ARRAY
AND R1,R1,0
LD R1,LENGTH
LOOP BRNZ END

;;Filled R4, with the value of the ARRAY and incriments the COUNTER
AND R4,R4,0
LDR R4,R3,0
ADD R4,R4,0
BRNP NOTZERO

ADD R3,R3,1
AND R0,R0,0 
LEA R0, Z 
PUTS
ADD R1,R1,-1
BRNZP LOOP

NOTZERO ADD R3,R3,1
;;BELOW CHECK if R4 contains an EVEN OR ODD VALUE
CHECK AND R4,R4,1
BRZ EVEN

;;Below is the checker for ODD
AND R0,R0,0
LEA R0, O
PUTS
ADD R1,R1,-1
BRNZP LOOP

;;Below is the checker for EVEN
EVEN AND R0,R0,0 
LEA R0, E 
PUTS
ADD R1,R1,-1
BRNZP LOOP



END HALT
E .stringz "EVEN\n"
O .stringz "ODD\n"
Z .stringz "ZERO\n"
LENGTH .fill 5
ARRAY .fill x6000

.end

.orig x6000
.fill 0
.fill 9
.fill 10
.fill 0
.fill 14
.end
