#include "cmsis_os2.h"  
#include "stm32f4xx_hal_gpio.h"
#include "main.h"

 void Pin_config(void);
void Pin_config(void){			
		static GPIO_InitTypeDef GPIO_InitStruct;
		
		// Leds
		__HAL_RCC_GPIOB_CLK_ENABLE();
		
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_7 | GPIO_PIN_14;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}