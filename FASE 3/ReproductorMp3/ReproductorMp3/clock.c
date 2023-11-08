#include "Principal.h"

/* Variables ------------------------------------------------------------------*/
int horas = 0;
int minutos = 0;
int segundos = 0;
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_clock;                        // thread id

void clock (void *argument);                   // thread function

/*----- Periodic Timer Example -----*/
osTimerId_t tim_id2;                            // timer id
static uint32_t exec2;                          // argument for the timer call back function
 
int Init_clock (void) {
 
  tid_clock = osThreadNew(clock, NULL, NULL);
  if (tid_clock == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void clock (void *argument) {
		clock_timer();
  while (1) {
		osThreadFlagsWait(START_CLOCK,osFlagsWaitAny,osWaitForever);
 		osTimerStart(tim_id2,1000U);
    osThreadYield();                            // suspend thread
  }
}
/*----------------------------------------------------------------------------
 *      Timer: Sample timer functions
 *---------------------------------------------------------------------------*/
 
// Periodic Timer Function
static void Timer2_Callback (void const *arg) {
    
  if((horas==23)&&(minutos==59)&&(segundos==59))
		{
			horas=0;
			minutos=0;
			segundos=0;
			
			
		} 
		if((minutos==59)&&(segundos==59))
		{
			minutos=0;
			segundos=0;
			horas++;
		}
		if(segundos==59)
		{
			minutos++;
		  segundos=0;
		}
		else
		{
			segundos++;
		}
		
	osThreadFlagsSet(tid_clock,START_CLOCK);
	osThreadFlagsSet(tid_Principal,ACTUALIZALCD);//Mandamos esta señal para que se actualice también el LCD cada segundo y no se vea afectada la resolución del mismo.
}

int clock_timer (void) {
  osStatus_t status;                            // function return status 
  // Create periodic timer
  exec2 = 2U;
  tim_id2 = osTimerNew((osTimerFunc_t)&Timer2_Callback, osTimerPeriodic, &exec2, NULL);
  if (tim_id2 != NULL) {  // Periodic timer created
    // start timer with periodic 1000ms interval
    status = osTimerStart(tim_id2, 1000U);            
    if (status != osOK) {
      return -1;
    }
  }
  return NULL;
}
