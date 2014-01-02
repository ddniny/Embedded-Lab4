/*
 *@Authors: Danni Wu < danniw @ andrew.cmu.edu > 
 *@	 	  Yu Qi < yq @ andrew.cmu.edu > 
 *@	  	  Wenting Gong < wentingg @ andrew.cmu.edu > 
 *@Date: 11 / 18 / 2013 10:00 pm
 */
#include <exports.h>
#include <arm/interrupt.h>
#include <myOwn.h>
#include <arm/reg.h>
#include <sched.h>
#include <math.h>
#include <assert.h>

#define irqVectorAddr (unsigned int*)0x18

extern uint32_t oldSupInstr1, oldSupInstr2, oldIRQInstr1, oldIRQInstr2;
extern uint32_t pre_icmr, pre_iclr; //store the old ICMR, ICLR and OIER register
uint32_t reschedul;			//a global variable to control the context switch
typedef void (*intrpAddr)(uint32_t int_num); //intrpAddr is a pointer to a function that has one parameter int_num
intrpAddr interruptHandlers[NUM_INTERRUPTS];  //NUM_INTERRUPTS = 32, store pointers that point to different interrupts handler functions.

void interrupt_panic(unsigned int int_num){
	panic("Received unknown interrupt: %d\n", int_num);
}

void init_interrupt(void){
	reschedul = 0;
	//initialize interrupt controller
	pre_icmr = reg_read(INT_ICMR_ADDR); 	//read the previous icmr and iclr in order to restore them later.
	pre_iclr = reg_read(INT_ICLR_ADDR);
	reg_write(INT_ICMR_ADDR, ICMR_TIMER); //ICMR_TIMER mask all other interrupts, only enable timer interrupt, #define ICMR_TIMER 0x04000000
	reg_write(INT_ICLR_ADDR, ICLR_TIMER); //control whether pending interrupts generate FIQs or IRQs;
	install_Handler (irqVectorAddr, (unsigned int) irq_wrapper);
}

/*
 * restore the previous value to the register.
 */
void destroy_interrupt(void){
	reg_write(INT_ICMR_ADDR, pre_icmr);
	reg_write(INT_ICLR_ADDR, pre_iclr);
	restore_Handler (irqVectorAddr);
}


void irq_handler(void){
	uint32_t interrupt;
	uint32_t int_num;
	//interrupt = (reg_read(INT_ICPR_ADDR) & reg_read(INT_ICMR_ADDR)); 
	if ((reg_read(INT_ICPR_ADDR) & reg_read(INT_ICMR_ADDR))){ //find the enabled and occurred interrupt
		interrupt = (reg_read(INT_ICPR_ADDR) & reg_read(INT_ICMR_ADDR));
		int_num = ilog2(interrupt);
		interruptHandlers[int_num](int_num); //go to a interrupt handler function
	}

	if (reschedul){		//if the reschedul == 1, call dispatch_save to context switch.
		reschedul = 0;
		dispatch_save();
	}
}

void request_reschedule(void){
	reschedul = 1;
}

void install_int_handler(unsigned int int_num, void (*int_handler)(unsigned int)){
	
	interruptHandlers[int_num] = int_handler;
	
}

