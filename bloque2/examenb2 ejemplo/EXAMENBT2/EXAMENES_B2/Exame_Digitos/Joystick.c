#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "main.h"
#include "Joystick.h"
#include "lcd.h"
/*---------------------Variables-----------------------------------------*/
 GPIO_InitTypeDef GPIO_InitStruct;
	int digitos = -1;
	bool primeravez = true;
	bool inverso = false;
  /*--Cola--*/
		typedef struct {                                // object data type
  uint8_t Buf[32];
  uint8_t Idx;
} MSGQUEUE_OBJ_t;
 MSGQUEUE_OBJ_t msg;
 osThreadId_t tid_ColaJoystick; 

 osThreadId_t tid_Joystick; 
 /*--Timer--*/
 static uint32_t exec1; 
 osTimerId_t tim_rebotes;  
 
  static uint32_t exec2; 
 osTimerId_t tim_digitos;

  static uint32_t exec3; 
 osTimerId_t tim_cadencia;

 /*----------------------------------------------------------------------------
 *      Timer: Sample timer functions
 *---------------------------------------------------------------------------*/

// One-Shoot Timer Function
static void Timer1_Callback (void const *arg) 
{
  if(HAL_GPIO_ReadPin(PUERTO_JOYSTICK_UP_RIGHT_GPIOB,JOYSTICK_UP)== GPIO_PIN_SET)
  {
		if(inverso==false)
			LCD_wr_cmd(0xA7);		
		else
			LCD_wr_cmd(0xA6);
		
		inverso =! inverso;
  }
    if(HAL_GPIO_ReadPin(PUERTO_JOYSTICK_UP_RIGHT_GPIOB,JOYSTICK_RIGHT)== GPIO_PIN_SET)
  {
		if(primeravez)
		{
		osTimerStart(tim_digitos,1500);
			primeravez=false;
		}
		
  }
    if(HAL_GPIO_ReadPin(PUERTO_JOYSTICK_DOWN_CENTER_LEFT_GPIOE,JOYSTICK_DOWN)== GPIO_PIN_SET)
  {

  }
      if(HAL_GPIO_ReadPin(PUERTO_JOYSTICK_DOWN_CENTER_LEFT_GPIOE,JOYSTICK_LEFT)== GPIO_PIN_SET)
  {
		if(!primeravez)
		{
				digitos++;
		if(digitos==0)
		{
				Borrado_LCD();
				LCD_EscribeFrase("0",1);
				LCD_update();
		}
		if(digitos==1)
		{
				Borrado_LCD();
				LCD_EscribeFrase("01",1);
				LCD_update();
		}	
		if(digitos==2)
		{
				Borrado_LCD();
				LCD_EscribeFrase("012",1);
				LCD_update();
		}
		if(digitos==3)
		{
				Borrado_LCD();
				LCD_EscribeFrase("0123",1);
				LCD_update();
		}	
		if(digitos==4)
		{
				Borrado_LCD();
				LCD_EscribeFrase("01234",1);
				LCD_update();
		}
		if(digitos==5)
		{
				Borrado_LCD();
				LCD_EscribeFrase("012345",1);
				LCD_update();
		}	
		if(digitos==6)
		{
				Borrado_LCD();
				LCD_EscribeFrase("0123456",1);
				LCD_update();
		}
		if(digitos==7)
		{
				Borrado_LCD();
				LCD_EscribeFrase("01234567",1);
				LCD_update();
		}
		if(digitos==8)
		{
				Borrado_LCD();
				LCD_EscribeFrase("012345678",1);
				LCD_update();
		}
		if(digitos==9)
		{
				Borrado_LCD();
				LCD_EscribeFrase("0123456789",1);
				LCD_update();
		}			
		osTimerStart(tim_digitos,1500);
	}
}
  if(HAL_GPIO_ReadPin(PUERTO_JOYSTICK_DOWN_CENTER_LEFT_GPIOE,JOYSTICK_CENTER)== GPIO_PIN_SET)
  {
  }
}
	
 static void Timer2_Callback (void const *arg) {
		digitos++;
		if(digitos==0)
		{
				Borrado_LCD();
				LCD_EscribeFrase("0",1);
				LCD_update();
		}
		if(digitos==1)
		{
				Borrado_LCD();
				LCD_EscribeFrase("01",1);
				LCD_update();
		}	
		if(digitos==2)
		{
				Borrado_LCD();
				LCD_EscribeFrase("012",1);
				LCD_update();
		}
		if(digitos==3)
		{
				Borrado_LCD();
				LCD_EscribeFrase("0123",1);
				LCD_update();
		}	
		if(digitos==4)
		{
				Borrado_LCD();
				LCD_EscribeFrase("01234",1);
				LCD_update();
		}
		if(digitos==5)
		{
				Borrado_LCD();
				LCD_EscribeFrase("012345",1);
				LCD_update();
		}	
		if(digitos==6)
		{
				Borrado_LCD();
				LCD_EscribeFrase("0123456",1);
				LCD_update();
		}
		if(digitos==7)
		{
				Borrado_LCD();
				LCD_EscribeFrase("01234567",1);
				LCD_update();
		}
		if(digitos==8)
		{
				Borrado_LCD();
				LCD_EscribeFrase("012345678",1);
				LCD_update();
		}
		if(digitos==9)
		{
				Borrado_LCD();
				LCD_EscribeFrase("0123456789",1);
				LCD_update();
		}							
	 
}
	static void Timer3_Callback (void const *arg) {
		

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
  exec2 = 2U;
  tim_digitos = osTimerNew((osTimerFunc_t)&Timer2_Callback, osTimerPeriodic, &exec2, NULL);
  if (tim_digitos != NULL) {  // One-shot timer created
    // start timer with delay 50ms
    if (status != osOK) {
      return -1;
    }
	}		 
	//Cadencia
	  exec3 = 3U;
  tim_cadencia = osTimerNew((osTimerFunc_t)&Timer3_Callback, osTimerPeriodic, &exec3, NULL);
  if (tim_cadencia != NULL) {  // One-shot timer created
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
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	osThreadFlagsSet(tid_Joystick,STARTREBOTES);
}