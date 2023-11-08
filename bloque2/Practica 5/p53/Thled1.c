#include "cmsis_os2.h"                         // CMSIS RTOS header file
#include "stm32f4xx_hal.h"
#include "Thled1.h"// CMSIS RTOS header file
#include "Thled2.h"

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 extern tid_Thled2;
 osThreadId_t tid_Thled1;                        // thread id
 	
void Thread (void *argument);                   // thread function
 int contador1;
int Init_Thled1 (void) {
 contador1=0;
  tid_Thled1 = osThreadNew(Thread, NULL, NULL);
  if (tid_Thled1 == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thread (void *argument) {
 
  while (1) {
    ; // Insert thread code here...
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
    osDelay(200);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
    osDelay(800);
		
		
		if (contador1==5){
			osThreadFlagSet(tid_Thled2,S_LED2);
			contador1++;
		}
		else{contador1++;}
    osThreadYield();                            // suspend thread
  }
}
