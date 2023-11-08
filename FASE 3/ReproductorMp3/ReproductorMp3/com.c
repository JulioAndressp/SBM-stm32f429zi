#include "Principal.h"

 osThreadId_t tid_myUART_Thread; 
/* USART Driver */
extern ARM_DRIVER_USART Driver_USART3;
 osThreadId_t tid_ComUART;
 
void myUSART3_callback(uint32_t event)
{
  uint32_t mask;
  mask = ARM_USART_EVENT_RECEIVE_COMPLETE  |
         ARM_USART_EVENT_TRANSFER_COMPLETE |
         ARM_USART_EVENT_SEND_COMPLETE     |
         ARM_USART_EVENT_TX_COMPLETE       ;
  if (event & mask) {
    /* Success: Wakeup Thread */
    osThreadFlagsSet(tid_myUART_Thread, 0x01);
  }
  if (event & ARM_USART_EVENT_RX_TIMEOUT) {
    __breakpoint(0);  /* Error: Call debugger or replace with custom error handling */
  }
  if (event & (ARM_USART_EVENT_RX_OVERFLOW | ARM_USART_EVENT_TX_UNDERFLOW)) {
    __breakpoint(0);  /* Error: Call debugger or replace with custom error handling */
  }
}
 
/* CMSIS-RTOS Thread - UART command thread */
void myUART_Thread(void *argument)
{
    static ARM_DRIVER_USART * USARTdrv = &Driver_USART3;
    //ARM_DRIVER_VERSION     version;
    //ARM_USART_CAPABILITIES drv_capabilities;
		char frase[40];
  #ifdef DEBUG
    version = USARTdrv->GetVersion();
    if (version.api < 0x200)   /* requires at minimum API version 2.00 or higher */
    {                          /* error handling */
        return;
    }
    drv_capabilities = USARTdrv->GetCapabilities();
    if (drv_capabilities.event_tx_complete == 0)
    {                          /* error handling */
        return;
    }
  #endif
 
    /*Initialize the USART driver */
    USARTdrv->Initialize(myUSART3_callback);
    /*Power up the USART peripheral */
    USARTdrv->PowerControl(ARM_POWER_FULL);
    /*Configure the USART to 4800 Bits/sec */
    USARTdrv->Control(ARM_USART_MODE_ASYNCHRONOUS |
                      ARM_USART_DATA_BITS_8 |
                      ARM_USART_PARITY_NONE |
                      ARM_USART_STOP_BITS_1 |
                      ARM_USART_FLOW_CONTROL_NONE, 9600);
     
    /* Enable Receiver and Transmitter lines */
    USARTdrv->Control (ARM_USART_CONTROL_TX, 1);
    USARTdrv->Control (ARM_USART_CONTROL_RX, 1);
 
    USARTdrv->Send("\nPress Enter to receive a message", 34);
     osStatus_t status;
    while (1)
    {
				status = osMessageQueueGet(mid_JoystickCOM, &msgcom, NULL, osWaitForever);
				if(status == osOK)
				{
					if(msgcom.Idx == 1U)
					{
							sprintf(frase,"\n\r%.2d:%.2d:%.2d -> %.2X  %.2X  %.2X  %.2X  %.2X  %.2X  %.2X  %.2X \n\r",horas,minutos,segundos,cmd[0],cmd[1],cmd[2],cmd[3],cmd[4],cmd[5],cmd[6],cmd[7]);						//	USARTdrv->Send(&frase,strlen(frase));
							USARTdrv->Send(&frase,strlen(frase));			
					}

 
				}
			}
}
/*---------------------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread && Message Queue creation & usage
 *---------------------------------------------------------------------------------------*/
 
int Init_ComUART (void) {
	
	  tid_myUART_Thread = osThreadNew(myUART_Thread, NULL, NULL);
  if (tid_myUART_Thread == NULL) {
    return(-1);
  }
 
  return(0);
}
