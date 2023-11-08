#ifndef __LCD_H
#define __LCD_H

#include "stdio.h"
#include "string.h"
#include "RTE_Device.h"
#include "Driver_SPI.h"
#include "stm32f4xx_hal.h"
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
 void pintar_cuadrante(int pulsacion);
 void ThLCD (void *argument);         // thread function 1
 int Init_ThLCD (void) ;
 void pintar_diagonal(int posicion);
 //Variables
 
 
/*---------------------------------------------------------------------------
 *     														PRUEBA
 *---------------------------------------------------------------------------*/
/* Exported constants -------------------------------------------------------*/
//Puertos
#define PUERTO_PULSADOR_AZUL_GPIOC GPIOC
#define PUERTO_LEDS_GPIOB GPIOB
#define PUERTO_JOYSTICK_DOWN_CENTER_LEFT_GPIOE GPIOE
#define PUERTO_JOYSTICK_UP_RIGHT_GPIOB GPIOB

//Pulsador 
#define PULSADOR_AZUL_PIN_13 GPIO_PIN_13

//Leds
#define LD1_PIN_0 GPIO_PIN_0
#define LD2_PIN_7 GPIO_PIN_7
#define LD3_PIN_14 GPIO_PIN_14

//Joystick
#define JOYSTICK_UP GPIO_PIN_10
#define JOYSTICK_DOWN GPIO_PIN_12
#define JOYSTICK_CENTER GPIO_PIN_15
#define JOYSTICK_LEFT GPIO_PIN_14
#define JOYSTICK_RIGHT GPIO_PIN_11

//Señales
#define STARTREBOTES 0X01
/* Exported macro ------------------------------------------------------------*/
/* Exported variables ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void gpio_init(void);
#endif
