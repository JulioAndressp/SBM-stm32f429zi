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
 

int cnt_led2=0;
extern int cnt_led3;
extern osThreadId_t tid_Thread3;


void Thled2 (void *argument) {
 
	
	 osThreadFlagsWait(startLED2,osFlagsWaitAny,osWaitForever);  //Esperar fuera del while al FLAG 

	
  while (1) {
		
		if (cnt_led2 < 20){ //Hace 20 ciclos desde que le llega la senal

		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
		  osDelay(137); //RETARDO CON HILOS
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
		  osDelay(137);			//RETARDO CON HILOS
			
		  if (cnt_led2 == 15){// Despues de 15 ciclos del LED2 el LED3 deberá comenzar su parpadeo durante 30 ciclos
			  osThreadFlagsSet(tid_Thread3,startLED3);
				cnt_led3=0;
		  }
			
		  cnt_led2++;
	  }
		else
			osThreadFlagsWait(startLED2,osFlagsWaitAny,osWaitForever);  //Esperar fuera del while

			

		
		
		// Insert thread code here...
    osThreadYield();                            // suspend thread
  }
}
