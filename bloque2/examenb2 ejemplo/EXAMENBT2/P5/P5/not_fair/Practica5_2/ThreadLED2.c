#include "cmsis_os2.h"// CMSIS RTOS header file
#include "ThreadLED2.h"
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': LED2
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thread_LED2;                        // thread id
 
void Thread_LED2 (void *argument);                   // thread function
 
int Init_Thread_LED2 (void) {
 
  tid_Thread_LED2 = osThreadNew(Thread_LED2, NULL, NULL);
  if (tid_Thread_LED2 == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thread_LED2 (void *argument) {
 
  while (1) {
    
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
		osDelay(137);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
		osDelay(137);
    osThreadYield();                            // suspend thread
  }
}
