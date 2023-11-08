#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "Thread.h"

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thread;                        // thread id
 int Init_Thread (void);	

void Thread1 (void *argument);                   // thread function
 
int Init_Thread (void) {
 
  tid_Thread = osThreadNew(Thread1, NULL, NULL);
  if (tid_Thread == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thread1 (void *argument) {
 
  while (1) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		osDelay(800); //RETARDO CON HILOS
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		osDelay(200); //RETARDO CON HILOS

		// Insert thread code here...
    osThreadYield();                            // suspend thread
  }
}
