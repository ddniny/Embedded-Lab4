@ S_Handler.S: assemblly code for kernel 
@
@ Authors: Danni Wu <danniw@andrew.cmu.edu>
@          Yu Qi <yq@andrew.cmu.edu>     
@          Wenting Gong <wentingg@andrew.cmu.edu> 
@ Date:    11/18/2013 01:00pm



	.file	"S_Handler.S"
	.text

	.global	S_Handler
	.global swi_dispatch
	
S_Handler:
	SUB	sp, sp, #4				@leave room on stack for SPSR
	STMFD	sp!, {r0-r12, lr}	@store user's gp registers
	MRS	r2, spsr				@get SPSR into gp registers
	STR	r2, [sp, #14*4]			@store SPSR above gp registers
	MOV	r1, sp					@pointer to parameters on stack

	LDR	r7, =global_data		@load r8
	LDR	r8, [r7]
	LDR r0, [lr, #-4]			@extract the SWI number
	BIC	r0, r0, #0xff000000		@get SWI # by bit-masking

	stmfd sp, {sp, lr}^			@store svc stack and return address
	add sp, sp, #-8

	BL	swi_dispatch			@goto handler	

	ldmfd sp, {sp, lr}^			
	add sp, sp, #8
	
	LDR	r2, [sp, #14*4]			@restore SPSR (NOT "SP!")
	MSR	spsr, r2				@restore SPSR from r2
	ADD 	sp, sp, #4			@protect the return value from read/write in r0
	LDMFD	sp!, {r1-r12, lr}	@remove space used to store SPSR
	ADD	sp, sp, #4				@remove space used to store SPSR
	MOVS 	pc, lr				@return from handler
