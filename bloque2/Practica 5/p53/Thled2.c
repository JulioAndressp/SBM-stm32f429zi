#include "cmsis_os2.h"   
#include "stm32f4xx_hal.h"
#include "Thled1.h"
#include "Thled2.h"
#include "Thled3.h"

// CMSIS RTOS header file
 
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 extern tid_Thled3;
 osThreadId_t tid_Thled2;                         // thread id
 
void Thled2 (void *argument);                   // thread function
int contador2;
int Init_Thled2 (void) {
 contador2=0;
	osThreadFlagsWait(S_LED2,0,osWaitForever);
  tid_Thled2 = osThreadNew(Thled2, NULL, NULL);
  if (tid_Thled2 == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thled2(void *argument) {
 
  while (1) {
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
    osDelay(137);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
    osDelay(137);
		
		if(contador2!=20|contador2!=15) 
		{contador2++;}
		else if(contador2==15){osThreadFlagSet(tid_Thled3,S_LED3);
			}
			else {contador2=0;
				osThreadFlagsWait(S_LED2,0,osWaitForever);//off thled2
				contador2++;
				}
    ; // Insert thread code here...
    osThreadYield();                            // suspend thread
  }
}
