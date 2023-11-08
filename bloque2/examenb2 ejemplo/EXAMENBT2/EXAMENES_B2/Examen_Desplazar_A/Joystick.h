/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __JOYSTICK_H
#define __JOYSTICK_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
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
	//Señales
#define STARTREBOTES 0X01
#define EMPIEZALED 	 0x02
#define BUCLE	 0X03
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/*functions Prototype------------------------------------------------------- */
void joystick_init(void);
void ThPulsaciones (void *argument);
int Init_ThJoystick (void);
void ThJoystick (void *argument);                   // thread function
int Timer_Joystick (void);
 void ThPelota (void *argument);
//void ThColaJoystick (void *argument);
  /* Exported thread functions,  
  Example: extern void app_main (void *arg); */


//Puertos
#define PUERTO_PULSADOR_AZUL_GPIOC GPIOC
#define PUERTO_LEDS_GPIOB GPIOB

//Pulsador 
#define PULSADOR_AZUL_PIN_13 GPIO_PIN_13

//Leds
#define LD1_PIN_0 GPIO_PIN_0
#define LD2_PIN_7 GPIO_PIN_7
#define LD3_PIN_14 GPIO_PIN_14
#endif /* __JOYSTICK_H */
