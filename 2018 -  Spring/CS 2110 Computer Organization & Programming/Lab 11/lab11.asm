; ==========================
; CS2110 Lab 11 Spring 2018
; Name: YOUR NAME HERE
; ==========================

.orig x40
.fill x5000


.end

.orig x3000

	; This is main. Do not modify it!

	LD R0, STRING
	TRAP x40
	PUTS

	HALT ; After halting, R0 - R6 should still contain their original values

STRING	.fill x4000
.end

.orig x4000
	.stringz "Hello, World! 123456789 !@#$%^&*"
.end

.orig x5000

	; TODO implement your trap here


	; TODO you might need some labels here to hold temporary values
	;void lower(char *str) {
	;    char c = str[0];
	;    while(c != 0) {
	;        if(c >= 'A' && c <= 'Z') {
	;            c += 32;
	;            str[0] = c;
	;	}
	;        string++;
	;        c = str[0];
	;    }
	;}
ST R0,LABEL0
ST R1,LABEL1
ST R2,LABEL2
ST R3,LABEL3
ST R4,LABEL4
ST R5,LABEL5
LOOP
LDR R1,R0,0
BRZ END 

AND R3,R3,0
ADD R3,R3,-16
ADD R3,R3,-16
ADD R3,R3,-16
ADD R3,R3,-16
ADD R3,R3,-1
ADD R3,R3,R1
BRN LESSTHAN

AND R3,R3,0
ADD R3,R3,15
ADD R3,R3,15
ADD R3,R3,15
ADD R3,R3,15
ADD R3,R3,15
ADD R3,R3,15
NOT R4,R1
ADD R4,R4,1
ADD R4,R3,R4
BRN GREATERTHAN

ADD R1,R1,15
ADD R1,R1,15
ADD R1,R1,2
STR R1,R0,0
BRNZP RESET


RESET
LESSTHAN
GREATERTHAN
ADD R0,R0,1
BRNZP LOOP

LABEL0 .fill 0
LABEL1 .fill 0
LABEL2 .fill 0
LABEL3 .fill 0
LABEL4 .fill 0
LABEL5 .fill 0

END 
LD R0,LABEL0
LD R1,LABEL1
LD R2,LABEL2
LD R3,LABEL3
LD R4,LABEL4
LD R5,LABEL5
RET
.end
