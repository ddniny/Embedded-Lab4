/** @file proc.c
 * 
 * @brief Implementation of `process' syscalls
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-12
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>

int task_create(task_t* tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
  task_t * allocTasks[num_tasks];
  task_t *temp;
  disable_interrupts();
  size_t i = 0, j = 0;
  if (num_tasks > OS_AVAIL_TASKS){  //OS_AVAIL_TASKS = 63
  puts("Task number should be within 0 to 63.\n");
  return -EINVAL;
  }
  if (!valid_addr(tasks, num_tasks, USR_START_ADDR, USR_END_ADDR)){
  puts("Address is invalid.\n");
  return -EFAULT;
  }
  for (i = 0; i < num_tasks; i ++){
  allocTasks[i] = tasks + i;
  }
  for (i = 0; i < num_tasks; i ++){ //organize the tasks in priority decreasing order.
  for (j = i+1; j < num_tasks; j ++){
    if (allocTasks[i]->T > allocTasks[j]->T){
      temp = allocTasks[i];
      allocTasks[i] = allocTasks[j];
      allocTasks[j] = temp;
    }
  }
  }
  if (assign_schedule(allocTasks, num_tasks)){
  allocate_tasks(allocTasks, num_tasks);
  dispatch_nosave();
  return 0;
}
else{
  while (1){
    puts("Tasks are not schedulable.\n");
  }
  return -1;
}
}

int event_wait(unsigned int dev  __attribute__((unused)))
{
  if (dev >= NUM_DEVICES) {
  puts("Device number should be within 0 to 3.\n");
  return -EINVAL;
  }
  disable_interrupts();
  dev_wait(dev);
  enable_interrupts();
  return 0;
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num  __attribute__((unused)))
{
  printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

  disable_interrupts();
  while(1);
}
