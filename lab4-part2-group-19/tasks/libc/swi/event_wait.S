/** @file event_wait.S
 *
 * @brief time sycall wrapper
 *
 * @author Danni Wu <danniw@andrew.cmu.edu>
 *         Yu Qi <yq@andrew.cmu.edu>
 *         Wenting Gong <wentingg@andrew.cmu.edu>
 * @date 2013-11-20
 */

#include <asm.h>
#include <bits/swi.h>

	.file "event_wait.S"

FUNC(event_wait)
	@ Add code here.  Put your name at the top.
	SWI EVENT_WAIT 			@call the software interruption
	CMP r0, #0				@compare the return value with zero
	BGE .GE					@if the return value equal to or bigger than zero then return to the user function
.LESS:	
	LDR r1, =errno  		@if the return value smaller than zero then load the address of the global errno to the register r1
	MVN r0, r0				@and set the errno to the negative of the syscall return
	ADD r0, #1
	STR r0, [r1]
	MOV r0, #-1				@the syscall wrapper will return -1 if there are any error
.GE:
	MOV pc, lr


