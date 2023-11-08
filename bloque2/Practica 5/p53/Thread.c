#include "cmsis_os2.h"                          // CMSIS RTOS header file
 
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thread;                        // thread id
 
void Thread (void *argument);                   // thread function
 
int Init_Thread (void) {
 
  tid_Thread = osThreadNew(Thread, NULL, NULL);
  if (tid_Thread == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thread (void *argument) {
 
  while (1) {
    HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
    osDelay(200);
    HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
    osDelay(800);
    osThreadYield();                            // suspend thread
  }
}
