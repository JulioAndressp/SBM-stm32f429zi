#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "com.h"

osThreadId_t tid_Thread_COM;

osMessageQueueId_t mid_COM_Queue;                // message queue id
 


MSGQUEUE_OBJ_t_COM msg_com;

extern int horas,minutos,segundos;
 
/* USART Driver */
extern ARM_DRIVER_USART Driver_USART3;								//Configuración del Driver USART3
static ARM_DRIVER_USART * USARTdrv = &Driver_USART3;	//Configuración del Driver USART3

int Init_Thread_COM(void) {
 
  tid_Thread_COM = osThreadNew(Thread_COM, NULL, NULL);
	
	mid_COM_Queue = osMessageQueueNew(MSGQUEUE_OBJECTS_COM, sizeof(MSGQUEUE_OBJ_t_COM), NULL);
	
  return(0);
}
 
void myUSART_callback(uint32_t event){
	
  uint32_t mask;
  mask = ARM_USART_EVENT_RECEIVE_COMPLETE  |
         ARM_USART_EVENT_TRANSFER_COMPLETE |
         ARM_USART_EVENT_SEND_COMPLETE     |
         ARM_USART_EVENT_TX_COMPLETE       ;
  if (event & mask) {
    /* Success: Wakeup Thread */
    osThreadFlagsSet(tid_Thread_COM, FLAG_COM); //Flag del Callback
  }

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
 
}


void Thread_COM (void *argument) {


	osStatus_t status_com;
	
  
	myUART_Thread(); 
	USARTdrv->Send("\r Teraterm Configurado\n", 23); //Mandamos mensaje para ver que esta configurado
	char message[500];
	
    while (1)
    {
			
				status_com = osMessageQueueGet(mid_COM_Queue, &msg_com, NULL, osWaitForever); //Comprobamos que el mensaje es bueno
			
				if(status_com == osOK) {
					
					sprintf(message,"\r%0.2d:%0.2d:%0.2d --> %.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X \r\n", horas,minutos,segundos,
					msg_com.RDA_REG_WRITE[1],msg_com.RDA_REG_WRITE[0],msg_com.RDA_REG_WRITE[3],msg_com.RDA_REG_WRITE[2],msg_com.RDA_REG_WRITE[5],msg_com.RDA_REG_WRITE[4],
					msg_com.RDA_REG_WRITE[7],msg_com.RDA_REG_WRITE[6],msg_com.RDA_REG_WRITE[9],msg_com.RDA_REG_WRITE[8],msg_com.RDA_REG_WRITE[11],msg_com.RDA_REG_WRITE[10]);
					USARTdrv->Send(message,strlen(message));	 //Enviamos el mensaje
					
     
				}
			}
  }

	
	





