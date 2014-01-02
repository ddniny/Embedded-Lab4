/** @file main.c
 *
 * @brief kernel main
 *
@Authors: Danni Wu < danniw @ andrew.cmu.edu > 
@	 	  Yu Qi < yq @ andrew.cmu.edu > 
@	  	  Wenting Gong < wentingg @ andrew.cmu.edu > 
@Date: 11 / 18 / 2013 10:00 pm
 */
 
#include <kernel.h>
#include <task.h>
#include <sched.h>
#include <device.h>
#include <assert.h>
#include <arm/swi.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <device.h>
#include <myOwn.h>
#include <lock.h>

uint32_t global_data, backSp;

int kmain(int argc __attribute__((unused)), char** argv  __attribute__((unused)), uint32_t table)
{

	app_startup();
	global_data = table;
	/* add your code up to assert statement */

	init_swi();			//initialize the swi 
	init_interrupt();   //initialize the interrupt
	init_timer(); 		//initialize the OS Timer
	dev_init();			//initialize the devide
	mutex_init();
	preForUser();		//go to user program
	assert(0);        /* should never get here */
}
