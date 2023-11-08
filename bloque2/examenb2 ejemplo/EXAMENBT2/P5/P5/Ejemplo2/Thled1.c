#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "Thled1.h"

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Thled1
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thread1;                        // thread id
 int Init_Thread1 (void);	

void Thled1 (void *argument);                   // thread function
 
int Init_Thread1 (void) {
 
  tid_Thread1 = osThreadNew(Thled1, NULL, NULL);
  if (tid_Thread1 == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thled1 (void *argument) {
 
  while (1) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		osDelay(800); //RETARDO CON HILOS
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		osDelay(200); //RETARDO CON HILOS

		// Insert thread code here...
    osThreadYield();                            // suspend thread
  }
}
