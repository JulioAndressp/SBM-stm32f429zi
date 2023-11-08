#include "Principal.h"
 
  GPIO_InitTypeDef GPIO_InitStruct;
	int modo = REPOSO;
	int min_song = 0;
	int seg_song = 0;
  double volumen = 0;
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
 osThreadId_t tid_Principal;                        // thread id
 osThreadId_t tid_Principal_2;
 osTimerId_t tim_song;                            // timer id

 MSGQUEUE_LCD_t msglcd;
 MSGQUEUE_MP3_t msgmp3Send;
 MSGQUEUE_RGB_t msgrgb;
 MSGQUEUE_COM_t msgcom;
 MSGQUEUE_PRIN_t msgprin;
 
 osMessageQueueId_t mid_JoystickLCD; 
 osMessageQueueId_t mid_JoystickMP3Send;
 osMessageQueueId_t mid_JoystickRGB;
 osMessageQueueId_t mid_JoystickCOM;
 osMessageQueueId_t mid_Principal;
 
 int music = PAUSE;
 bool firstplay = true;
 int actual_song = 0;
 int actual_folder = 1;
 
 static uint32_t exec1; 
 void Principal (void *argument);                   // thread function
 void Principal_2 (void *argument);

int Init_Reproductor (void) {
 
  tid_Principal = osThreadNew(Principal, NULL, NULL);
  if (tid_Principal == NULL) {
    return(-1);
  }
	tid_Principal_2 = osThreadNew(Principal_2, NULL, NULL);
  if (tid_Principal == NULL) {
    return(-1);
  }
		/*---Creación de la cola del ---*/
	   mid_JoystickLCD = osMessageQueueNew(MSGQUEUE_OBJECTS, sizeof(MSGQUEUE_LCD_t), NULL);
  if (mid_JoystickLCD == NULL) {
    ; // Message Queue object not created, handle failure
  }
		  mid_JoystickMP3Send = osMessageQueueNew(MSGQUEUE_OBJECTS, sizeof(MSGQUEUE_MP3_t), NULL);
  if (mid_JoystickMP3Send == NULL) {
    ; // Message Queue object not created, handle failure
  }
	
	
		  mid_JoystickRGB = osMessageQueueNew(MSGQUEUE_OBJECTS, sizeof(MSGQUEUE_RGB_t), NULL);
  if (mid_JoystickRGB == NULL) {
    ; // Message Queue object not created, handle failure
  }
			mid_JoystickCOM = osMessageQueueNew(MSGQUEUE_OBJECTS, sizeof(MSGQUEUE_COM_t), NULL);
  if (mid_JoystickCOM == NULL) {
    ; // Message Queue object not created, handle failure
  }
				mid_Principal = osMessageQueueNew(MSGQUEUE_OBJECTS, sizeof(MSGQUEUE_PRIN_t), NULL);
  if (mid_Principal == NULL) {
    ; // Message Queue object not created, handle failure
  }
 
  return(0);
}
 
