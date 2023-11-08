#include "cmsis_os2.h"                          // CMSIS RTOS header file
 #include "stm32f4xx_hal.h"
#include "Thled1.h"

extern tid_Thled2;
/*---------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
 osThreadId_t tid_Thled3;                        // thread id
 
void Thled3 (void *argument);                   // thread function
 int contador3;
int Init_Thled3 (void) {
 contador3=0;
	osThreadFlagsWait(S_LED3,0,osWaitForever);
  tid_Thled3 = osThreadNew(Thled3, NULL, NULL);
  if (tid_Thled3 == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thled3 (void *argument) {
 
  while (1) {
		
     HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_14);
    osDelay(287);
    HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_14);
    osDelay(287);
		
		if(contador3!=30 |contador3!=25) 
		{contador3++;}
		else if(contador3==25){osThreadFlagSet(tid_Thled2,S_LED2);
			contador3++;}
			else {contador3=0;
			osThreadFlagsWait(S_LED3,0,osWaitForever);
				contador3++;
			}
    osThreadYield();                            // suspend thread
  }
}
