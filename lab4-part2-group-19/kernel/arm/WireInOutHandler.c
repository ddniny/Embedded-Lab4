/*
 *@Authors: Danni Wu < danniw @ andrew.cmu.edu > 
 *@     Yu Qi < yq @ andrew.cmu.edu > 
 *@       Wenting Gong < wentingg @ andrew.cmu.edu > 
 *@Date: 11 / 18 / 2013 10:00 pm
 */
 
#include <bits/swi.h>
#include <syscall.h>
#include <exports.h>
#include <arm/exception.h>

#define EXIT_NUMBER 0x0badc0de
uint32_t oldSupInstr1, oldSupInstr2, oldIRQInstr1, oldIRQInstr2;

int install_Handler (unsigned int *vectorAddr, unsigned int ourHandlerAddr)
{
  int offset;
  unsigned int *jmpAddr = 0, *swiAddr, *our_S_Handler;
  if ((*vectorAddr & ~0x00000fff) != 0xe59ff000)	//confirm that the SWI vector contains a the right ldr instruction
    {				//identify whether the #imm12 is positive or negative
      return -1;
    }
  offset = *vectorAddr & 0x00000fff;
  jmpAddr = (unsigned int *) ((int) vectorAddr + offset + 0x08);
  swiAddr = (unsigned int *) *jmpAddr;
  our_S_Handler = swiAddr + 1;

  if(vectorAddr == (unsigned int *)0x08){ //store the old instructions in the SWI_HANDLER
	oldSupInstr1 = *swiAddr;
	oldSupInstr2 = *(swiAddr + 1);
  }else{
	oldIRQInstr1 = *swiAddr;
	oldIRQInstr2 = *(swiAddr + 1);
  }
  *swiAddr = (unsigned int)0xe51ff004;	//The ARM machine code corresponding to the assembly instruction of LDR PC, [PC, #-4]
  *our_S_Handler = ourHandlerAddr;	//The address of OUR_S_HANDLER
  return 0;
}

int restore_Handler (unsigned int *vectorAddr)
{
  int offset;
  unsigned int *jmpAddr = 0, *swiAddr, *our_S_Handler;
  if ((*vectorAddr & ~0x00000fff) != 0xe59ff000)	//confirm that the SWI vector contains a the right ldr instruction
    {				//identify whether the #imm12 is positive or negative
      return -1;
    }
  offset = *vectorAddr & 0x00000fff;
  jmpAddr = (unsigned int *) ((int) vectorAddr + offset + 0x08);
  swiAddr = (unsigned int *) *jmpAddr;
  our_S_Handler = swiAddr + 1;
  if (vectorAddr == (unsigned int *)0x08){
  	*swiAddr = oldSupInstr1;
  	*our_S_Handler = oldSupInstr2;
  }else{
	*swiAddr = oldIRQInstr1;
  	*our_S_Handler = oldIRQInstr2;
  }
  return 0;
}

