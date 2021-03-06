;;===============================
;; Name: Zach Waters
;;===============================

.orig x3000

AND R1,R1,0
LD R1,LENGTH

AND R6,R6,0
ADD R6,R6,R1
ADD R6,R6,-2
BRZ ZEROLENGTH

AND R3,R3,0
LD R3,STRING
ADD R3,R3,R1
ADD R3,R3,-2

AND R0,R0,0
LDR R0,R3,0

AND R4,R4,0
LD R4,STORAGE

RESTART STR R0,R4,0
ADD R4,R4,1
ADD R3,R3,-1
LDR R0,R3,0
ADD R1,R1,-1
BRP RESTART

AND R1,R1,0
LD R1,LENGTH
ADD R1,R1,-1

AND R3,R3,0
LD R3,STRING

AND R4,R4,0
LD R4,STORAGE

AND R0,R0,0
LDR R0,R4,0

LOOP STR R0,R3,0
ADD R4,R4,1
LDR R0,R4,0
ADD R3,R3,1
ADD R1,R1,-1
BRP LOOP
AND R0,R0,0
LD R0,STRING
PUTS
BRNZP END

ZEROLENGTH AND R0,R0,0

END HALT

STORAGE .fill x4000
STRING .fill x6000
LENGTH .fill 21

.end

.orig x6000
.stringz "To Hell With Georgia\n"
.end
