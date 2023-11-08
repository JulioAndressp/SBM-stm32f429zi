#include "Thjoy.h"
#include "stm32f4xx_hal.h"
 
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 

osTimerId_t timer_debounce;
uint16_t    gestos_contados=0;
osThreadId_t tid_Thjoy;

void Threadjoy (void *argument);                   // thread function
static void init_Joystick(void);
static void TimerDebounce_Callback(void);
 
int Init_Thjoy (void) {
  timer_debounce = osTimerNew((osTimerFunc_t)&TimerDebounce_Callback,osTimerOnce,0x00,NULL);
  init_Joystick();
  tid_Thjoy = osThreadNew(Threadjoy, NULL, NULL);
  if (tid_Thjoy == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Threadjoy (void *argument) {
 
  while (1) {
    ; // Insert thread code here...
    osThreadFlagsWait(JOYINTERRUPT,osFlagsWaitAny,osWaitForever);
    osTimerStart(timer_debounce,50);
    osThreadYield();                            // suspend thread
  }
}
static void TimerDebounce_Callback(void)
{
  if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10))
  {
      gestos_contados = gestos_contados + 1;
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
  }
  
  if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11))
  {
      gestos_contados = gestos_contados + 2;
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
    
  }
  
  if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12))
  {
      gestos_contados = gestos_contados + 3;
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
  }
  
  if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14))
  {
      gestos_contados = gestos_contados + 4;
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
  }
  
  if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15))
  {
      gestos_contados = gestos_contados + 5;
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
  }
  
}

static void init_Joystick(void)
{
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  
  GPIO_InitTypeDef GPIO_InitStruct;
  
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 ; // SW_UP SW_RIGHT
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  
  HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);  
  
  
  
  
  __HAL_RCC_GPIOE_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15; //SW_DOWN SW_LEFT SW_CENTER
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
   GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  
  HAL_GPIO_Init(GPIOE,&GPIO_InitStruct);


}
