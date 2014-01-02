/** @file write.S
 *
 * @brief write sycall wrapper
 *
 * @author Danni Wu <danniw@andrew.cmu.edu>
 *		   Yu Qi <yq@andrew.cmu.edu>
 *		   Wenting Gong <wentingg@andrew.cmu.edu> 
 *
 * @date 2013-11-20
 */

#include <asm.h>
#include <bits/swi.h>

	.file "write.S"

FUNC(write)
	@ Add code here.  Put your names at the top.
	swi WRITE_SWI		@call the software interruption
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

	



