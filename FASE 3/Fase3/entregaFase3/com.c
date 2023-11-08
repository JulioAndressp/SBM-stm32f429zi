#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "com.h"


void Thread_COM(void *argument);
osThreadId_t tid_Thread_COM;
 
/* USART Driver */
extern ARM_DRIVER_USART Driver_USART3;								//Configuración del Driver USART3
static ARM_DRIVER_USART * USARTdrv = &Driver_USART3;	//Configuración del Driver USART3

int Init_Thread_COM(void) {
 
  tid_Thread_COM = osThreadNew(Thread_COM, NULL, NULL);
  if (tid_Thread_COM == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void myUSART_callback(uint32_t event)
{
  uint32_t mask;
	//No se si todos los eventos son necesarios, diría que no
  mask = ARM_USART_EVENT_RECEIVE_COMPLETE  |
         ARM_USART_EVENT_TRANSFER_COMPLETE |
         ARM_USART_EVENT_SEND_COMPLETE     |
         ARM_USART_EVENT_TX_COMPLETE       ;
  if (event & mask) {
    /* Success: Wakeup Thread */
    osThreadFlagsSet(tid_Thread_COM, FLAG_COM); //Flag del Callback
  }
	//Creo que esto tampoco es necesario
//  if (event & ARM_USART_EVENT_RX_TIMEOUT) {
//    __breakpoint(0);  /* Error: Call debugger or replace with custom error handling */
//  }
//  if (event & (ARM_USART_EVENT_RX_OVERFLOW | ARM_USART_EVENT_TX_UNDERFLOW)) {
//    __breakpoint(0);  /* Error: Call debugger or replace with custom error handling */
//  }
}
 
/* CMSIS-RTOS Thread - UART command thread */
void myUART_Thread(void)
{
 
    /*Initialize the USART driver */
    USARTdrv->Initialize(myUSART_callback);
    /*Power up the USART peripheral */
    USARTdrv->PowerControl(ARM_POWER_FULL);
    /*Configure the USART to 9600 Bits/sec */
    USARTdrv->Control(ARM_USART_MODE_ASYNCHRONOUS |
                      ARM_USART_DATA_BITS_8 |
                      ARM_USART_PARITY_NONE |
                      ARM_USART_STOP_BITS_1 |
                      ARM_USART_FLOW_CONTROL_NONE, 9600);
     
    /* Enable Receiver and Transmitter lines */
    USARTdrv->Control (ARM_USART_CONTROL_TX, 1);
    USARTdrv->Control (ARM_USART_CONTROL_RX, 1);
 
	
			/*Mensaje de prueba*/

//    USARTdrv->Send("\nPress Enter to receive a message", 34);
//    osThreadFlagsWait(0x01,osFlagsWaitAll,osWaitForever);
//     
//    while (1)
//    {		
//        USARTdrv->Receive(&cmd, 1);          /* Get byte from UART */
//        osThreadFlagsWait(0x01,osFlagsWaitAll,osWaitForever);
//        if (cmd == 13)                       /* CR, send greeting  */
//        {
//					USARTdrv->Send("\nHello World!", 12);
//          osThreadFlagsWait(0x01,osFlagsWaitAll,osWaitForever);
//        }
// 
//    }
}

/*Funcion para escribir en el Teraterm*/
void WriteCMD(char cmd[]){
	
	USARTdrv->Send(cmd,strlen(cmd)); //Mandamos la cadena y la longitud de la cadena
	
	osThreadFlagsWait(FLAG_COM, osFlagsWaitAny, osWaitForever); //Esperamos al Callback
	memset(cmd,0,strlen(cmd));
}


//QUEUE TERATERM

/*----------------------------------------------------------------------------
 *      Message Queue creation & usage "TeraTerm Comm"
 *---------------------------------------------------------------------------*/
 
osMessageQueueId_t mid_COM_Queue;                // message queue id
 
osThreadId_t tid_ThreadCOM_MsgQueue;              // thread id 

MSGQUEUE_OBJ_t_COM msg_com;
 
void Thread_COM (void *argument) {
	
	myUART_Thread();
  //MSGQUEUE_OBJ_t_COM msg_com;	//Variable mensaje
	//osStatus_t status;			//Creamos variable status

  while (1) {
		
	osThreadFlagsWait(GESTION_COM,osFlagsWaitAny,osWaitForever);	
	//status = osMessageQueueGet(mid_COM_Queue, &msg_com, NULL, osWaitForever);
	osMessageQueueGet(mid_COM_Queue, &msg_com, NULL, osWaitForever);
			
	//if(status == osOK){					//Si el status esta okey y listo para recibir
	WriteCMD(msg_com.message);
		
		
   osThreadYield();
		// suspend thread
    }
  }


