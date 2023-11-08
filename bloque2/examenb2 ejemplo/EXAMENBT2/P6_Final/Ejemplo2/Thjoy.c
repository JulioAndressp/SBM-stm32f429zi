#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "Thjoy.h"
#include "TimerJoy.h"
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Thjoy
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thread_JOY;                        // thread id
 

int Init_Thread_Joy (void) {
 
  tid_Thread_JOY = osThreadNew(Thread_JOY, NULL, NULL);
  if (tid_Thread_JOY == NULL) {
    return(-1);
  }
 
  return(0);
}
 

extern osTimerId_t tim_id1_JOY; 

void Thread_JOY (void *argument) {
	
 	Init_Timers_Rebotes();
	
  while (1) {
		
		osThreadFlagsWait(ARRANCA_TIMER_JOY,osFlagsWaitAny,osWaitForever);
		osTimerStart(tim_id1_JOY, 50U);

    osThreadYield();                            // suspend thread
  }
}
