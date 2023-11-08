#ifndef __THREAD_H
#define __THREAD_H

#include "stm32f4xx_hal.h"

#define startLED2 0x01
#define startLED3 0x02

int Init_Thread3 (void);
void Thled3 (void *argument);   

#endif
