#include "Com-pc.h"
#include "Driver_USART.h"
 

osThreadId_t          tid_ThUsart; 
osMessageQueueId_t    mid_comQueue;
osEventFlagsId_t ThCom_id; 


extern ARM_DRIVER_USART Driver_USART3;
static ARM_DRIVER_USART * USARTdrv = &Driver_USART3;


void ThreadCom (void *argument);
static void init_Usart(void);
int Init_Thcom (void);

//static void Thcom (void *argument);

int Init_Thcom(void) {
	tid_ThUsart = osThreadNew(ThreadCom, NULL, NULL);
	mid_comQueue = osMessageQueueNew(1,50,NULL);
	init_Usart();
	return(0);
}
 
void myUSART_callback(uint32_t event)
{
  uint32_t mask;
  mask = ARM_USART_EVENT_RECEIVE_COMPLETE  | //Recepción completada,para la sd
         ARM_USART_EVENT_TRANSFER_COMPLETE |//Transferencia completada
         ARM_USART_EVENT_SEND_COMPLETE     |//Envío completado
         ARM_USART_EVENT_TX_COMPLETE       ;//Transmisión completada (opcional)
  if (event & mask) {
 osThreadFlagsSet(tid_ThUsart, 0x0001U); /* A */
  osDelay(2U);
   // osSignalSet(Thcom, 0x01);
  }
  if (event & ARM_USART_EVENT_RX_TIMEOUT) {
    __breakpoint(0);  /* Error: Call debugger or replace with custom error handling */
  }
  if (event & (ARM_USART_EVENT_RX_OVERFLOW | ARM_USART_EVENT_TX_UNDERFLOW)) {
    __breakpoint(0);  /* Error: Call debugger or replace with custom error handling */
  }
}




static void init_Usart(void){/////TERATERM
  USARTdrv ->Initialize(myUSART_callback);	
	USARTdrv ->PowerControl(ARM_POWER_FULL);
	USARTdrv->Control(ARM_USART_MODE_ASYNCHRONOUS |
                      ARM_USART_DATA_BITS_8 |
                      ARM_USART_PARITY_NONE |
                      ARM_USART_STOP_BITS_1 |
                      ARM_USART_FLOW_CONTROL_NONE, 9600);
	USARTdrv->Control (ARM_USART_CONTROL_TX, 1);
  USARTdrv->Control (ARM_USART_CONTROL_RX, 0);
  
		USARTdrv->Send("Teraterm Configurado\n", 23);

}

 static void ThreadCom (void *argument){//dato apunta al buffer
	 char comando[9]="\r\nhola";
	 char msm[22];
	 //sprintf(hora,"\r\n00:%d:%d:%D<---C0 04 00 00 01 84 D4 40 00 00 00",horas,minutos,Segundos...); crear funcion o queue donde se adjunten los comandos

	osThreadFlagsWait(0x0001U, osFlagsWaitAny, osWaitForever);
	USARTdrv->Send(comando, 9);
	osMessageQueueGet(mid_comQueue,&msm,NULL,osWaitForever);
	USARTdrv->Send(msm, 22);
	osDelay(20);
}	
	
