#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "main.h"
#include "Joystick.h"
#include "lcd.h"
/*---------------------Variables-----------------------------------------*/
 GPIO_InitTypeDef GPIO_InitStruct;
	int posicion = -1; 


 osThreadId_t tid_Joystick; 
 osThreadId_t tid_Azul; 
 /*--Timer--*/
 static uint32_t exec1; 
 osTimerId_t tim_rebotes;  
 
  static uint32_t exec2; 
 osTimerId_t tim_barrido;
 osTimerId_t tim_barro;


 /*----------------------------------------------------------------------------
 *      Timer: Sample timer functions
 *---------------------------------------------------------------------------*/

// One-Shoot Timer Function
static void Timer1_Callback (void const *arg) 
{
  if(HAL_GPIO_ReadPin(PUERTO_JOYSTICK_UP_RIGHT_GPIOB,JOYSTICK_UP)== GPIO_PIN_SET)
  {

  }
    if(HAL_GPIO_ReadPin(PUERTO_JOYSTICK_UP_RIGHT_GPIOB,JOYSTICK_RIGHT)== GPIO_PIN_SET)
  {
		

  }
    if(HAL_GPIO_ReadPin(PUERTO_JOYSTICK_DOWN_CENTER_LEFT_GPIOE,JOYSTICK_DOWN)== GPIO_PIN_SET)
  {


  }
      if(HAL_GPIO_ReadPin(PUERTO_JOYSTICK_DOWN_CENTER_LEFT_GPIOE,JOYSTICK_LEFT)== GPIO_PIN_SET)
  {

	}

  if(HAL_GPIO_ReadPin(PUERTO_JOYSTICK_DOWN_CENTER_LEFT_GPIOE,JOYSTICK_CENTER)== GPIO_PIN_SET)
  {

  }
}
	
// static void Timer2_Callback (void const *arg) {


// }
 
  static void Timer3_Callback (void const *arg) {
	 
			 osTimerStart(tim_barro,300);
			 posicion++;
				if(posicion == 126)
					posicion = 0;
			 barra_vertical(posicion);
			 HAL_GPIO_WritePin(PUERTO_LEDS_GPIOB, LD1_PIN_0, GPIO_PIN_RESET);

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
	}		    //Timer para representar cuadrantes



  tim_barro = osTimerNew((osTimerFunc_t)&Timer3_Callback, osTimerOnce, &exec2, NULL);
  if (tim_barro != NULL) {  // One-shot timer created
    osTimerStart(tim_barro,300);
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
    tid_Azul = osThreadNew(ThAzul, NULL, NULL);
  if (tid_Azul == NULL) {
    return(-1);
  }

 
  return(0);
}
 void ThAzul (void *argument) {
  while (1) {	
  osThreadFlagsWait(PULSADOR,osFlagsWaitAny,osWaitForever);

       Borrado_LCD();
       posicion = 0;

  }
}
void ThJoystick (void *argument) {
	
	 Timer_Joystick ();
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
	__HAL_RCC_GPIOC_CLK_ENABLE();
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
				
			//Pulsador Azul
        GPIO_InitStruct.Pin = PULSADOR_AZUL_PIN_13;
        GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;

        HAL_GPIO_Init(PUERTO_PULSADOR_AZUL_GPIOC, &GPIO_InitStruct);
        HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/*---------------------------------------------------------------------------------------
 *      Interrupciones
 *---------------------------------------------------------------------------------------*/
	void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(JOYSTICK_CENTER);
	HAL_GPIO_EXTI_IRQHandler(JOYSTICK_DOWN);
	HAL_GPIO_EXTI_IRQHandler(JOYSTICK_LEFT);
	HAL_GPIO_EXTI_IRQHandler(JOYSTICK_RIGHT);
	HAL_GPIO_EXTI_IRQHandler(JOYSTICK_UP);
	HAL_GPIO_EXTI_IRQHandler(PULSADOR_AZUL_PIN_13);
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	osThreadFlagsSet(tid_Joystick,STARTREBOTES);
  osThreadFlagsSet(tid_Azul,PULSADOR);
}
