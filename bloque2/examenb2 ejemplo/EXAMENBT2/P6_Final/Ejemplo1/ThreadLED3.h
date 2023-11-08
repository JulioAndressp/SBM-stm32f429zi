#ifndef __THREAD3_H
#define __THREAD3_H

#include "stm32f4xx_hal.h"

#define ARRANCALED3 0x02

void Thread_LED3 (void *argument);                   // thread function
int Init_Thread (void);				

#endif
