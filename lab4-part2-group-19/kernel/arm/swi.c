/*
 *@Authors: Danni Wu < danniw @ andrew.cmu.edu > 
 *@         Yu Qi < yq @ andrew.cmu.edu > 
 *@         Wenting Gong < wentingg @ andrew.cmu.edu > 
 *@Date: 11 / 18 / 2013 10:00 pm
 */

#include <arm/swi.h>
#include <myOwn.h>
#include <arm/exception.h>
#include <bits/swi.h>
#include <syscall.h>
#include <task.h>
#include <lock.h>
#define swiVectorAddr (unsigned int*)0x08
extern int S_Handler(void);

/*
 * wire in the swi handler
 */
void init_swi(void){
	install_Handler (swiVectorAddr, (unsigned int) S_Handler);   
}

/*
 * restore swi handler
 */
void destroy_swi(void){
	restore_Handler (swiVectorAddr);
}

void swi_dispatch(unsigned int swi_num, struct ex_context* c){
  if (swi_num != CREATE_SWI && swi_num != EVENT_WAIT){
    enable_interrupts();
  }
  switch (swi_num)
    {
    case READ_SWI:		//0x900003
      c->r0 = read_syscall (c->r0, (void *) c->r1, c->r2);	//go to our system call read
      break;
    case WRITE_SWI:		//0x900004
      c->r0 = write_syscall (c->r0, (void *) c->r1, c->r2);	//go to our system call write
      break;
    case TIME_SWI: 		//0x900006
      c->r0 = time_syscall();
      break;   
    case SLEEP_SWI:		//0x900007
      sleep_syscall(c->r0);
      break; 
    case CREATE_SWI:		//0x90000a
      c->r0 = task_create((task_t*) c->r0, (size_t) c->r1);	//create tasks 
      break; 
    case MUTEX_CREATE:		//0x90000f
      mutex_create();		
      break;  
    case MUTEX_LOCK:		//0x900010
      c->r0 = mutex_lock(c->r0);			
      break; 
    case MUTEX_UNLOCK:		//0x900011
      c->r0 = mutex_unlock(c->r0);
      break;
    case EVENT_WAIT:		//0x900014
	    c->r0 = event_wait((unsigned int) c->r0);   //called at the end of one task
      break;
    default:
	invalid_syscall (swi_num); 
      break;
    }
	disable_interrupts();
 
}