int Reproductor_timer (void) {
  osStatus_t status;                            // function return status 
  // Create periodic timer
  exec1 = 2U;
  tim_song = osTimerNew((osTimerFunc_t)&Timer1_Callback, osTimerOnce, &exec1, NULL);
  if (tim_song != NULL) {  // Periodic timer created
    // start timer with periodic 1000ms interval           
    if (status != osOK) {
      return -1;
    }
  }
  return NULL;
}
void Principal (void *argument) {
	
	Init_ComUART();
	Init_ThLCD();
	Init_ThJoystick();
	Init_Vol();
	Init_clock();
	Init_Rgb();
	Init_Pwm();
	Init_Temp();
  Init_MP3UART();
	Reproductor_timer();
	
  while (1) {
		
		if( modo == REPOSO)
		{
			//Modo REPOSO
			
			//LCD
    msglcd.Buf[0] = 0x01U;									
    msglcd.Idx = 0U;    
		osMessageQueuePut(mid_JoystickLCD, &msglcd, 0U, 0U);
			
			//RGB
		msgrgb.Buf[3] = 0x01U;									
		msgrgb.Idx = 3U;
		osMessageQueuePut(mid_JoystickRGB, &msgrgb, 0U, 0U);
		
			//PWM
		iniciado = false;
		HAL_TIM_OC_Stop(&tim2_tarjeta,TIM_CHANNEL_4);
    music = PAUSE;
		
		}
		else if( modo == REPRODUCCION)
	  {
			//Modo REPRODUCCION
			//LCD
			msglcd.Buf[1] = 0x02U;									 
			msglcd.Idx = 1U;
			osMessageQueuePut(mid_JoystickLCD, &msglcd, 0U, 0U);
			
			if(music == PLAY)
			{
				//RGB
				msgrgb.Buf[1] = 0x02U;									
				msgrgb.Idx = 1U;		
			  osMessageQueuePut(mid_JoystickRGB, &msgrgb, 0U, 0U);
			}							
			if(music == PAUSE)
			{
				//RGB
				msgrgb.Buf[2] = 0x01U;									
				msgrgb.Idx = 2U;
				osMessageQueuePut(mid_JoystickRGB, &msgrgb, 0U, 0U);
			}			
	 	}
				if( modo == REPOSO_TARJETA)
		{
				//REPOSO_TARJETA
			  //LCD
				msglcd.Buf[0] = 0x01U;									
				msglcd.Idx = 0U;    
				osMessageQueuePut(mid_JoystickLCD, &msglcd, 0U, 0U);
				//RGB
				msgrgb.Buf[4] = 0x02U;									
				msgrgb.Idx = 4U;		
			  osMessageQueuePut(mid_JoystickRGB, &msgrgb, 0U, 0U);
				//PWM
				if(!iniciado)
				osThreadFlagsSet(tid_pwm_tarjeta,INSERTAR_TARJETA);
				//COM
				msgcom.Buf[1] = 0x01U;									 
				msgcom.Idx = 1U;			
				osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);
        
        music = PAUSE;
		}
		osThreadFlagsWait(ACTUALIZALCD,osFlagsWaitAny,osWaitForever);//Esperamos esta señal para que se actualice cada segundo el LCD y no altere la resolución del mismo.
		osThreadYield();                           
  }
}


