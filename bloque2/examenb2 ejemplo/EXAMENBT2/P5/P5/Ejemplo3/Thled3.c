#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "Thled3.h"

/*----------------------------------------------------------------------------
 *      Thread 3 'Thread_Name': Thled3
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thread3;                        // thread id
 int Init_Thread3 (void);	
 int cnt_led3=0;
 extern int cnt_led2;
 extern osThreadId_t tid_Thread2;

void Thled3 (void *argument);                   // thread function
 
int Init_Thread3 (void) {
 
  tid_Thread3 = osThreadNew(Thled3, NULL, NULL);
  if (tid_Thread3 == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thled3 (void *argument) {
 
		
 osThreadFlagsWait(startLED3,osFlagsWaitAny,osWaitForever);  //Esperar fuera del while
	
  while (1) {
		if (cnt_led3 < 30){ //30 ciclos y se detiene el led3

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
		osDelay(287); //RETARDO CON HILOS
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
		osDelay(287); //RETARDO CON HILOS
			if (cnt_led3 == 25){
			osThreadFlagsSet(tid_Thread2,startLED2); //Despues de 25 ciclos del LED3 el LED2 deberá comenzar nuevamente su parpadeo 20 ciclos
			cnt_led2=0;	
			}
			cnt_led3++;
		}else
		 osThreadFlagsWait(startLED3,osFlagsWaitAny,osWaitForever);  //Esperaramos hasta otros 15 ciclos del LED2
		// Insert thread code here...
    osThreadYield();                            // suspend thread
  }
}
