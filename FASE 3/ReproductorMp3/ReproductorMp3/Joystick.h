/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __JOYSTICK_H
#define __JOYSTICK_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"  
#define MSGQUEUE_OBJECTS 16
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
	//Puertos
#define PUERTO_JOYSTICK_DOWN_CENTER_LEFT_GPIOE GPIOE
#define PUERTO_JOYSTICK_UP_RIGHT_GPIOB GPIOB
	//Joystick
#define JOYSTICK_UP GPIO_PIN_10
#define JOYSTICK_DOWN GPIO_PIN_12
#define JOYSTICK_CENTER GPIO_PIN_15
#define JOYSTICK_LEFT GPIO_PIN_14
#define JOYSTICK_RIGHT GPIO_PIN_11
	//Se�ales
#define STARTREBOTES 0X01
#define PULSOLARGO 	 0x02
#define PULSOCORTO	 0X03

#define PAUSE 			 0
#define PLAY 				 1
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/*functions Prototype------------------------------------------------------- */
void joystick_init(void);
void ThPulsaciones (void *argument);
int Init_ThJoystick (void);
void ThJoystick (void *argument);  
int Timer_Joystick (void);
/*functions Prototype------------------------------------------------------- */ 
 extern osThreadId_t tid_Joystick; 
#endif /* __JOYSTICK_H */