/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COM_H
#define __COM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>
#include "Driver_USART.h"
/* Prototypes ------------------------------------------------------------------*/
void myUART_Thread(void *argument);
void myUSART3_callback(uint32_t event);
int  Init_ComUART (void);
#endif /* __COM_H */
