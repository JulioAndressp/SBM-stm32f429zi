#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "ThreadBarrido.h"
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thread2;                        // thread id
 
void Thread_Barrido (void *argument);                   // thread function
 
int Init_Thread_Barrido (void) {
 
  tid_Thread2 = osThreadNew(Thread_Barrido, NULL, NULL);
  if (tid_Thread2 == NULL) {
    return(-1);
  }
 
  return(0);
}
 

extern osThreadId_t tid_Thread;
void Thread_Barrido (void *argument) {
	
 	osThreadFlagsWait(BARRIDO_INVERSO,osFlagsWaitAny,osWaitForever);
	
  while (1) {

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
		osDelay(250);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
		osDelay(250);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		osDelay(250);
	
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
		osDelay(250);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
		osDelay(250);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		osDelay(250);
		
		osThreadFlagsSet(tid_Thread,VUELTA);
		osThreadFlagsWait(BARRIDO_INVERSO,osFlagsWaitAny,osWaitForever);
		
    osThreadYield();                            // suspend thread
  }
}

