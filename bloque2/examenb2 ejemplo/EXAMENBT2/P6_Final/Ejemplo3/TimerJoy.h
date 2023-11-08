#ifndef __TIMERJOY_H
#define __TIMERJOY_H

#include "stm32f4xx_hal.h"

static void Timer1_Callback (void const *arg);
static void Timer2_Callback (void const *arg);
int Init_Timers_Rebotes (void);

#endif
