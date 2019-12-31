;;===============================
;; Name: 
;;===============================
.orig x3000


AND R1,R1,0 ;;counter=0

AND R2,R2,0
LD R2,TRIANGLE ;;triangle

AND R4,R4,0 ;;forspaces=0

AND R3,R3,0
ADD R3,R3,R2 ;;Decimal=traingle

MAINRESET ADD R3,R3,0
BRZ END 	;; if(decimal !=0)
BIGINNER ADD R3,R3,0
BRNZ MAINRESET 	;; while(decimal > 0)
AND R1,R1,0	 	;; counter = 0;
INNERFIRST AND R6,R6,0	;; \\while conditions starts
ADD R6,R6,R1		;;
NOT R6,R6		;;
ADD R6,R6,1		;;
ADD R6,R6,R4		;;
BRNZ FAILONE		;; while(counter<forspaces)
AND R0,R0,0			;;\\print line starts
ADD R0,R0,15			;;
ADD R0,R0,15			;;
ADD R0,R0,2			;;
OUT				;; printf(" ")
ADD R1,R1,1			;; counter++
BRP INNERFIRST			;; \\end of first while loop
FAILONE AND R1,R1,0	;; counter=0
INNERSECOND AND R7,R7,0	;; \\while condition starts
ADD R7,R7,R1		;;
NOT R7,R7		;;
ADD R7,R7,1		;;
ADD R7,R7,R3		;;
BRNZ FAILTWO		;; while(counter<decimal)
AND R0,R0,0			;; \\print line starts
ADD R0,R0,R3			;;
ADD R0,R0,15			;;
ADD R0,R0,15			;;
ADD R0,R0,15			;;
ADD R0,R0,3			;;
OUT				;; printf("%d", decimal)
ADD R1,R1,1			;; counter++
BRP INNERSECOND			;; \\end of second while loop
FAILTWO ADD R3,R3,-2	;; decimal=decimal-2
ADD R4,R4,1		;; fourspaces++
AND R0,R0,0		;; \\Start of printline
ADD R0,R0,10		;;
OUT			;;printf("\n")
BRP BIGINNER		;;end of first While loop

END HALT
TRIANGLE	.fill 9
.end

