#include "Principal.h"
/*----- HILO & COLA---------*/
osThreadId_t tid_myUART_Thmp3Send;
osThreadId_t tid_myUART_Thmp3Receive;
	#define MSGQUEUE_OBJECTS 16 

/*--------USART Driver----------- */
extern ARM_DRIVER_USART Driver_USART6;
       ARM_DRIVER_USART * USARTdrv = &Driver_USART6;

 unsigned char cmd[8];
 unsigned char trm[10];

 extern GPIO_InitTypeDef GPIO_InitStruct;
 /*----------------------------------------------------------------------------
 *      												CALLBACK
 *---------------------------------------------------------------------------*/

void myUSART_callback(uint32_t event)
{
  
	if(event & ARM_USART_EVENT_TX_COMPLETE)
	{
		osThreadFlagsSet(tid_myUART_Thmp3Send, ARM_USART_EVENT_TX_COMPLETE);
    
	}
		if(event &ARM_USART_EVENT_RECEIVE_COMPLETE)
	{
		osThreadFlagsSet(tid_myUART_Thmp3Receive, ARM_USART_EVENT_RECEIVE_COMPLETE);
	}
}
 /*----------------------------------------------------------------------------
 *      HILO DEL MP3 PARA RECIBIR TRAMAS
 *---------------------------------------------------------------------------*/
void myUART_ThMP3Receive(void *argument)
{
    
    //ARM_DRIVER_VERSION     version;
    //ARM_USART_CAPABILITIES drv_capabilities;
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
    while (1)
    {
              recibir_trama();
							if(trm[6]== 0 & cmd[3]==0x48 )
							{
									modo = REPOSO_TARJETA;									
							}
							if ( trm[3] == 0x3B)
							{
									modo = REPOSO_TARJETA;
							}
						  if ((trm[3] == 0x3A)||(trm[3] == 0x3F))
							{
								  modo = REPOSO;

							}
							if(trm[3]== 0x3D )
							{
											if(actual_folder != 3)
											{
												actual_song++;	
											}
											if(actual_folder == 1 && actual_song == 2)
												actual_song = 0;
											if(actual_folder == 2 && actual_song == 3)
												actual_song = 0;
											
											//Reproducciones
											 if(actual_folder == 1 && actual_song == 0)
											{				
												//Carpeta 1 cancion 0
												msgmp3Send.Buf[5] = 0x04U;									 
												msgmp3Send.Idx = 5U;			
												osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);
																								
												msgprin.Buf[21] = 0x04U;									 
												msgprin.Idx = 21U;			
												osMessageQueuePut(mid_Principal, &msgprin, 0U, 0U);
																
												min_song = 0;
												seg_song = 0;
												osTimerStart(tim_song,1000);
											}
														 if(actual_folder == 1 && actual_song == 1)
											{				
												//Carpeta 1 cancion 1
												msgmp3Send.Buf[6] = 0x05U;									 
												msgmp3Send.Idx = 6U;			
												osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);
												
												msgprin.Buf[21] = 0x04U;									 
												msgprin.Idx = 21U;			
												osMessageQueuePut(mid_Principal, &msgprin, 0U, 0U);
																
												min_song = 0;
												seg_song = 0;
												osTimerStart(tim_song,1000);
											}
														 if(actual_folder == 2 && actual_song == 0)
											{				
												//Carpeta 2 cancion 0
												msgmp3Send.Buf[7] = 0x06U;									 
												msgmp3Send.Idx = 7U;			
												osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);
												
												msgprin.Buf[21] = 0x04U;									 
												msgprin.Idx = 21U;			
												osMessageQueuePut(mid_Principal, &msgprin, 0U, 0U);														
												
												min_song = 0;
												seg_song = 0;
												osTimerStart(tim_song,1000);
											}
											 if(actual_folder == 2 && actual_song == 1)
											{
																//Carpeta 2 cancion 1
												msgmp3Send.Buf[8] = 0x07U;									 
												msgmp3Send.Idx = 8U;			
												osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);
												
												msgprin.Buf[21] = 0x04U;									 
												msgprin.Idx = 21U;			
												osMessageQueuePut(mid_Principal, &msgprin, 0U, 0U);
												
												min_song = 0;
												seg_song = 0;
												osTimerStart(tim_song,1000);

											}		
											 if(actual_folder == 2 && actual_song == 2)
											{
																//Carpeta 2 cancion 2
												msgmp3Send.Buf[9] = 0x08U;									 
												msgmp3Send.Idx = 9U;			
												osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);
												
												
												msgprin.Buf[21] = 0x04U;									 
												msgprin.Idx = 21U;			
												osMessageQueuePut(mid_Principal, &msgprin, 0U, 0U);
												
												min_song = 0;
												seg_song = 0;
												osTimerStart(tim_song,1000);

											}		

											if(actual_folder == 3 && actual_song == 0)
											{
												//Carpeta 3 cancion 0
												msgmp3Send.Buf[10] = 0x09U;									 
												msgmp3Send.Idx = 10U;			
												osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);
											
												msgprin.Buf[21] = 0x04U;									 
												msgprin.Idx = 21U;			
												osMessageQueuePut(mid_Principal, &msgprin, 0U, 0U);
												
												min_song = 0;
												seg_song = 0;
												osTimerStart(tim_song,1000);
											}
										
								
							}

    }
}

/*----------------------------------------------------------------------------
 *      HILO DEL MP3 PARA ENVIAR TRAMAS
 *---------------------------------------------------------------------------*/
