#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "stm32f4xx_hal.h"
#include "Thled2.h"

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/


osThreadId_t tid_Thled2;                        // thread id


void Thled2 (void *argument);                   // thread function


void InitLED2(void);


int Init_Thled2 (void) {
  tid_Thled2 = osThreadNew(Thled2, NULL, NULL);
  if (tid_Thled2 == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thled2 (void *argument) {

  while (1) {
     // Insert thread code here...
   
    
    HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);
    osDelay(200);
    HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);
    osDelay(800);

    //initTimer(800);
   // osThreadYield();                            // suspend thread
  }
}





