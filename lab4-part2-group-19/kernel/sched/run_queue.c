/** @file run_queue.c
 * 
 * @brief Run queue maintainence routines.
 *
 *@Authors: Danni Wu < danniw @ andrew.cmu.edu > 
 *@         Yu Qi < yq @ andrew.cmu.edu > 
 *@         Wenting Gong < wentingg @ andrew.cmu.edu > 
 *@Date: 11 / 18 / 2013 10:00 pm
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <sched.h>
#include "sched_i.h"



static tcb_t* run_list[OS_MAX_TASKS]  __attribute__((unused));

/* A high bit in this bitmap means that the task whose priority is
 * equal to the bit number of the high bit is runnable.
 */
static uint8_t run_bits[OS_MAX_TASKS/8] __attribute__((unused));

/* This is a trie structure.  Tasks are grouped in groups of 8.  If any task
 * in a particular group is runnable, the corresponding group flag is set.
 * Since we can only have 64 possible tasks, a single byte can represent the
 * run bits of all 8 groups.
 */
static uint8_t group_run_bits __attribute__((unused));

/* This unmap table finds the bit position of the lowest bit in a given byte
 * Useful for doing reverse lookup.
 */
static uint8_t prio_unmap_table[]  __attribute__((unused)) =
{

0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

/**
 * @brief Clears the run-queues and sets them all to empty.
 */
void runqueue_init(void)
{
	int i = 0;
	for (i = 0; i < OS_MAX_TASKS/8; i++){
		run_bits[i] = 0;
	}
	for (i = 0; i < OS_MAX_TASKS; i++){
		run_list[i] = (tcb_t*)0;
	}
	group_run_bits = 0;
}

/**
 * @brief Adds the thread identified by the given TCB to the runqueue at
 * a given priority.
 *
 * The native priority of the thread need not be the specified priority.  The
 * only requirement is that the run queue for that priority is empty.  This
 * function needs to be externally synchronized.
 */
void runqueue_add(tcb_t* tcb  __attribute__((unused)), uint8_t prio  __attribute__((unused)))
{
	if (prio > OS_MAX_TASKS){
		return;
	}
	group_run_bits =  (0x1 << (prio >> 3)) | group_run_bits;
	run_bits[prio>>3] = (0x1 << (prio & 0x07)) | run_bits[prio>>3];
	run_list[prio] = tcb;
}


/**
 * @brief Empty the run queue of the given priority.
 *
 * @return  The tcb at enqueued at the given priority.
 *
 * This function needs to be externally synchronized.
 */
tcb_t* runqueue_remove(uint8_t prio  __attribute__((unused)))
{
	tcb_t * rmTcb;
	if (prio >= OS_MAX_TASKS){
		return 0;
	}
	if (prio == IDLE_PRIO){
		return run_list[prio];	
	}
	
	rmTcb = run_list[prio];
	run_list[prio] = (tcb_t *)0;
	run_bits[prio>>3] = run_bits[prio>>3] & (~(0x1 << (prio & 0x7)));
	if (run_bits[prio>>3] == 0){
	group_run_bits = group_run_bits & (~(0x1 << (prio >> 3)));
	}

	return rmTcb; // fix this; dummy return to prevent warning messages	
}

/**
 * @brief This function examines the run bits and the run queue and returns the
 * priority of the runnable task with the highest priority (lower number).
 */
uint8_t highest_prio(void)
{	
	int x, y, hprio;
	y = prio_unmap_table[group_run_bits];
	x = prio_unmap_table[run_bits[y]];
	hprio = (y<<3)+x;
	return hprio; // fix this; dummy return to prevent warning messages	
}
