/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PRINCIPAL_H
#define __PRINCIPAL_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "mp3.h"
#include "vol.h"
#include "com.h"
#include "joystick.h"
#include "lcd.h"
#include "clock.h"
#include "pwm.h"
#include "temp.h"
#include "led.h"
/* Exported types ----------------------------------------------------------- */
	typedef struct {                                // object data type
  uint8_t Buf[32];
  uint8_t Idx;
} MSGQUEUE_COM_t;
	
	typedef struct {                                // object data type
  uint8_t Buf[32];
  uint8_t Idx;
} MSGQUEUE_MP3_t;

  typedef struct {                                // object data type
  uint8_t Buf[32];
  uint8_t Idx;
} MSGQUEUE_LCD_t;
				
	typedef struct {                                // object data type
  uint8_t Buf[32];
  uint8_t Idx;
} MSGQUEUE_RGB_t;
	
	typedef struct {                                // object data type
  uint8_t Buf[32];
  uint8_t Idx;
} MSGQUEUE_PRIN_t;
/* Exported constants ------------------------------------------------------- */
#define MSGQUEUE_OBJECTS 16                     // number of Message Queue Objects
/* Exported macro ----------------------------------------------------------- */
/* Exported functions ------------------------------------------------------- */
 int Init_Reproductor (void);
 int Reproductor_timer (void);
/* Exported Signals --------------------------------------------------------- */
#define REPOSO 				  0
#define REPRODUCCION 		1
#define REPOSO_TARJETA 	2
#define EMPIEZAHILO 	  0X03
/* Exported Variables ------------------------------------------------------- */
 extern osThreadId_t tid_Principal;
 extern GPIO_InitTypeDef GPIO_InitStruct;

 extern int modo;
 extern int min_song;
 extern int seg_song;
 extern double volumen;
 extern osTimerId_t tim_song;
 extern osMessageQueueId_t mid_JoystickLCD; 
 extern osMessageQueueId_t mid_JoystickMP3Send;
 extern osMessageQueueId_t mid_JoystickRGB;
 extern osMessageQueueId_t mid_JoystickCOM;
 extern osMessageQueueId_t mid_Principal;
 
 extern MSGQUEUE_MP3_t msgmp3Send;
 extern MSGQUEUE_LCD_t msglcd;
 extern MSGQUEUE_RGB_t msgrgb;
 extern MSGQUEUE_COM_t msgcom;
 extern MSGQUEUE_PRIN_t msgprin;
 
 extern bool firstplay; 
 extern int actual_song;
 extern int actual_folder;
 extern int music;
 
#endif /* __PRINCIPAL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

