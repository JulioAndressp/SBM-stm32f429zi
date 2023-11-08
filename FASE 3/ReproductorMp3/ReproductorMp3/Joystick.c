#include "Principal.h"
/*---------------------Variables-----------------------------------------*/
 
 osThreadId_t tid_Joystick; 
 /*--Timer--*/
 static uint32_t exec1; 
 osTimerId_t tim_rebotes;  
 
  static uint32_t exec2; 
 osTimerId_t tim_pulso_largo;

   static uint32_t exec3; 
 osTimerId_t tim_pulso_corto;
 /*----------------------------------------------------------------------------
 *      Timer: Sample timer functions
 *---------------------------------------------------------------------------*/

// One-Shoot Timer Function
void Timer1_Callback (void const *arg) {
  if(HAL_GPIO_ReadPin(PUERTO_JOYSTICK_UP_RIGHT_GPIOB,JOYSTICK_UP)== GPIO_PIN_SET)
  {
				msgprin.Buf[1] = 0x00U;									 
				msgprin.Idx = 1U;			
				osMessageQueuePut(mid_Principal, &msgprin, 0U, 0U);

  }
    if(HAL_GPIO_ReadPin(PUERTO_JOYSTICK_UP_RIGHT_GPIOB,JOYSTICK_RIGHT)== GPIO_PIN_SET)
  {
  			msgprin.Buf[2] = 0x00U;									 
				msgprin.Idx = 2U;			
				osMessageQueuePut(mid_Principal, &msgprin, 0U, 0U);
  }
    if(HAL_GPIO_ReadPin(PUERTO_JOYSTICK_DOWN_CENTER_LEFT_GPIOE,JOYSTICK_DOWN)== GPIO_PIN_SET)
  {
				msgprin.Buf[3] = 0x00U;									 
				msgprin.Idx = 3U;			
				osMessageQueuePut(mid_Principal, &msgprin, 0U, 0U);
  }
      if(HAL_GPIO_ReadPin(PUERTO_JOYSTICK_DOWN_CENTER_LEFT_GPIOE,JOYSTICK_LEFT)== GPIO_PIN_SET)
  {
				msgprin.Buf[4] = 0x00U;									 
				msgprin.Idx = 4U;			
				osMessageQueuePut(mid_Principal, &msgprin, 0U, 0U);
  }
  if(HAL_GPIO_ReadPin(PUERTO_JOYSTICK_DOWN_CENTER_LEFT_GPIOE,JOYSTICK_CENTER)== GPIO_PIN_SET)
  {
		
		osTimerStart(tim_pulso_corto,350U);//Arranco el timer de la pulsacion corta
    osTimerStart(tim_pulso_largo,950U);//Arranco el timer de la pulsacion larga
		
  }
  }

 void Timer2_Callback (void const *arg) {
    

  if((HAL_GPIO_ReadPin(PUERTO_JOYSTICK_DOWN_CENTER_LEFT_GPIOE,JOYSTICK_CENTER)== GPIO_PIN_SET))//Larga.
  {
		
				msgprin.Buf[6] = 0x00U;									 
				msgprin.Idx = 6U;			
				osMessageQueuePut(mid_Principal, &msgprin, 0U, 0U);
  }
	
  }

  void Timer3_Callback (void const *arg) {
    
    if(HAL_GPIO_ReadPin(PUERTO_JOYSTICK_DOWN_CENTER_LEFT_GPIOE,JOYSTICK_CENTER)== GPIO_PIN_RESET )//corta
  {

				msgprin.Buf[5] = 0x00U;									 
				msgprin.Idx = 5U;			
				osMessageQueuePut(mid_Principal, &msgprin, 0U, 0U); 
  }

}
// Example: Create and Start timers
int Timer_Joystick (void) {
  osStatus_t status;                            // function return status
 
  // Create one-shoot timer
  exec1 = 1U;
  tim_rebotes = osTimerNew((osTimerFunc_t)&Timer1_Callback, osTimerOnce, &exec1, NULL);
  if (tim_rebotes != NULL) {  // One-shot timer created
    // start timer with delay 50ms
    status = osTimerStart(tim_rebotes, 50U); 
    if (status != osOK) {
      return -1;
    }
	}
  //Timer para el pulso largo
    exec2 = 1U;
  tim_pulso_largo = osTimerNew((osTimerFunc_t)&Timer2_Callback, osTimerOnce, &exec2, NULL);
  if (tim_pulso_largo != NULL) {  // One-shot timer created
    // start timer with delay 50ms
    if (status != osOK) {
      return -1;
    }
  }
    //Timer para representar mas rapido el pulsocorto
    exec3 = 1U;
  tim_pulso_corto = osTimerNew((osTimerFunc_t)&Timer3_Callback, osTimerOnce, &exec3, NULL);
  if (tim_pulso_corto != NULL) {  // One-shot timer created
    // start timer with delay 50ms
    if (status != osOK) {
      return -1;
    }
  }
  return NULL;
}

/*---------------------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread && Message Queue creation & usage
 *---------------------------------------------------------------------------------------*/
 
int Init_ThJoystick (void) {
 /*---Creación del hilo del joystick---*/
  tid_Joystick = osThreadNew(ThJoystick, NULL, NULL);
  if (tid_Joystick == NULL) {
    return(-1);
  } 
  return(0);
}
 
void ThJoystick (void *argument) {
	 Timer_Joystick();
  while (1) {
		osThreadFlagsWait(STARTREBOTES,osFlagsWaitAny,osWaitForever);
    osTimerStart(tim_rebotes, 50U); // Insert thread code here...
    osThreadYield();                               // suspend thread
  }
}

/*----------------------------------------------------------------------------
 *      Configuracion Pines del Joystick
 *---------------------------------------------------------------------------*/
void joystick_init(void)
{
    //Reloj
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
    //Pines
      //Arriba
        GPIO_InitStruct.Pin = JOYSTICK_UP;
        GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;

        HAL_GPIO_Init(PUERTO_JOYSTICK_UP_RIGHT_GPIOB, &GPIO_InitStruct);
        HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

      //Abajo
        GPIO_InitStruct.Pin = JOYSTICK_DOWN;
        GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;

        HAL_GPIO_Init(PUERTO_JOYSTICK_DOWN_CENTER_LEFT_GPIOE, &GPIO_InitStruct);
        HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

      //Centro
        GPIO_InitStruct.Pin = JOYSTICK_CENTER;
        GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;

        HAL_GPIO_Init(PUERTO_JOYSTICK_DOWN_CENTER_LEFT_GPIOE, &GPIO_InitStruct);
        HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
        
      //Izquierda
        GPIO_InitStruct.Pin = JOYSTICK_LEFT;
        GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;

        HAL_GPIO_Init(PUERTO_JOYSTICK_DOWN_CENTER_LEFT_GPIOE, &GPIO_InitStruct);
        HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
        
      //Derecha
        GPIO_InitStruct.Pin = JOYSTICK_RIGHT;
        GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;

        HAL_GPIO_Init(PUERTO_JOYSTICK_UP_RIGHT_GPIOB, &GPIO_InitStruct);
        HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

	void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(JOYSTICK_CENTER);
	HAL_GPIO_EXTI_IRQHandler(JOYSTICK_DOWN);
	HAL_GPIO_EXTI_IRQHandler(JOYSTICK_LEFT);
	HAL_GPIO_EXTI_IRQHandler(JOYSTICK_RIGHT);
	HAL_GPIO_EXTI_IRQHandler(JOYSTICK_UP);
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	osThreadFlagsSet(tid_Joystick,STARTREBOTES);
}