void Principal_2 (void *argument) 
{
	  osStatus_t status;
    while (1)
    {
        
    status = osMessageQueueGet(mid_Principal, &msgprin, NULL, osWaitForever);
      
    if(status == osOK)
    {
      if( msgprin.Idx == 1U )//Pulsacion Up
      {     				
				if ( modo == REPRODUCCION )
					{			
						actual_folder--;
						
						if(actual_folder == 0)
						{
							//Carpeta 3 cancion 0
							msgmp3Send.Buf[10] = 0x01U;									 
							msgmp3Send.Idx = 10U;			
							osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);
					
							msgcom.Buf[1] = 0x01U;									 
							msgcom.Idx = 1U;			
							osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);
			
							actual_folder = 3;
							actual_song = 0;
							
						}
						if(actual_folder == 1)
						{
							//Carpeta 1 cancion 0
							msgmp3Send.Buf[5] = 0x02U;									 
							msgmp3Send.Idx = 5U;			
							osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);
										
							msgcom.Buf[1] = 0x01U;									 
							msgcom.Idx = 1U;			
							osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);
							
							actual_song = 0;
						}
						if(actual_folder == 2)
						{
							//Carpeta 2 cancion 0
							msgmp3Send.Buf[7] = 0x03U;									 
							msgmp3Send.Idx = 7U;			
							osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);	
				
							msgcom.Buf[1] = 0x01U;									 
							msgcom.Idx = 1U;			
							osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);							
							actual_song = 0;				
						}

						min_song = 0;
						seg_song = 0;
						osTimerStart(tim_song,1000);
						osThreadFlagsSet(tid_pwm,ZUMBIDO);
					}   
       }
			 if( msgprin.Idx == 2U )//Pulsacion right
			 {
							if( modo == REPRODUCCION )
					{
						if(actual_folder != 3)
						{
							actual_song++;	
							
							min_song = 0;
							seg_song = 0;
							osTimerStart(tim_song,1000);
							osThreadFlagsSet(tid_pwm,ZUMBIDO);
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
						
							msgcom.Buf[1] = 0x01U;									 
							msgcom.Idx = 1U;			
							osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);
											

						}
									 if(actual_folder == 1 && actual_song == 1)
						{				
							//Carpeta 1 cancion 1
							msgmp3Send.Buf[6] = 0x05U;									 
							msgmp3Send.Idx = 6U;			
							osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);
						
							msgcom.Buf[1] = 0x01U;									 
							msgcom.Idx = 1U;			
							osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);
											

						}
									 if(actual_folder == 2 && actual_song == 0)
						{				
							//Carpeta 2 cancion 0
							msgmp3Send.Buf[7] = 0x06U;									 
							msgmp3Send.Idx = 7U;			
							osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);
					
							msgcom.Buf[1] = 0x01U;									 
							msgcom.Idx = 1U;			
							osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);
											

						}
						 if(actual_folder == 2 && actual_song == 1)
						{
											//Carpeta 2 cancion 1
							msgmp3Send.Buf[8] = 0x07U;									 
							msgmp3Send.Idx = 8U;			
							osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);
							
							msgcom.Buf[1] = 0x01U;									 
							msgcom.Idx = 1U;			
							osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);
							


						}		
						 if(actual_folder == 2 && actual_song == 2)
						{
											//Carpeta 2 cancion 2
							msgmp3Send.Buf[9] = 0x08U;									 
							msgmp3Send.Idx = 9U;			
							osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);
							
							msgcom.Buf[1] = 0x01U;									 
							msgcom.Idx = 1U;			
							osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);
							


						}		
						
					}
			 }
			 if( msgprin.Idx == 3U )//Pulsacion Down
			 {
					if( modo == REPRODUCCION)
		{
			actual_folder++;
				
			if( actual_folder == 4 )
			{
					actual_folder = 1;
					
					//Carpeta 1 cancion 0
					msgmp3Send.Buf[5] = 0x10U;									 
					msgmp3Send.Idx = 5U;			
					osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);
				
					msgcom.Buf[1] = 0x01U;									 
					msgcom.Idx = 1U;			
					osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);
									
					min_song = 0;
					seg_song = 0;
					osTimerStart(tim_song,1000);
							
					actual_song = 0;
				}
				if(actual_folder == 2)
				{
					//Carpeta 2 cancion 0
					msgmp3Send.Buf[7] = 0x11U;									 
					msgmp3Send.Idx = 7U;			
					osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);
							    
					msgcom.Buf[1] = 0x01U;									 
					msgcom.Idx = 1U;			
					osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);
									
					min_song = 0;
					seg_song = 0;
					osTimerStart(tim_song,1000);	
					actual_song = 0;	
				}
				if(actual_folder == 3)
				{
					//Carpeta 3 cancion 0
					msgmp3Send.Buf[10] = 0x12U;									 
					msgmp3Send.Idx = 10U;			
					osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);
					
					msgcom.Buf[1] = 0x01U;									 
					msgcom.Idx = 1U;			
					osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);
									
					actual_song = 0;
				}
				
				osThreadFlagsSet(tid_pwm,ZUMBIDO);
			}
			 }
			 if( msgprin.Idx == 4U )//Pulsacion Left
			 {
					if ( modo == REPRODUCCION)
					{	
					if(actual_folder != 3)
						{
							actual_song--;
							
							min_song = 0;
							seg_song = 0;
							osTimerStart(tim_song,1000);	
							osThreadFlagsSet(tid_pwm,ZUMBIDO);							
						}
						if(actual_folder == 1 && actual_song == -1)
							actual_song = 1;
						if(actual_folder == 2 && actual_song == -1)
							actual_song = 2;
						
						//Reproducciones
						 if(actual_folder == 1 && actual_song == 0)
						{				
							//Carpeta 1 cancion 0
							msgmp3Send.Buf[5] = 0x13U;									 
							msgmp3Send.Idx = 5U;			
							osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);
							
							msgcom.Buf[1] = 0x01U;									 
							msgcom.Idx = 1U;			
							osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);
											
							min_song = 0;
							seg_song = 0;
							osTimerStart(tim_song,1000);
						}
									 if(actual_folder == 1 && actual_song == 1)
						{				
							//Carpeta 1 cancion 1
							msgmp3Send.Buf[6] = 0x14U;									 
							msgmp3Send.Idx = 6U;			
							osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);
							
							msgcom.Buf[1] = 0x01U;									 
							msgcom.Idx = 1U;			
							osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);
											

						}
									 if(actual_folder == 2 && actual_song == 0)
						{				
							//Carpeta 2 cancion 0
							msgmp3Send.Buf[7] = 0x15U;									 
							msgmp3Send.Idx = 7U;			
							osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);
							
							msgcom.Buf[1] = 0x01U;									 
							msgcom.Idx = 1U;			
							osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);
											

						}
						 if(actual_folder == 2 && actual_song == 1)
						{
							//Carpeta 2 cancion 1
							msgmp3Send.Buf[8] = 0x16U;									 
							msgmp3Send.Idx = 8U;			
							osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);
						
							msgcom.Buf[1] = 0x01U;									 
							msgcom.Idx = 1U;			
							osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);
											


						}		
						 if(actual_folder == 2 && actual_song == 2)
						{
							//Carpeta 2 cancion 2
							msgmp3Send.Buf[9] = 0x17U;									 
							msgmp3Send.Idx = 9U;			
							osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);
							
							msgcom.Buf[1] = 0x01U;									 
							msgcom.Idx = 1U;			
							osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);
											


						}		
					
					}
			 }
			 if( msgprin.Idx == 5U )//Pulsacion Center Corta
			 {
					if( modo == REPRODUCCION )
					{		
							music =! music;	
							if(music == PLAY )
							{
									msgmp3Send.Buf[2] = 0x21U;									    
									msgmp3Send.Idx = 2U;  		
									osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);
					  
									msgcom.Buf[1] = 0x01U;									 
									msgcom.Idx = 1U;			
									osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);
									osTimerStart(tim_song,1000);
							}
							if( music == PAUSE )
							{
									msgmp3Send.Buf[3] = 0x22U;									    
									msgmp3Send.Idx = 3U;  		
									osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);

									msgcom.Buf[1] = 0x01U;									 
									msgcom.Idx = 1U;			
									osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);
									osTimerStop(tim_song);	
							}
							osThreadFlagsSet(tid_pwm,ZUMBIDO);
					}
			 }
			 if(msgprin.Idx == 6U )//Pulsacion Center Larga
			 {
				 				 if(modo != REPOSO_TARJETA)
				 {
						modo =! modo;
		if(modo == REPRODUCCION)
		{			
					//Wake up
          msgmp3Send.Buf[12] = 0x20U;									    
          msgmp3Send.Idx = 12U;  		
          osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);
					osDelay(500);
			
					msgcom.Buf[1] = 0x01U;									 
					msgcom.Idx = 1U;			
					osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);
          osDelay(500);
					//Reproduzco la primera cancion de esa carpeta
					msgmp3Send.Buf[1] = 0x19U;									    
					msgmp3Send.Idx = 1U;  		
					osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U);

					msgcom.Buf[1] = 0x01U;									 
					msgcom.Idx = 1U;			
					osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);			
			
					min_song = 0;
					seg_song = 0;
					osTimerStart(tim_song,1000);
					
					music = PLAY ;
				}
				if(modo == REPOSO)
				{
					//Sleep
          msgmp3Send.Buf[11] = 0x20U;									    
          msgmp3Send.Idx = 11U;  		
          osMessageQueuePut(mid_JoystickMP3Send, &msgmp3Send, 0U, 0U); 
					osDelay(500);
          //Teraterm
					msgcom.Buf[1] = 0x01U;									 
					msgcom.Idx = 1U;			
					osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);
					//Paro el reproductor
					osTimerStop(tim_song); 
					min_song = 0;
					seg_song = 0;
					//Pongo la cancion actual y la carpeta actual a 0.
					actual_song = 0;
					actual_folder = 1;
					
				}
				
				osThreadFlagsSet(tid_pwm,ZUMBIDO);	
			}
			 }
			 if( msgprin.Idx == 20U )//Volumen
			 {
         if(modo == REPRODUCCION)
         {
            volumen = msgvol.Buf[20];// do some work...
				    osMessageQueuePut(mid_JoystickMP3Send, &msgvol, 0U, 0U);
						osMessageQueuePut(mid_JoystickLCD, &msgvol, 0U, 0U);
				 		
				    msgcom.Buf[1] = 0x01U;									 
					  msgcom.Idx = 1U;			
					  osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);
			 }
     }
			 if( msgprin.Idx == 21U )//COM:Caso del mp3
			 {
				 		msgcom.Buf[1] = 0x02U;									 
						msgcom.Idx = 1U;			
						osMessageQueuePut(mid_JoystickCOM, &msgcom, 0U, 0U);
			 }
		 }
	 }
 }

static void Timer1_Callback (void const *arg) {
    
		if((min_song==59)&&(seg_song==59))
		{
			min_song=0;
			seg_song=0;
		}
		if(seg_song==59)
		{
			min_song++;
		  seg_song=0;
		}
		else
		{
			seg_song++;
		}
		osTimerStart(tim_song, 1000U); 
		
}