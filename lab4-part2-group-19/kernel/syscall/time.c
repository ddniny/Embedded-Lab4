/** @file time.c
 *
 * @brief Kernel timer based syscall implementations
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-16
 */

#include <types.h>
#include <config.h>
#include <bits/errno.h>
#include <arm/timer.h>
#include <syscall.h>


unsigned long time_syscall(void)
{
 	return get_millis();
}



/** @brief Waits in a tight loop for atleast the given number of milliseconds.
 *
 * @param millis  The number of milliseconds to sleep.
 *
 * 
 */
void sleep_syscall(unsigned long millis  __attribute__((unused)))
{
	unsigned long sleepEnd = get_millis() + millis;
	while (get_millis() <= sleepEnd);
}
