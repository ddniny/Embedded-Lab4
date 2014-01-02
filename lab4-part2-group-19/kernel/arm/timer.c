/*
 *@Authors: Danni Wu < danniw @ andrew.cmu.edu > 
 *@	 	    Yu Qi < yq @ andrew.cmu.edu > 
 *@	  	    Wenting Gong < wentingg @ andrew.cmu.edu > 
 *@Date: 11 / 18 / 2013 10:00 pm
 */
 
#include <arm/interrupt.h>
#include <arm/reg.h>
#include <arm/timer.h>
#include <myOwn.h>
#include <device.h>

volatile unsigned long systemTime;
uint32_t pre_icmr, pre_iclr, pre_oier;

/*
 * initialize the OS Timer register
 */
void init_timer(void){
	pre_oier = reg_read(OSTMR_OIER_ADDR);
	systemTime = 0;				//initialize the global variable sistemTime to 1
	reg_write(OSTMR_OSMR_ADDR(0), OSTMR_TIMER); //1ms
	reg_write(OSTMR_OIER_ADDR, OSTMR_OIER_E0);

	install_int_handler(INT_OSTMR_0, timer_handler); //INT_OSTMR_0 = 26
	
    reg_write(OSTMR_OSSR_ADDR, 0x0);
	reg_write(OSTMR_OSCR_ADDR, 0x0);
}

void destroy_timer(void){ //make all OS Match register inactive
	reg_write(OSTMR_OIER_ADDR, 0X0);
	install_int_handler(INT_OSTMR_0, interrupt_panic);  //replace timer interrupt with panic handler
}

void timer_handler(unsigned int int_num __attribute__((unused))){
	if (reg_read(OSTMR_OSSR_ADDR) == OSTMR_OSSR_M0){  //indicate a match has occurred between OSCR and OSMR0
		systemTime ++;
		reg_write(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);		//signal the match
		reg_write(OSTMR_OSCR_ADDR, 0x0);
		reg_write(OSTMR_OSMR_ADDR(0), OSTMR_TIMER); //1ms
		dev_update(systemTime);
	}
}

unsigned long get_ticks(void){
	return systemTime;
}
unsigned long get_millis(void){
	return systemTime;
}
