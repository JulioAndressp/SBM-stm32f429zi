#ifndef __LCD_H
#define __LCD_H

#include "stdio.h"
#include "string.h"
#include "RTE_Device.h"
#include "Driver_SPI.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h" 
//Funciones que nos permiten configurar el lcd
/* Orden en el main--------------------------------------------------------------------------*/
 void init_SPI(void);
 void reset_lcd(void);
 void LCD_init(void);
 void Borrado_LCD(void);
/* Orden en el main--------------------------------------------------------------------------*/
 void LCD_wr_data(unsigned char data);
 void LCD_wr_cmd(unsigned char cmd);
 void LCD_update(void);
 void LCD_symbolToLocalBuffer_L1(uint8_t symbol);
 void LCD_symbolToLocalBuffer_L2(uint8_t symbol);
 void LCD_symbolToLocalBuffer(uint8_t linea, uint8_t symbol);//Escribe en la linea 1 o en la linea 2
 void LCD_EscribeFrase(char *frase, uint8_t line);
 void delay(uint32_t n_microsegundos);
 
 void ThLCD (void *argument);         // thread function 1
 int Init_ThLCD (void) ;
 //Variables
 extern osThreadId_t tid_ThLCD;
//Señales
#define STARTREBOTES 0X01
/* Exported macro ------------------------------------------------------------*/
/* Exported variables ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void gpio_init(void);
#endif
