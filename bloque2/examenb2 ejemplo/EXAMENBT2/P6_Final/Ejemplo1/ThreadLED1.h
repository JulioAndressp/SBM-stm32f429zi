#ifndef __THREAD1_H
#define __THREAD1_H

#include "stm32f4xx_hal.h"

#define ARRANCATIMER 0x04

void Thread_LED1 (void *argument);                   // thread function
int Init_Thread (void);				

#endif
