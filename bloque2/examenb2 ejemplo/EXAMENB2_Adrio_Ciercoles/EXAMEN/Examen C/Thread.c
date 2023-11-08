#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "Thread.h" 
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thread_Ej;                        // thread id
extern osTimerId_t tim_id2;
extern osTimerId_t tim_id1;

void Thread_Ej (void *argument);                   // thread function
 
int Init_Thread_Ej (void) {
 
  tid_Thread_Ej = osThreadNew(Thread_Ej, NULL, NULL);
  if (tid_Thread_Ej == NULL) {
    return(-1);
  }
 
  return(0);
}


 int parar = 0;
void Thread_Ej (void *argument) {
  Init_Timers_Ej ();
	
  while (1) {
		if (parar == 0){
    osThreadFlagsWait(EJEMPLO,osFlagsWaitAny,osWaitForever);
		osTimerStop(tim_id2);
      osTimerStart(tim_id1, 2000U);
			parar = 1;
			
		} else if (parar == 1){
			osThreadFlagsWait(EJEMPLO,osFlagsWaitAny,osWaitForever);
			parar = 0;
			osTimerStart(tim_id2, 500U);
      osTimerStart(tim_id1, 2000U);
		}
    osThreadYield();                            // suspend thread
  }
}