#ifndef __THREADRELOJ_H
#define __THREADRELOJ_H

#include "stm32f4xx_hal.h"

#define BARRIDO 0x04
#define BARRIDO_INVERSO 0x02
#define VUELTA 0x01


int Init_Thread (void);                   // thread function
void Thread_Reloj (void *argument);

#endif
