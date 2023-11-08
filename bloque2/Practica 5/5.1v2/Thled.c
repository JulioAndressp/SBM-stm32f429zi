#include "cmsis_os2.h"    // CMSIS RTOS header file
#include "main.h"
#include "Thled.h"
 
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thled;                        // thread id
 
void Thread (void *argument);                   // thread function
void Pin_config(void);
int Init_Thled (void);

int Init_Thled (void) {
 Pin_config();
  tid_Thled = osThreadNew(Thread, NULL, NULL);
  if (tid_Thled == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thread (void *argument) {
 
  while (1) {

    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
		osDelay(200);
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
		osDelay(800);
    osThreadYield();                            // suspend thread
  }
}
	
 void Pin_config(void){			
		static GPIO_InitTypeDef GPIO_InitStruct;
		
		// Leds
		__HAL_RCC_GPIOB_CLK_ENABLE();
		
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Pin = GPIO_PIN_0;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


}
