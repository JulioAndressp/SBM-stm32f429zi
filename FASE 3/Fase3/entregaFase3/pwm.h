#ifndef __PWM_H
#define __PWM_H

#include "stm32f4xx_hal.h"

int Init_Zumbador(void);
void Thread_Zumbador (void *argument);                   // thread function
	 

#define PWM_SIGNAL 0x0013





#endif 

