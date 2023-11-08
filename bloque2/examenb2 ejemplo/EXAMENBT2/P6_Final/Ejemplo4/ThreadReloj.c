#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "ThreadReloj.h"
#include "TimeReloj.h"
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thread;                        // thread id
 
 
int Init_Thread (void) {
 
  tid_Thread = osThreadNew(Thread_Reloj, NULL, NULL);
  if (tid_Thread == NULL) {
    return(-1);
  }
 
  return(0);
}
 extern osTimerId_t tim_id1;  

extern osThreadId_t tid_Thread2;   

void Thread_Reloj (void *argument) {
	

	Init_Timer_Reloj(); //Inicializamos TIMER RELOJ
	osTimerStart(tim_id1, 1000U);
	
	osThreadFlagsWait(BARRIDO,osFlagsWaitAny,osWaitForever);
	
  while (1) {

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		osDelay(250);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
		osDelay(250);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
		osDelay(250);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		osDelay(250);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
		osDelay(250);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
		osDelay(250);
		
		osThreadFlagsSet(tid_Thread2,BARRIDO_INVERSO);
		osThreadFlagsWait(VUELTA,osFlagsWaitAny,osWaitForever);
		
    osThreadYield();                            // suspend thread
  }
}
