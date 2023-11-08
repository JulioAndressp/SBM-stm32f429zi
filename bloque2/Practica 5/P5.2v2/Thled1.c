#include "cmsis_os2.h"  					// CMSIS RTOS header file
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal.h"
#include "Thled1.h"
 
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thled1;                        // thread id
 
void Thled1 (void *argument);                   // thread function

int Init_Thled1 (void) {
  
  tid_Thled1 = osThreadNew(Thled1, NULL, NULL);
  if (tid_Thled1 == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thled1 (void *argument) {

  while (1) {
     // Insert thread code here...
   
    
    HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
    osDelay(200);
    HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
    osDelay(800);

    //initTimer(800);
   // osThreadYield();                            // suspend thread
  }
}


