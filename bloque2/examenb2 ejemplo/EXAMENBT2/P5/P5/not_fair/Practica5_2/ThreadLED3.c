#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "ThreadLED3.h"

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thread_LED3;                        // thread id
 
void Thread_LED3 (void *argument);                   // thread function
 
int Init_Thread_LED3 (void) {
 
  tid_Thread_LED3 = osThreadNew(Thread_LED3, NULL, NULL);
  if (tid_Thread_LED3 == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thread_LED3 (void *argument) {
 
  while (1) {
  
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
		osDelay(287);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
		osDelay(287);
    osThreadYield(); 
                          // suspend thread
  }
}
