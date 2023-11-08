#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "Thled3.h"

/*----------------------------------------------------------------------------
 *      Thread 3 'Thread_Name': Thled3
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thread3;                        // thread id
 int Init_Thread3 (void);	

void Thled3 (void *argument);                   // thread function
 
int Init_Thread3 (void) {
 
  tid_Thread3 = osThreadNew(Thled3, NULL, NULL);
  if (tid_Thread3 == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thled3 (void *argument) {
 
  while (1) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
		osDelay(287); //RETARDO CON HILOS
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
		osDelay(287); //RETARDO CON HILOS

		// Insert thread code here...
    osThreadYield();                            // suspend thread
  }
}
