#ifndef __THREAD_H
#define __THREAD_H

#include "stm32f4xx_hal.h"

#define startLED2 0x01

int Init_Thread1 (void);
void Thled1 (void *argument);   


#endif
