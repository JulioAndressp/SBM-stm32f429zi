#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "ThreadLED3.h"
#include "Timer.h"
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': ThreadLED1
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_LED3;                        // thread id
 
void Thread_LED3 (void *argument);                   // thread function
 
int Init_Thread_LED3 (void) {
 
  tid_LED3 = osThreadNew(Thread_LED3, NULL, NULL);
  if (tid_LED3 == NULL) {
    return(-1);
  }
 
  return(0);
}

extern osTimerId_t tim_id1;  
 
void Thread_LED3 (void *argument) {
 
	osThreadFlagsWait(ARRANCALED3,osFlagsWaitAny,osWaitForever);
	
  while (1) {
		
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
		osTimerDelete (tim_id1);
    osThreadYield();                            // suspend thread
  }
}
