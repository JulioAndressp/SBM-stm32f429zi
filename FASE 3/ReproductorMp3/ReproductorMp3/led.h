/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H
#define __LED_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h" 
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define RGBPUERTO_GPIOD     GPIOD
#define RGBREDPIN_13 				GPIO_PIN_13
#define RGBGREENPIN_12 			GPIO_PIN_12
#define RGBBLUEPIN_11  			GPIO_PIN_11
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
	int Init_Rgb (void);
	void rgb_init(void);
  static void Timer1_Callback (void const *arg);
/* Exported variables ------------------------------------------------------- */
extern  osThreadId_t tid_rgb; 
 
#endif /* __LED_H */
