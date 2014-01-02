/** @file sched.c
 * 
 * @brief Top level implementation of the scheduler.
 *
 * @Authors: Danni Wu < danniw @ andrew.cmu.edu > 
 * @         Yu Qi < yq @ andrew.cmu.edu > 
 * @         Wenting Gong < wentingg @ andrew.cmu.edu > 
 * @Date: 11 / 18 / 2013 10:00 pm
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <config.h>
#include "sched_i.h"

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */

void sched_init(task_t* main_task  __attribute__((unused)))
{
	
}

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
 
static void __attribute__((unused)) idle(void)
{
	 enable_interrupts();
	 while(1);
}

/**
 * @brief Allocate user-stacks and initializes the kernel contexts of the
 * given threads.
 *
 * This function assumes that:
 * - num_tasks < number of tasks allowed on the system.
 * - the tasks have already been deemed schedulable and have been appropriately
 *   scheduled.  In particular, this means that the task list is sorted in order
 *   of priority -- higher priority tasks come first.
 *
 * @param tasks  A list of scheduled task descriptors.
 * @param size   The number of tasks is the list.
 */
void allocate_tasks(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{

	size_t i = 0;
	runqueue_init();
	for (i = 1; i < num_tasks+1; i ++){
		system_tcb[i].native_prio = i;
		system_tcb[i].cur_prio = i;
		system_tcb[i].sleep_queue = 0;
		system_tcb[i].holds_lock = 0;
		system_tcb[i].context.r4 = (uint32_t)tasks[i-1]->lambda;
		system_tcb[i].context.r5 = (uint32_t)tasks[i-1]->data;
		system_tcb[i].context.r6 = (uint32_t)tasks[i-1]->stack_pos;
		system_tcb[i].context.r7 = tasks[i-1]->C;
		system_tcb[i].context.r8 = global_data;
		system_tcb[i].context.r9 = tasks[i-1]->T;
		system_tcb[i].context.r10 = 0;
		system_tcb[i].context.r11 = 0;
		system_tcb[i].context.sp = -8 + &(system_tcb[i].kstack[OS_KSTACK_SIZE/sizeof(uint32_t)-1]);
		system_tcb[i].context.lr = launch_task;
		runqueue_add(&system_tcb[i], i);
	}
	system_tcb[IDLE_PRIO].native_prio = IDLE_PRIO;
	system_tcb[IDLE_PRIO].cur_prio = IDLE_PRIO;
	system_tcb[IDLE_PRIO].sleep_queue = 0;
	system_tcb[IDLE_PRIO].holds_lock = 0;
	system_tcb[IDLE_PRIO].context.r4 = (uint32_t)idle;
	system_tcb[IDLE_PRIO].context.r5 = 0;
	system_tcb[IDLE_PRIO].context.r6 = 0xa2ffffff;    //set up the stack for idle
	system_tcb[IDLE_PRIO].context.r7 = 0;
	system_tcb[IDLE_PRIO].context.r8 = global_data;
	system_tcb[IDLE_PRIO].context.r9 = 0;
	system_tcb[IDLE_PRIO].context.r10 = 0;
	system_tcb[IDLE_PRIO].context.r11 = 0;
	system_tcb[IDLE_PRIO].context.sp = -8 + &(system_tcb[IDLE_PRIO].kstack[OS_KSTACK_SIZE/sizeof(uint32_t)-1]);
	system_tcb[IDLE_PRIO].context.lr = launch_task;
	runqueue_add(&system_tcb[IDLE_PRIO], IDLE_PRIO);
	dispatch_init(&(system_tcb[IDLE_PRIO]));
}

