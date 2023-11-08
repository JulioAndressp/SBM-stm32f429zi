/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MP3_H
#define __MP3_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include <stdio.h>
#include <string.h>
#include "Driver_USART.h"
#include "cmsis_os2.h"                   /* ARM::CMSIS:RTOS:Keil RTX */
/* Defines  -------------------------------------------------------------------*/
  /*MP3SIGNAL*/
#define TRANFERSCOMPLETE 0x02
	/*CMD*/
#define CMDNEXTSONG 0x01
#define CMDPREVSONG 0x02 
#define CMDPLAYSONG 0x03
#define CMDVOLUP    0x04
#define CMDVOLDOWN  0x05
#define CMDSETVOL   0x06
#define CMDPLAYLOOP 0x08
#define CMDSELECTTF 0x09
#define CMDSLEEPMOD 0x0A
#define CMDWAKEUP   0x0B
#define CMDRESET    0x0C
#define CMDRESUME   0x0D
#define CMDPAUSE    0x0E
#define CMDFOLYFIL  0x0F
#define CMDSTOPPLAY 0x16
#define CMDCYCFOLNM 0x17
#define CMDSINGCYCP 0x19
#define CMDSETDAC   0x1A
#define CMDPLAYWVOL 0x22 
#define CMDTFEXIST  0x48//Comando que se envía para saber cuantas canciones hay
/*DAT1*/
#define DAT1REST    0x00
//#define DAT1FOL01   0x01
//#define DAT1FOL02   0x02
//#define DAT1FOL03   0x03
/*DAT2*/
#define DAT2REST    0x00
#define DAT2FS10    0x01
#define DAT2FS11    0x02
#define DAT2FS20    0x03
#define DAT2FS21    0x05
#define DAT2FS22    0x04
#define DAT2FS30    0x06
#define DAT2SELCTF  0X02
///* Señales ---------------------------------------------------*/
/* Prototipos de funciones  ---------------------------------------------------*/
void myUART_ThMP3Send(void *argument);
void myUART_ThMP3Receive(void *argument);
void myUSART_callback(uint32_t event);
void ThPruebaMP3 (void *argument);
int Init_MP3UART (void);
//void Reset_mp3 (void);
void recibir_trama (void);
//void select_TF (void);
//void Play_song(void);
//void Wake_up (void);
//void Play_folder(void);
void enviar_trama(unsigned char command,unsigned char dat1,unsigned char dat2);

/*Parametros de la cola*/

extern unsigned char cmd[8];
extern unsigned char trm[10]; 

#endif /* __MP3_H */
