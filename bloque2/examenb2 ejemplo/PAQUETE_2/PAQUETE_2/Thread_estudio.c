#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "Thread_estudio.h"
#include "Timer_LCD.h"
#include "TimerJoy.h"
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Para iniciar el TIMER
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thread_estudio;                        // thread id
extern osTimerId_t tim_id1_JOY;                         // timer id
 
int Init_Thread_Estudio (void) {
 
  tid_Thread_estudio = osThreadNew(Thread_Estudio, NULL, NULL);

  return(0);
}
 
void Thread_Estudio (void *argument) {
	
 Init_Timers();	//TIMER DEL LCD
 Init_Timers_Rebotes(); //TIMER DE LOS REBOTES
	
  while (1) {
		
		osThreadFlagsWait(ARRANCA_TIMER_JOY,osFlagsWaitAny,osWaitForever);
		osTimerStart(tim_id1_JOY, 50U);
		
		
    osThreadYield();                            // suspend thread
  }
}
