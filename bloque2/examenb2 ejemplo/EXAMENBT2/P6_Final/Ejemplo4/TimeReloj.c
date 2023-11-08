#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "lcd.h"
#include "TimeReloj.h"
/*----------------------------------------------------------------------------
 *      Timer: Sample timer functions
 *---------------------------------------------------------------------------*/
 
/*----- One-Shoot Timer Example -----*/
osTimerId_t tim_id1;                            // timer id
static uint32_t exec1;                          // argument for the timer call back function

int minutos_uni=2;
int minutos_dec=0;
int segundos=0;
int cont=0;


char frase[60];

extern osThreadId_t tid_Thread;  

osTimerId_t tim_id2;                            // timer id
static uint32_t exec2;                          // argument for the timer call back function

// One-Shoot Timer Function
static void Timer1_Callback (void const *arg) {

	if(segundos == 0 && minutos_uni == 0){
		osTimerDelete (tim_id1);
		osThreadFlagsSet(tid_Thread,BARRIDO);
	}
	
	if(minutos_uni == 1){
		if(segundos%4 == 0){
			cont=0;
			osTimerStart(tim_id2, 125U);
		}
	}		
	
	if(segundos > 9){
	sprintf(frase, "Crono -> %d%d:%d",minutos_dec,minutos_uni,segundos);
	EscribeFrase(frase,0);
	}
	else{
	sprintf(frase, "Crono -> %d%d:%d%d",minutos_dec,minutos_uni,0,segundos);
	EscribeFrase(frase,0);
	}

if(segundos==00){
	segundos=60;
	minutos_uni--;
	}
	segundos--;

  osTimerStart(tim_id1, 1000U);
	
}
 
/*----- Periodic Timer Example -----*/
bool mux = false;

// Periodic Timer Function
static void Timer2_Callback (void const *arg) {


	
	if (cont < 9){
	if(mux == false){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	osTimerStart(tim_id2, 125U);
	mux = true;
		cont++;
	}
	else{

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
	osTimerStart(tim_id2, 125U);
	mux = false;
	}
}else
	osTimerStop (tim_id2);

	  
}
 
// Example: Create and Start timers
int Init_Timer_Reloj (void) {
  osStatus_t status;                            // function return status
 
  // Create one-shoot timer
  exec1 = 1U;
  tim_id1 = osTimerNew((osTimerFunc_t)&Timer1_Callback, osTimerOnce, &exec1, NULL);
  if (tim_id1 != NULL) {  // One-shot timer created
    // start timer with delay 100ms
    status = osTimerStart(tim_id1, 100U); 
    if (status != osOK) {
      return -1;
    }
  }
 
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
