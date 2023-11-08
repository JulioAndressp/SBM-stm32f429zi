#include "stm32f4xx_hal.h"

//Variables Globales
extern const unsigned char initLCD[];
extern unsigned char g_buffer[];
extern uint8_t position;

//funciones
void LCD_reset(void);
void LCD_update(void);
void  LCD_Init(void);
void symbolToLocalBuffer(uint8_t line, uint8_t symbol);
