ARM_OBJS := reg.o psr.o swi.o S_Handler.o int_asm.o interrupt.o timer.o WireInOutHandler.o preForUser.o 
ARM_OBJS := $(ARM_OBJS:%=$(KDIR)/arm/%)

KOBJS += $(ARM_OBJS)
