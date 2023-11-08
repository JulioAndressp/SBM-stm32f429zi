#include "cmsis_os2.h"   
#include "stm32f4xx_hal.h"
#include "Thled1.h"// CMSIS RTOS header file
 
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thled2;                        // thread id
 
void Thled2 (void *argument);                   // thread function
 
int Init_Thled2 (void) {
 
  tid_Thled2 = osThreadNew(Thled2, NULL, NULL);
  if (tid_Thled2 == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thled2(void *argument) {
 
  while (1) {
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
    osDelay(137);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
    osDelay(137);
    ; // Insert thread code here...
    osThreadYield();                            // suspend thread
  }
}