void myUART_ThMP3Send(void *argument)
{
    
    //ARM_DRIVER_VERSION     version;
    //ARM_USART_CAPABILITIES drv_capabilities;
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
		
		//Reset
		osDelay(500);
    enviar_trama(CMDRESET,DAT1REST,DAT2REST);
		//Seleccionar la TF
    enviar_trama(CMDSELECTTF,DAT2REST,DAT2SELCTF);
		//Seleccionar El volumen
    enviar_trama(CMDSETVOL,DAT1REST,volumen);
		//Hay TF
    enviar_trama(CMDTFEXIST,DAT1REST,DAT2REST);

		
		osStatus_t status;

		while (1)
    {
        
    status = osMessageQueueGet(mid_JoystickMP3Send, &msgmp3Send, NULL, osWaitForever);
      
    if(status == osOK)
    {
      if(msgmp3Send.Idx == 1U)// darle al play Por primera vez
      {     				
             enviar_trama(CMDPLAYSONG,DAT1REST,DAT2FS10);           
      }                
			if(msgmp3Send.Idx == 2U)// darle al play 
      {         
             enviar_trama(CMDRESUME,DAT1REST,DAT2REST);   
      }  
      if(msgmp3Send.Idx == 3U)//Parar cancion 
      {
             enviar_trama(CMDPAUSE,DAT1REST,DAT2REST);  	
      }
			if(msgmp3Send.Idx == 20U)//Pone el volumen:VOL
      {
							enviar_trama(CMDSETVOL,DAT1REST,volumen);  
      }

			if(msgmp3Send.Idx == 5U)//Carpeta:1 Cancion: 0
			{
							enviar_trama(CMDPLAYSONG,DAT1REST,DAT2FS10);	
			}
			
			if(msgmp3Send.Idx == 6U)//Carpeta:1 Cancion: 1
			{
							enviar_trama(CMDPLAYSONG,DAT1REST,DAT2FS11);	
			}
			
			if(msgmp3Send.Idx == 7U)//Carpeta:2 Cancion: 0
			{
							enviar_trama(CMDPLAYSONG,DAT1REST,DAT2FS20);	
				
			}
			
			if(msgmp3Send.Idx == 8U)//Carpeta:2 Cancion: 1
			{
							enviar_trama(CMDPLAYSONG,DAT1REST,DAT2FS21);		
			}
			
			if(msgmp3Send.Idx == 9U)//Carpeta:2 Cancion: 2
			{
							enviar_trama(CMDPLAYSONG,DAT1REST,DAT2FS22);			
			}
			
			if(msgmp3Send.Idx == 10U)//Carpeta:3 Cancion: 0
			{
							enviar_trama(CMDPLAYSONG,DAT1REST,DAT2FS30);
			}
      if(msgmp3Send.Idx == 11U)//Sleep mode
      {
              enviar_trama(CMDSLEEPMOD,DAT1REST,DAT2REST);
      }
      if(msgmp3Send.Idx == 12U)//Wakeup
      {
              enviar_trama(CMDWAKEUP,DAT1REST,DAT2REST);
              
      }
    }
    }
}

/*---------------------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread && Message Queue creation & usage
 *---------------------------------------------------------------------------------------*/
 
int Init_MP3UART (void) {	
	//Send
	  tid_myUART_Thmp3Send = osThreadNew(myUART_ThMP3Send, NULL, NULL);
  if (tid_myUART_Thmp3Send == NULL) {
    return(-1);
  }
	//Receive
 	  tid_myUART_Thmp3Receive = osThreadNew(myUART_ThMP3Receive, NULL, NULL);
  if (tid_myUART_Thmp3Receive == NULL) {
    return(-1);
  }
  return(0);
}
 


/*---------------------------------------------------------------------------------------
 *      Configuración de los pines y funciones del mp3
 *---------------------------------------------------------------------------------------*/
 void init_mp3(void)
{

	__HAL_RCC_GPIOG_CLK_ENABLE();
	
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
	
		/* -2- Configure Rx(PG14) pin */
	
	GPIO_InitStruct.Pin = GPIO_PIN_14;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_SET);	
	
	/* -2- Configure Tx(PG9) pin */
	
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_SET);
	
 
}
void enviar_trama(unsigned char command,unsigned char dat1,unsigned char dat2)
{
		
    cmd[0] = 0x7E;
    cmd[1] = 0xFF;
    cmd[2] = 0x06;
    cmd[3] = command;//CMD
    cmd[4] = 0x00;//FBACK
    cmd[5] = dat1;//DAT1
    cmd[6] = dat2;//DAT2
    cmd[7] = 0xEF;
		USARTdrv->Send(cmd,8);
	  osThreadFlagsWait(ARM_USART_EVENT_TX_COMPLETE,osFlagsWaitAny,osWaitForever);
    if(command==0x09)
		{
				osDelay(200);
    }
		if(command==0x0C)
		{
					msgprin.Buf[21] = 0x04U;									 
					msgprin.Idx = 21U;			
					osMessageQueuePut(mid_Principal, &msgprin, 0U, 0U);
					osDelay(500);
		}
    if(command == CMDSETVOL)
          osDelay(200);
		if(command == CMDTFEXIST)
		{
          osDelay(500);
          osThreadFlagsSet(tid_myUART_Thmp3Receive, ARM_USART_EVENT_RECEIVE_COMPLETE);
		}
		msgprin.Buf[21] = 0x04U;									 
		msgprin.Idx = 21U;			
		osMessageQueuePut(mid_Principal, &msgprin, 0U, 0U);
}
			

void recibir_trama(void)
{	
		USARTdrv->Receive(trm,10); 
		osThreadFlagsWait(ARM_USART_EVENT_RECEIVE_COMPLETE,osFlagsWaitAny,osWaitForever);
}
