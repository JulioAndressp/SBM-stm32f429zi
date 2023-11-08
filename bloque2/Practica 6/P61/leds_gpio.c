#include "leds_gpio.h"
#include "stm32f4xx_hal.h"


void InitLeds(void){
    GPIO_InitTypeDef gpio_initstruct;
    __HAL_RCC_GPIOB_CLK_ENABLE();
  
  gpio_initstruct.Pin= GPIO_PIN_0 | GPIO_PIN_7| GPIO_PIN_14; //GREEN BLUE RED
  gpio_initstruct.Mode=GPIO_MODE_OUTPUT_PP;
  gpio_initstruct.Pull=GPIO_NOPULL;
  gpio_initstruct.Speed=GPIO_SPEED_FREQ_MEDIUM;
  
  HAL_GPIO_Init(GPIOB,&gpio_initstruct);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_7| GPIO_PIN_14, GPIO_PIN_RESET);
  

}


