#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "joystick.h"

/*----------------------------------------------------------------------------
 *      Timer: JOYSTICK GESTION DE REBOTES
 *---------------------------------------------------------------------------*/
 
/*----- One-Shoot Timer Example -----*/
osTimerId_t tim_id1_long_short;                            // timer id

static uint32_t exec1;													// argument for the timer call back function

osTimerId_t tim_id2_rebotes;                            // timer id
static uint32_t exec2;                          // argument for the timer call back function


osMessageQueueId_t mid_PulsadorQueue;  //Creamos esta variable aqui para no tener que declarala dos veces ya que pertenece a la Queue

MSGQUEUE_OBJ_t_joy msg_joystick; //Variable con el typedef que tiene ID diciendo la pulsacion


int cont_center=0; //Contador para definir las pulsaciones centrales


static void Timer1_Callback (void const *arg) {
	
  cont_center++;
	
		if (cont_center == 19){ 			//19 porque los primeros 50 ms son de gestion de pulsaciones
			
			msg_joystick.Idx=IDENTIFICADOR_CENTRO_LARGA;
			osMessageQueuePut(mid_PulsadorQueue, &msg_joystick, 0U, 0U); //Introducimos a la cola una pulsacion
			osTimerStop(tim_id1_long_short);														 //Paramos el timer para que no siga contando
			
		}
			
		if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15)== GPIO_PIN_SET){	//SI SIGUE PULSADO
			osTimerStart(tim_id1_long_short, 50U); 								//REARRANCO EL TIMER SI EL BOTON SIGUE PULSADO

		}else {
			
			if (cont_center < 19){ 	//Confirmamos si la pulsacion ha sido corta 
				
				msg_joystick.Idx=IDENTIFICADOR_CENTRO_CORTA;
				osMessageQueuePut(mid_PulsadorQueue, &msg_joystick, 0U, 0U); //Introducimos a la cola la pulsacion
			}
			osTimerStop(tim_id1_long_short);

			}
}


//GESTION DE REBOTES DEL JOYSTICK

 

static void Timer2_Callback (void const *arg) {
	
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)== GPIO_PIN_SET) //PULSADOR ARRIBA
	{
		msg_joystick.Idx=IDENTIFICADOR_ARRIBA;
   osMessageQueuePut(mid_PulsadorQueue, &msg_joystick, 0U, 0U);
	}
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)== GPIO_PIN_SET) //PULSADOR DERECHA
	{
		msg_joystick.Idx=IDENTIFICADOR_DERECHA;
   osMessageQueuePut(mid_PulsadorQueue, &msg_joystick, 0U, 0U);
	}
		if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12)== GPIO_PIN_SET)	//PULSADOR ABAJO
	{
		msg_joystick.Idx=IDENTIFICADOR_ABAJO;
		osMessageQueuePut(mid_PulsadorQueue, &msg_joystick, 0U, 0U);
		
	}
			if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14)== GPIO_PIN_SET) //PULSADOR IZQUIERDA
	{
		msg_joystick.Idx=IDENTIFICADOR_IZQUIERDA;
    osMessageQueuePut(mid_PulsadorQueue, &msg_joystick, 0U, 0U);
	}
			if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15)== GPIO_PIN_SET) //PULSADOR CENTRAL
	{
			cont_center=0;
			osTimerStart(tim_id1_long_short, 50U); //ARRANCO EL TIMER PARA VER CUANTO TIEMPO ESTAN LAS PULSACIONES
	}
				
}



int Init_Timers_Joystick (void) {
	
  osStatus_t status;                            // function return status
 
// Timer periodico para contar pulsaciones largas o cortas
	
  exec1 = 1U;																						
  tim_id1_long_short = osTimerNew((osTimerFunc_t)&Timer1_Callback, osTimerPeriodic, &exec1, NULL);
   
  //Timer one-shot para gestionar el JOYSTICK la gestion de rebotes
		
  exec2 = 2U;
  tim_id2_rebotes = osTimerNew((osTimerFunc_t)&Timer2_Callback, osTimerOnce, &exec2, NULL);
  if (tim_id2_rebotes != NULL) {  // Periodic timer created
    // start timer with one-shot 50ms interval
    status = osTimerStart(tim_id2_rebotes, 50U);            
    if (status != osOK) {
      return -1;
    }
  }
  return NULL;
}


/*----------------------------------------------------------------------------
 *      Message Queue creation & usage "JOYSTICK"
 *---------------------------------------------------------------------------*/

osThreadId_t tid_Thread_MsgQueue_Joystick;              // thread id 1

 
int Init_MsgQueue_Pulsadores (void) {
 
  mid_PulsadorQueue = osMessageQueueNew(MSGQUEUE_OBJECTS_JOY, sizeof(MSGQUEUE_OBJ_t_joy), NULL);

  tid_Thread_MsgQueue_Joystick = osThreadNew(Thread_MsgQueue_Joystick, NULL, NULL);

  return(0);
}
 
void Thread_MsgQueue_Joystick (void *argument) {
	

  while (1) {
		
	osThreadFlagsWait(GESTION_PULSADOR,osFlagsWaitAny,osWaitForever);
	osTimerStart(tim_id2_rebotes, 50U);
				
    osThreadYield();      			// suspend thread
			
    }
  }


