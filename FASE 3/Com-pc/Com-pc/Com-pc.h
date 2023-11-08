#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include "Driver_USART.h"

extern osThreadId_t          tid_ThUsart;                        
extern osMessageQueueId_t    mid_comQueue;

int Init_Thcom (void);
