#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "stm32f4xx_hal.h"
#include "ThreadLeds.h" 
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_ThreadLEDS;                        // thread id
uint8_t g_expired=0;
osTimerId_t timer_led;
 
void Threadleds (void *argument);                   // thread function
static void TimerLed_Callback(void);
 
int Init_ThreadLEDS (void) {
  timer_led = osTimerNew((osTimerFunc_t)&TimerLed_Callback,osTimerPeriodic,0x00,NULL);
  tid_ThreadLEDS = osThreadNew(Threadleds, NULL, NULL);
  if (tid_ThreadLEDS == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Threadleds (void *argument) {
  osTimerStart(timer_led,3000U);
  while (1) {
    osThreadYield();                            // suspend thread
  }
}

static void TimerLed_Callback(void)
{
  g_expired=1;
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
}


