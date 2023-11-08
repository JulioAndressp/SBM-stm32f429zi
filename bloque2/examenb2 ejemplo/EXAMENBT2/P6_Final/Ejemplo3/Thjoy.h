#ifndef __THREADJOY_H
#define __THREADJOY_H

#include "stm32f4xx_hal.h"

#define ARRANCA_TIMER_JOY 0x02

void Thread_JOY (void *argument);
int Init_Thread (void);

#endif
