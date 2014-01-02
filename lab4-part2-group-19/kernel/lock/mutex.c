/**
 * @file mutex.c
 *
 * @brief Implements mutices.
 *
 * @Authors: Danni Wu < danniw @ andrew.cmu.edu > 
 * @         Yu Qi < yq @ andrew.cmu.edu > 
 * @         Wenting Gong < wentingg @ andrew.cmu.edu > 
 *
 * 
 * @date  12/02/2013 23:00
 */

//#define DEBUG_MUTEX

#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>
#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#endif
#define NULL 0
mutex_t gtMutex[OS_NUM_MUTEX];
volatile int mutex_num = 0;
void mutex_init()
{
	int i;

	for(i=0;i<OS_NUM_MUTEX;i++){

		gtMutex[i].bAvailable = TRUE;//mutex is available to be created
		gtMutex[i].pHolding_Tcb = FALSE;
		gtMutex[i].bLock = 0;
		gtMutex[i].pSleep_queue = 0;	
	}
	return;
	
}

int mutex_create(void)
{	
	disable_interrupts();
	int i = 0;
	if(mutex_num >= OS_NUM_MUTEX){
		enable_interrupts();
		return -ENOMEM;
	}

	else{ 
		while(i<OS_NUM_MUTEX){		
			if(gtMutex[i].bAvailable){//if the mutex can be created
				gtMutex[i].bAvailable = FALSE;//the mutex is created, so it is now unavailable
				break;
			}
			i++;
		}
		mutex_num++;
		enable_interrupts();
		return i;
	}
	
}

int mutex_lock(int mutex  __attribute__((unused)))
{
	disable_interrupts();
	tcb_t* current_tcb = get_cur_tcb();
	
	
	if(mutex >=OS_NUM_MUTEX || mutex < 0){
		enable_interrupts();

		return -EINVAL;	
	}
	if(gtMutex[mutex].pHolding_Tcb == get_cur_tcb()){
		enable_interrupts();

		return -EDEADLOCK;	
	}
	if(gtMutex[mutex].bAvailable == TRUE){
		enable_interrupts();
		return -EINVAL;//mutex is not created	
	}
	if(gtMutex[mutex].bLock == FALSE){

		gtMutex[mutex].bLock = TRUE;//lock the mutex
		gtMutex[mutex].pHolding_Tcb = get_cur_tcb();
		current_tcb->native_prio = current_tcb->cur_prio; //save current priority in native priority
		current_tcb->cur_prio=0; //set current priority to zero
		current_tcb->holds_lock = 1;
			
	}
	else{//if mutex is already locked, add the current task to the sleep queue

		tcb_t* queue_head = gtMutex[mutex].pSleep_queue;
		tcb_t* current_tcb = get_cur_tcb();
		
		if(queue_head ==  NULL){
			gtMutex[mutex].pSleep_queue= current_tcb;
			current_tcb->sleep_queue = NULL;
		}
		else{
			while(queue_head->sleep_queue!= NULL){//add to sleep queue
				queue_head = queue_head->sleep_queue;		
			}
			queue_head->sleep_queue = current_tcb;
			current_tcb->sleep_queue = NULL;
		}
		dispatch_sleep();
	}
	
	// gtMutex[mutex].bLock = TRUE;//lock the mutex
	// gtMutex[mutex].pHolding_Tcb = get_cur_tcb();		
	enable_interrupts();
	return 0;
}

int mutex_unlock(int mutex  __attribute__((unused)))
{
	mutex_t *mutex_p;
	mutex_p = &gtMutex[mutex];
	tcb_t* header;
	disable_interrupts();
	if(mutex >=OS_NUM_MUTEX || mutex < 0){
		enable_interrupts();
		return -EINVAL;	
	}
	if(gtMutex[mutex].bAvailable == TRUE){
		enable_interrupts();
		return -1;//mutex is not created	
	}
	if(gtMutex[mutex].pHolding_Tcb != get_cur_tcb()){
		enable_interrupts();
		return -EPERM;	
	}
//	if(gtMutex[mutex].bLock == TRUE){
		// gtMutex[mutex].bLock = 0;//unlock the mutex
		// gtMutex[mutex].pHolding_Tcb = 0;
		// if(gtMutex[mutex].pSleep_queue != 0){
		// 	tcb_t* next_tcb = gtMutex[mutex].pSleep_queue;
		// 	gtMutex[mutex].pSleep_queue = next_tcb->sleep_queue;
		// 	next_tcb->sleep_queue = 0;
		// 	runqueue_add(next_tcb, next_tcb->cur_prio);
		// 	tcb_t* current_tcb = get_cur_tcb(); 
		// 	current_tcb->cur_prio = current_tcb->native_prio; //restore native_prio back to cur_prio
	
		// }
//	}

	tcb_t* current_tcb = get_cur_tcb();
	current_tcb->holds_lock = 0;
	current_tcb->cur_prio = current_tcb->native_prio;

	if (mutex_p->pSleep_queue != 0){
		header = mutex_p->pSleep_queue;
		header->holds_lock = 1;
		header->cur_prio = 0;
		mutex_p->pSleep_queue = header->sleep_queue;
		mutex_p->pHolding_Tcb = header;
		mutex_p->bLock = 1;
		runqueue_add (header, header->cur_prio);
	}
	else{
		mutex_p->pHolding_Tcb = 0;
		mutex_p->bLock = 0;
	}
  enable_interrupts();
	return 0;
	
}



























