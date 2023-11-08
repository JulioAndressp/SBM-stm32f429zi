#include "cmsis_os2.h"                          // CMSIS RTOS header file
 
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_ThreadLEDS;                        // thread id
uint8_t g_expired=0;
osTimerId_t timer_led;
 
void Thread (void *argument);                   // thread function
static void TimerLed_Callback(void);
 
int Init_ThreadLEDS (void) {
  timer_led = osTimerNew((osTimerFunc_t)&TimerLed_Callback,osTimerPeriodic,0x00,NULL);
  tid_ThreadLEDS = osThreadNew(Thread, NULL, NULL);
  if (tid_ThreadLEDS == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thread (void *argument) {
 
  while (1) {
    ; // Insert thread code here...
    osThreadYield();                            // suspend thread
  }
}

static void TimerLed_Callback(void)
{
  g_expired=1;
}
