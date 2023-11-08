#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "ThreadLED1.h"
#include "Timer.h"
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': ThreadLED1
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_LED1;    // thread id

 
int Init_Thread_LED1 (void) {
 
  tid_LED1 = osThreadNew(Thread_LED1, NULL, NULL);
  if (tid_LED1 == NULL) {
    return(-1);
  }
 
  return(0);
}
 
extern osTimerId_t tim_id1;  

void Thread_LED1 (void *argument) {
	
 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
	Init_Timers();
	
  while (1) {
		
		osTimerStart(tim_id1, 3000U);
		osThreadFlagsWait(ARRANCATIMER,osFlagsWaitAny,osWaitForever);


		
    osThreadYield();                            // suspend thread
  }
}
