#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "stm32f4xx_hal.h"
#include "Thled1.h"

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/


osThreadId_t tid_Thled1;                        // thread id


void Thled1 (void *argument);                   // thread function


void InitLED1(void);


int Init_Thled1 (void) {
  InitLED1();
  tid_Thled1 = osThreadNew(Thled1, NULL, NULL);
  if (tid_Thled1 == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thled1 (void *argument) {

  while (1) {
     // Insert thread code here...
   
    
    HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
    osDelay(200);
    HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
    osDelay(800);

    //initTimer(800);
   // osThreadYield();                            // suspend thread
  }
}

void InitLED1(void){
    GPIO_InitTypeDef gpio_initstruct;
    __HAL_RCC_GPIOB_CLK_ENABLE();
  
  gpio_initstruct.Pin=GPIO_PIN_0;
  gpio_initstruct.Mode=GPIO_MODE_OUTPUT_PP;
  gpio_initstruct.Pull=GPIO_NOPULL;
  gpio_initstruct.Speed=GPIO_SPEED_FREQ_MEDIUM;
  
  HAL_GPIO_Init(GPIOB,&gpio_initstruct);

}



