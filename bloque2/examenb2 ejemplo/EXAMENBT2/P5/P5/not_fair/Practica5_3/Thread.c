#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "Thread.h"

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': LED 1
 *---------------------------------------------------------------------------*/

osThreadId_t tid_ThreadLED1;                        // thread id

int Init_Thread (void);
void ThreadLED1 (void *argument);                   // thread function


int Init_Thread (void) {
 
  tid_ThreadLED1 = osThreadNew(ThreadLED1, NULL, NULL);
  if (tid_ThreadLED1 == NULL) {
    return(-1);
  }
  return(0);
}
 
void ThreadLED1 (void *argument) {
 
  while (1) {
		
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		osDelay(800);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		osDelay(200);
		
		ciclosled1 ++;
		
		if (ciclosled1 == 5)
		{
			osThreadFlagsSet (tid_Thread_LED2,EMPIEZALED2);
		}
		
    osThreadYield();                            // suspend thread
  }
}

