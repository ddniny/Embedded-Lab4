/** @file ctx_switch.c
 * 
 * @brief C wrappers around assembly context switch routines.
 *
 * @Authors: Danni Wu < danniw @ andrew.cmu.edu > 
 * @         Yu Qi < yq @ andrew.cmu.edu > 
 * @         Wenting Gong < wentingg @ andrew.cmu.edu > 
 * @Date: 11 / 18 / 2013 10:00 pm
 */
 
 

#include <types.h>
#include <assert.h>

#include <config.h>
#include <kernel.h>
#include "sched_i.h"

#include <arm/exception.h>
#ifdef DEBUG_MUTEX
#include <exports.h>
#endif

static __attribute__((unused)) tcb_t* cur_tcb; /* use this if needed */

/**
 * @brief Initialize the current TCB and priority.
 *
 * Set the initialization thread's priority to IDLE so that anything
 * will preempt it when dispatching the first task.
 */
void dispatch_init(tcb_t* idle __attribute__((unused)))
{
	cur_tcb = idle;
}


/**
 * @brief Context switch to the highest priority task while saving off the 
 * current task state.
 *
 * This function needs to be externally synchronized.
 * We could be switching from the idle task.  The priority searcher has been tuned
 * to return IDLE_PRIO for a completely empty run_queue case.
 */
void dispatch_save(void)
{
	tcb_t* prev_tcb = cur_tcb; 
	tcb_t* next_tcb; 
	sched_context_t* cur_ctx, *prev_ctx;
	runqueue_add(cur_tcb, cur_tcb->cur_prio);//put current task to run queue
	next_tcb = runqueue_remove(highest_prio());//get the next task, remove it from run queue
	if (next_tcb == cur_tcb){
		return;
	}
	cur_tcb = next_tcb;//change cur_tcb to the target's tcb
	cur_ctx = &(cur_tcb->context);//target context
	prev_ctx = &(prev_tcb->context);//current context
	ctx_switch_full(cur_ctx, prev_ctx);
}

/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * don't save the current task state.
 *
 * There is always an idle task to switch to.
 */
void dispatch_nosave(void)
{
	tcb_t* next_tcb; 
	sched_context_t * next_ctx;
	next_tcb = runqueue_remove(highest_prio());
	cur_tcb = next_tcb;
	next_ctx = &(next_tcb->context);
	ctx_switch_half(next_ctx);//parameter for ctx_switch should be void* (maybe useful when debugging)
}


/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * and save the current task but don't mark is runnable.
 *
 * There is always an idle task to switch to.
 */
void dispatch_sleep(void)
{
	tcb_t *prev_tcb; 
	sched_context_t *prev_ctx, *cur_ctx;
	prev_tcb = get_cur_tcb();
	cur_tcb = runqueue_remove(highest_prio());
	
	cur_ctx = &cur_tcb->context;
	prev_ctx = &prev_tcb->context;
	ctx_switch_full(cur_ctx, prev_ctx);
}

/**
 * @brief Returns the priority value of the current task.
 */
uint8_t get_cur_prio(void)
{
	return cur_tcb->cur_prio;
}

/**
 * @brief Returns the TCB of the current task.
 */
tcb_t* get_cur_tcb(void)
{
	return cur_tcb;
}
