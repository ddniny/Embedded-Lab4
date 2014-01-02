#include <exports.h>
#include <types.h>

#define OSTMR_TIMER 3686 //(OSTMR_FREQ/1000)
#define ICMR_TIMER 0x04000000
#define ICLR_TIMER 0x0

int preForUser(void);

int install_Handler (unsigned int *vectorAddr, unsigned int ourHandlerAddr);
int restore_Handler (unsigned int *vectorAddr);

int S_Handler(void);

void irq_wrapper(void);
void C_IRQ_Handler(void);

extern uint32_t oldSupInstr1, oldSupInstr2, oldIRQInstr1, oldIRQInstr2;
extern uint32_t backSp;
extern volatile unsigned long systemTime;
extern uint32_t pre_icmr, pre_iclr, pre_oier; //store the old ICMR, ICLR and OIER register
extern uint32_t reschedul;




