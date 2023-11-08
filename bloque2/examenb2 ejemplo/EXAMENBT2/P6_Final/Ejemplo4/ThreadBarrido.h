#ifndef __THREADBARRIDO_H
#define __THREADBARRIDO_H

#include "stm32f4xx_hal.h"

#define BARRIDO_INVERSO 0x02
#define VUELTA 0x01

int Init_Thread_Barrido (void);
void Thread_Barrido (void *argument);

#endif
