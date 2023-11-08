#include "cmsis_os2.h" 
#include "pwm.h"
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thread_Zumbador;                        // thread id

 extern TIM_HandleTypeDef tim2;//Timer
 extern TIM_OC_InitTypeDef octim4;//Canal
	 
int Init_Zumbador(void) {
 
  tid_Thread_Zumbador = osThreadNew(Thread_Zumbador, NULL, NULL);
  if (tid_Thread_Zumbador == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thread_Zumbador (void *argument) {
	
	
  while (1) {
		
		osThreadFlagsWait(PWM_SIGNAL,osFlagsWaitAll,osWaitForever);
		HAL_TIM_PWM_Start(&tim2,TIM_CHANNEL_4);
		osDelay(250);
		HAL_TIM_PWM_Stop(&tim2,TIM_CHANNEL_4);
    osThreadYield();       
		
  }
}





