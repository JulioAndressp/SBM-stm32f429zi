#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "TimerJoy.h"
#include "PulsadorQueue.h"

/*----------------------------------------------------------------------------
 *      Timer: Sample timer functions
 *---------------------------------------------------------------------------*/
 
/*----- One-Shoot Timer Example -----*/

int cont_gen;
int cont_up;
int cont_right;
int cont_down;
int cont_left;
int cont_center;

osTimerId_t tim_id1_JOY;                            // timer id
static uint32_t exec1;                          // argument for the timer call back function

extern osMessageQueueId_t mid_PulsadorQueue; //ID de mi Queue
extern MSGQUEUE_OBJ_t msg; //Variable con el typedef que tiene ID y Buffer
	
// One-Shoot Timer Function
static void Timer1_Callback (void const *arg) {
  // add user code here
	
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)== GPIO_PIN_SET) //puerto arriba
	{
		cont_gen++;
		cont_up++;
		
		msg.Buf[0]=0x00U;
		msg.Idx=0x00;
		
    osMessageQueuePut(mid_PulsadorQueue, &msg, 0U, 0U);

	}
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)== GPIO_PIN_SET)//puerto derecha
	{
		
		cont_gen++;
		cont_right++;
		
		msg.Buf[1]=0x01U;
		msg.Idx=0x01;

		osMessageQueuePut(mid_PulsadorQueue, &msg, 0U, 0U); //Introducimos el mensaje en la cola
	}
		if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12)== GPIO_PIN_SET)//puerto abajo
	{
		
		cont_gen++;
		cont_down++;
		
		msg.Buf[2]=0x02U;
		msg.Idx=0x02;

		osMessageQueuePut(mid_PulsadorQueue, &msg, 0U, 0U);//Introducimos el mensaje en la cola
		
	}
			if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14)== GPIO_PIN_SET) //puerto izquierda
	{
		cont_gen++;
		cont_left++;
		
		msg.Buf[3]=0x03U;
		msg.Idx=0x03;

		osMessageQueuePut(mid_PulsadorQueue, &msg, 0U, 0U);//Introducimos el mensaje en la cola

	}
			if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15)== GPIO_PIN_SET) //puerto central
	{
		cont_gen++;
		cont_center++;
		
		msg.Buf[4]=0x04U;
		msg.Idx=0x04;

		osMessageQueuePut(mid_PulsadorQueue, &msg, 0U, 0U);//Introducimos el mensaje en la cola
	}
}
 
/*----- Periodic Timer Example -----*/
osTimerId_t tim_id2_JOY;                            // timer id
static uint32_t exec2;                          // argument for the timer call back function
 
// Periodic Timer Function
static void Timer2_Callback (void const *arg) {
  // add user code here
}
 
// Example: Create and Start timers
int Init_Timers_Rebotes (void) {
  osStatus_t status;                            // function return status
 
  // Create one-shoot timer
  exec1 = 1U;
  tim_id1_JOY = osTimerNew((osTimerFunc_t)&Timer1_Callback, osTimerOnce, &exec1, NULL);
  if (tim_id1_JOY != NULL) {  // One-shot timer created
    // start timer with delay 100ms
    status = osTimerStart(tim_id1_JOY, 100U); 
    if (status != osOK) {
      return -1;
    }
  }
 
  // Create periodic timer
  exec2 = 2U;
  tim_id2_JOY = osTimerNew((osTimerFunc_t)&Timer2_Callback, osTimerPeriodic, &exec2, NULL);
  if (tim_id2_JOY != NULL) {  // Periodic timer created
    // start timer with periodic 1000ms interval
    status = osTimerStart(tim_id2_JOY, 1000U);            
    if (status != osOK) {
      return -1;
    }
  }
  return NULL;
}
