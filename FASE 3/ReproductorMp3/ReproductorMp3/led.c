#include "Principal.h"
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
 osThreadId_t tid_rgb;                        // thread id
 static uint32_t exec1; 										//Timer OneShot
 osTimerId_t tim_rgb; 
void ThRgb (void *argument);                   // thread function

int Init_Rgb (void) {
	osStatus_t status;
  tid_rgb = osThreadNew(ThRgb, NULL, NULL);
  if (tid_rgb == NULL) {
    return(-1);
  }
   // Create one-shoot timer
  exec1 = 1U;
  tim_rgb = osTimerNew((osTimerFunc_t)&Timer1_Callback, osTimerOnce, &exec1, NULL);
  if (tim_rgb != NULL) {  // One-shot timer created
    // start timer with delay 50ms
    
    if (status != osOK) {
      return -1;
    }
	}
  return(0);
}
 
void ThRgb (void *argument) {
	
	rgb_init();
	
  while (1) {
    osMessageQueueGet(mid_JoystickRGB,&msgrgb,NULL,osWaitForever);
		if(msgrgb.Idx==1U)
		{
		osTimerStart(tim_rgb, 250);
    }
		if(msgrgb.Idx == 2U)
		{
			osTimerStart(tim_rgb, 1000); 
		}		
		if(msgrgb.Idx == 3U)
		{
			 HAL_GPIO_WritePin(RGBPUERTO_GPIOD, RGBBLUEPIN_11, GPIO_PIN_SET);
       HAL_GPIO_WritePin(RGBPUERTO_GPIOD, RGBGREENPIN_12, GPIO_PIN_SET);
       HAL_GPIO_WritePin(RGBPUERTO_GPIOD, RGBREDPIN_13, GPIO_PIN_SET);
		}
		if(msgrgb.Idx == 4U )
		{
			osTimerStart(tim_rgb,200);
		}
			
			osThreadYield();                            // suspend thread
  }
}

/*----------------------------------------------------------------------------
 *      				Timer 1: OneShot for frequency
 *---------------------------------------------------------------------------*/
  static void Timer1_Callback (void const *arg)
	{
		if(music==PLAY && modo==REPRODUCCION)
		{
			 HAL_GPIO_TogglePin(RGBPUERTO_GPIOD, RGBGREENPIN_12);
		   HAL_GPIO_WritePin(RGBPUERTO_GPIOD, RGBBLUEPIN_11, GPIO_PIN_SET);
       HAL_GPIO_WritePin(RGBPUERTO_GPIOD, RGBREDPIN_13, GPIO_PIN_SET);
			 osTimerStart(tim_rgb,250);
		}
		if(music == PAUSE && modo==REPRODUCCION)
		{
			 HAL_GPIO_TogglePin(RGBPUERTO_GPIOD, RGBBLUEPIN_11);
       HAL_GPIO_WritePin(RGBPUERTO_GPIOD, RGBGREENPIN_12, GPIO_PIN_SET);
       HAL_GPIO_WritePin(RGBPUERTO_GPIOD, RGBREDPIN_13, GPIO_PIN_SET);
			osTimerStart(tim_rgb,1000);
		}
		if( modo == REPOSO_TARJETA)
		{
			 HAL_GPIO_TogglePin(RGBPUERTO_GPIOD, RGBREDPIN_13);
		   HAL_GPIO_WritePin(RGBPUERTO_GPIOD, RGBBLUEPIN_11, GPIO_PIN_SET);
       HAL_GPIO_WritePin(RGBPUERTO_GPIOD, RGBGREENPIN_12, GPIO_PIN_SET);
			 osTimerStart(tim_rgb,200);			
		}
	}

/*----------------------------------------------------------------------------
 *      				Configuración RGB
 *---------------------------------------------------------------------------*/
void rgb_init(void)
{
       //Reloj
    __HAL_RCC_GPIOD_CLK_ENABLE();
       //Pines
            GPIO_InitStruct.Pin = RGBBLUEPIN_11|RGBGREENPIN_12|RGBREDPIN_13;
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
            GPIO_InitStruct.Pull = GPIO_PULLUP;
       //3.Habilitamos los puertos
            HAL_GPIO_Init(RGBPUERTO_GPIOD, &GPIO_InitStruct);
			//Inicialmente:
					/*RECUERDA QUE LOS PINES DEL RGB SON ACTIVOS A NIVEL BAJO.(1->Apagado. 0-> Encendido)*/
       HAL_GPIO_WritePin(RGBPUERTO_GPIOD, RGBBLUEPIN_11, GPIO_PIN_SET);
       HAL_GPIO_WritePin(RGBPUERTO_GPIOD, RGBGREENPIN_12, GPIO_PIN_SET);
       HAL_GPIO_WritePin(RGBPUERTO_GPIOD, RGBREDPIN_13, GPIO_PIN_SET);
}
