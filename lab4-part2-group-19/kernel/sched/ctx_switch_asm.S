/** @file ctx_switch_asm.S
 *
 * @brief Contains the core context switch routine.
 *
 * These routines all assume that the caller is following AAPCS, needs
 * no co-processor registers and that the caller is in SVC mode. Furthermore,
 * the SPSR is considered a spare/scratch register and hence, is not saved.
 *
 * @Authors: Danni Wu < danniw @ andrew.cmu.edu > 
 * @         Yu Qi < yq @ andrew.cmu.edu > 
 * @         Wenting Gong < wentingg @ andrew.cmu.edu > 
 * @Date: 11 / 18 / 2013 10:00 pm
 */
 

	.file "ctx_switch_asm.S"

#include <asm.h>
#include <arm/psr.h>

/**
 * @brief Special exit routine from the scheduler that launches a task for the
 * first time.
 *
 * r4 contains the user entry point.
 * r5 contains the single argument to the user function called.
 * r6 contains the user-mode stack pointer.
 * Upon completion, we should be in user mode.
 */
FUNC(launch_task)
	mov     r0, r5
	mov     r1, #0
	mov     r2, #0
	mov     r3, #0
	mov     r5, #0
	mov     r8, #0
	mov     ip, #0
	msr     cpsr_c, #(PSR_MODE_USR)
	mov     sp, r6
	mov     r6, #0
	ldr     lr, =0xdeadbeef   /* Causes a crash instead of calling the reset vector */
	mov     pc, r4

/* r0 points to the target context, r1 to the current context. */
/* add your code to perform a full context switch */
FUNC(ctx_switch_full)
	STMIA	r1!, {r4-r11, sp, lr}//save current context into a address space with the start addresse pointed by parameter 1
	LDMIA	r0!, {r4-r11, sp, lr}//load next context from parameter 0
	MOV		pc, lr

/*add your code to perform a half context switch */	
FUNC(ctx_switch_half)
	LDMIA	r0!, {r4-r11, sp, lr}//cannot remember specific register numbers, but xuezhang wrote like this..
	MOV	pc, lr
