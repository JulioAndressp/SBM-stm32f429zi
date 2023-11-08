#include "cmsis_os2.h"                          // CMSIS RTOS header file
 #include "Thjoy.h"           
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Thjoy
 *---------------------------------------------------------------------------*/
 
osThreadId_t Thjoy_Thread;                        // thread id
int terminar = 0;

int Init_Thread (void) {
 
  Thjoy_Thread = osThreadNew(Thjoy, NULL, NULL);
	
  if (Thjoy_Thread == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thjoy (void *argument) {
 
	 osThreadFlagsWait(PRUEBA,osFlagsWaitAny,osWaitForever);
	
  while (1){

	
    osThreadYield();                            // suspend thread
  }
}
