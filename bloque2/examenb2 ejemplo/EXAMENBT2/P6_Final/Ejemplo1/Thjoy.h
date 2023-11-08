#ifndef __THREAD_H
#define __THREAD_H

#include "stm32f4xx_hal.h"

#define PRUEBA 0x02



void Thjoy (void *argument);                   // thread function
int Init_Thread (void);				

#endif
