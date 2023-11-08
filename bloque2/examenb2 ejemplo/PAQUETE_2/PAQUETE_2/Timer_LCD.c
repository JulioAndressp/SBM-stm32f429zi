#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "Timer_LCD.h"
#include "stm32f4xx_hal.h" //La incluimos para poder usar el HAL, solo si es necesario
/*----------------------------------------------------------------------------
 *      Timer: Sample timer functions
 *---------------------------------------------------------------------------*/
 
/*----- One-Shoot Timer Example -----*/
osTimerId_t tim_id1;                            // timer id
static uint32_t exec1;                          // argument for the timer call back function

osTimerId_t tim_id2;                            // timer id
static uint32_t exec2;                          // argument for the timer call back function

// One-Shoot Timer Function
static void Timer1_Callback (void const *arg) {
  pintar_pelota();
}
 
// Periodic Timer Function
static void Timer2_Callback (void const *arg) {
  //Examen_BT2();
//Examen_BT2_Posible_2();
}
 


int Init_Timers (void) {

 
  // Create one-shoot timer
  exec1 = 1U;
  tim_id1 = osTimerNew((osTimerFunc_t)&Timer1_Callback, osTimerOnce, &exec1, NULL);
  if (tim_id1 != NULL) {  // One-shot timer created
    // start timer with delay 100ms
   osTimerStart(tim_id1, 100U); 

  }
 
  // Create periodic timer
  exec2 = 2U;
  tim_id2 = osTimerNew((osTimerFunc_t)&Timer2_Callback, osTimerPeriodic, &exec2, NULL);
  if (tim_id2 != NULL) {  // Periodic timer created
    // start timer with periodic 1000ms interval
    osTimerStart(tim_id2, 2000U);            

  }
  return NULL;
}
