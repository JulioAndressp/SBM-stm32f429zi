#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "Thled2.h"

/*----------------------------------------------------------------------------
 *      Thread 2 'Thread_Name': Thled2
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thread2;                        // thread id
 int Init_Thread2 (void);	

void Thled2 (void *argument);                   // thread function
 
int Init_Thread2 (void) {
 
  tid_Thread2 = osThreadNew(Thled2, NULL, NULL);
  if (tid_Thread2 == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thled2 (void *argument) {
 
  while (1) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
		osDelay(137); //RETARDO CON HILOS
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
		osDelay(137); //RETARDO CON HILOS

		// Insert thread code here...
    osThreadYield();                            // suspend thread
  }
}
