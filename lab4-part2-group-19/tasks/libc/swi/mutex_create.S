/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>
#include <bits/swi.h>
	.file "mutex_create.S"

FUNC(mutex_create)
	SWI	MUTEX_CREATE
	CMP r0, #0			@compare the return value with zero
	BGE .GE				@if the return value equal to or bigger than zero then return to the user function  
.LESS:	
	LDR r1, =errno  	@if the return value smaller than zero then load the address of the global errno to the register r1
	MVN r0, r0			@and set the errno to the negative of the syscall return value
	ADD r0, #1
	STR r0, [r1]
	MOV r0, #-1			@the write syscall wrapper will return -1 if there are any errors
.GE:
	MOV pc, lr 			@return
