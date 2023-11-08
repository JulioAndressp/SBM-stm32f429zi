#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "TimerJoy.h"

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

// One-Shoot Timer Function
static void Timer1_Callback (void const *arg) {
  // add user code here
	
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)== GPIO_PIN_SET) //puerto arriba
	{
		cont_gen++;
		cont_up++;

	}
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)== GPIO_PIN_SET)//puerto derecha
	{
		
		cont_gen++;
		cont_right++;

	}
		if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12)== GPIO_PIN_SET)//puerto abajo
	{
		
		cont_gen++;
		cont_down++;
		
	}
			if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14)== GPIO_PIN_SET) //puerto izquierda
	{
		cont_gen++;
		cont_left++;

	}
			if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15)== GPIO_PIN_SET) //puerto central
	{
		cont_gen++;
		cont_center++;
		
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
