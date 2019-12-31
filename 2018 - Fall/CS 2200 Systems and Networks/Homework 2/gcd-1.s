!============================================================
! CS 2200 Homework 2 Part 2: gcd
! Zachary Waters
! Apart from initializing the stack,
! please do not edit main's functionality.
!============================================================

main:
    lea     $sp, stack          ! load ADDRESS of stack label into $sp

	!////////////////////
    lw      $sp, 0($sp)       
	!////////////////////
	
	add     $zero, $zero, $zero ! Here, you need to initialize the stack
                                ! using the label below by loading its
                                ! VALUE into $sp (CHANGE THIS INSTRUCTION)
    lea     $at, gcd            ! load address of gcd label into $at
    addi    $a0, $zero, 25      ! $a0 = 25, the number a to compute gcd(a,b)
    addi    $a1, $zero, 15      ! $a1 = 15, the number b to compute gcd(a,b)
    jalr    $ra, $at            ! jump to gcd, set $ra to return addr
    halt                        ! when we return, just halt

gcd:
    !//////////////////////////
    sw       $fp, 0x0($sp)
	!/////////////////////////
	
	add     $zero, $zero, $zero ! TODO: perform post-call portion of
                                !       the calling convention
    bne     $a1, $zero, mod     ! if (b != 0) go to mod

    addi    $t1, $zero, 1       ! $t1 = 1
    bne     $t1, $zero, fin     ! else, return the value of a

mod:
    addi    $t0, $a0, 0         ! x = a
loop:                           ! while x >= b
    slt     $t1, $t0, $a1       ! if x >= b, $t1 = 0
    bne     $t1, $zero, rec     ! if x < b, recurse

    nand    $t2, $a1, $a1       ! x = x - b
    addi    $t2, $t2, 1
    add     $t0, $t0, $t2

    addi    $t1, $zero, 1       ! $t1 = 1
    bne     $t1, $zero, loop
    
rec:

	!///////////////////////////
	addi     $sp, $sp, -1
	sw       $ra, 0x1($sp)
	
	addi     $a0, $a1, 0
	addi     $a1, $t0, 0
   !////////////////////////////
	
	add     $zero, $zero, $zero ! TODO: perform pre-call portion
                                !       of the calling convention
    
    lea     $at, gcd            ! load address of gcd label into $at
    jalr    $ra, $at            ! jump to gcd, set $ra to return address
    

	!//////////////////////////
	lw      $ra, 0x1($sp)
	lw      $fp, 0x0($sp)
	addi    $sp, $sp, 1
    !/////////////////////////
	
    add     $zero, $zero, $zero ! TODO: perform post-return portion
                                !       of the calling convention
								
    
fin:
    add     $zero, $zero, $zero ! TODO: perform pre-return portion
                                !       of the calling convention
    
	!///////////////////////
	addi     $v0, $a0, 0
	!///////////////////////
	
    jalr    $zero, $ra          ! return to caller

stack: .word 0xFFFFFF           ! the stack begins here
