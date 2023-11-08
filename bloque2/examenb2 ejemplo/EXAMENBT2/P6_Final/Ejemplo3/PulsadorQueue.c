#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "PulsadorQueue.h"
#include "lcd.h"
/*----------------------------------------------------------------------------
 *      Creamos una cola para almacenar las pulsaciones generadas
 *---------------------------------------------------------------------------*/
 

 
osMessageQueueId_t mid_PulsadorQueue;                // message queue id
 
 
osThreadId_t tid_Thread_MsgQueue1;              // thread id 1
osThreadId_t tid_Thread_MsgQueue2;              // thread id 2
 
MSGQUEUE_OBJ_t msg;//Ponemos un mensaje con el typedef
 
int Init_MsgQueue_Pulsador (void) {
 
  mid_PulsadorQueue = osMessageQueueNew(MSGQUEUE_OBJECTS, sizeof(MSGQUEUE_OBJ_t), NULL);
  if (mid_PulsadorQueue == NULL) {
    ; // Message Queue object not created, handle failure
  }
 
  tid_Thread_MsgQueue1 = osThreadNew(Thread_MsgQueue1, NULL, NULL);
  if (tid_Thread_MsgQueue1 == NULL) {
    return(-1);
  }
  tid_Thread_MsgQueue2 = osThreadNew(Thread_MsgQueue2, NULL, NULL);
  if (tid_Thread_MsgQueue2 == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thread_MsgQueue1 (void *argument) {
	
  MSGQUEUE_OBJ_t msg;//Variable mensaje
	osStatus_t status;//Creamos variable status
 
  while (1) {
		
	status = osMessageQueueGet(mid_PulsadorQueue, &msg, NULL, osWaitForever); //Este mensaje viene pero es util
		
		if(status == osOK){//Si el status esta okey y listo para recibir
			if(msg.Idx == 0x00)
				EscribeFrase("ARRIBA ESPANA",0);
			if(msg.Idx == 0x01)
				EscribeFrase("DERECHA ESPANA",1);
			if(msg.Idx == 0x02)
				EscribeFrase("ABAJO ESPANA",0);
			if(msg.Idx == 0x03)
				EscribeFrase("IZQUIERDA ESPANA",1);
			if(msg.Idx == 0x04)
				EscribeFrase("CENTRO ESPANA",0);
		}
    osThreadYield();                                            // suspend thread
  }
}
 
void Thread_MsgQueue2 (void *argument) {
  MSGQUEUE_OBJ_t msg;
  osStatus_t status;

  while (1) {
    ; // Insert thread code here...
    status = osMessageQueueGet(mid_PulsadorQueue, &msg, NULL, 0U);   // wait for message
    if (status == osOK) {
      ; // process data
    }
  }
}
