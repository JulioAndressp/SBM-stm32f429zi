#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "clock.h"

/*----------------------------------------------------------------------------
 *      Timer: Clock
 *---------------------------------------------------------------------------*/
 
/*----- Periodic Timer Example -----*/
osTimerId_t tim_id_clock;                            // timer id
static uint32_t exec2;                          // argument for the timer call back function

 int horas=0;
 int minutos=0;
 int segundos=0;

// Periodic Timer Function
static void Timer2_Callback (void const *arg) {

		segundos++;
	
	if(segundos>59){
		segundos=0;
		minutos++;
		
			if(minutos>59){
				minutos=0;
				horas++;
			}
			
			if(horas>23){
				horas=0;
			}
	}
	
}
 
// Example: Create and Start timers
int Init_Timers_Clock (void) {
	
  osStatus_t status;                            // function return status
 
  // Create periodic timer
	
  exec2 = 2U;
	
  tim_id_clock = osTimerNew((osTimerFunc_t)&Timer2_Callback, osTimerPeriodic, &exec2, NULL);
	
  if (tim_id_clock != NULL) {  // Periodic timer created

    status = osTimerStart(tim_id_clock, 1000U);   
		
    if (status != osOK) {
      return -1;
    }
  }
  return NULL;
}
