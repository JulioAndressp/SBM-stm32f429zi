/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PWM_H
#define __PWM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h" 
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define PUERTO_PWM_GPIOA    GPIOA
#define PWM_PIN_3						GPIO_PIN_3
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
	 int Init_Pwm(void);
	 void ThPwm (void *argument);                   // thread function
	 void ThPwmTarjeta (void *argument);
	 void Init_Zumb(void);
	 void Init_Zumb_TF(void);
/* Exported signals ------------------------------------------------------- */
#define ZUMBIDO 				 0x01
#define INSERTAR_TARJETA 0x03
/* Exported Variables ------------------------------------------------------- */
	 extern osThreadId_t tid_pwm; 	 // thread id
	 extern osThreadId_t tid_pwm_tarjeta;
	 extern TIM_HandleTypeDef tim2;//Timer
   extern TIM_OC_InitTypeDef octim4;//Canal
	 extern TIM_HandleTypeDef tim2_tarjeta;//Timer_TF
   extern TIM_OC_InitTypeDef octim4_tarjeta;//Canal_TF
	 extern bool iniciado;
#endif /* __PWM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
