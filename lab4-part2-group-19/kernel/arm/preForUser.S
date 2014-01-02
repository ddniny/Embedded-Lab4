@ preForUser.S:assemblly code for kernel
@ 
@Authors: Danni Wu < danniw @ andrew.cmu.edu > 
@	 	  Yu Qi < yq @ andrew.cmu.edu > 
@	  	  Wenting Gong < wentingg @ andrew.cmu.edu > 
@Date: 11 / 18 / 2013 10:00 pm

#define CPSR_IRQ_FLAG 0x80
	.file "S_Handler.S"
	.text

	.global preForUser
	.global init_swi 
	.global init_interrupt
	.global init_timer
	.global dev_init
preForUser:
	MRS	r5, cpsr
	BIC	r5, r5, #0x000000ff			@switch to user mode with IRQs & FIQs masked
	EOR	r5, r5, #0x000000d0 		@#0x00000050
	MSR	cpsr, r5

	LDR	sp, =0xa3000000				@setup a full descending user mode stack
	
    MOV 	lr, pc
    LDR 	pc, =0xa0000000 		@jump to a loaded user program at address 0xa0000000 
