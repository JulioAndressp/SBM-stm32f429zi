#include "lcd.h"
#include "Arial12x12.h"
#include "cmsis_os2.h"    
/*------------Variables globales-------------------------*/
 extern GPIO_InitTypeDef GPIO_InitStruct;
 extern ARM_DRIVER_SPI Driver_SPI1;
 ARM_DRIVER_SPI* SPIdrv = &Driver_SPI1;
 TIM_HandleTypeDef timer7;
 unsigned char buffer[512];
 char frase[60];
 uint32_t positionL1 = 0;
 uint32_t positionL2 = 256;

 
  extern	int posicion ; 
//	extern osTimerId_t tim_barro;
	/*----------------------------------------------------------------------------
 *      Message Queue creation & usage
 *---------------------------------------------------------------------------*/
 /*------------Define-------------------------*/
	#define MSGQUEUE_OBJECTS 16                     // number of Message Queue Objects
/*------------Types & Variables-------------------------*/

	typedef struct {                                // object data type
		uint8_t Buf[32];
		uint8_t Idx;
	} MSGQUEUE_OBJ_t;
 

  extern osTimerId_t tim_barrido;
/*------------Funciones-------------------------*/
 
/*----------------------------------------------------------------------------
 *     Funciones de la cola
 *---------------------------------------------------------------------------*/

void ThLCD (void *argument) {

	}
/*----------------------------------------------------------------------------
 *     Funciones configuración LCD
 *---------------------------------------------------------------------------*/

void LCD_symbolToLocalBuffer(uint8_t linea, uint8_t symbol)
{
  if (linea == 0)
  {
    LCD_symbolToLocalBuffer_L1(symbol);
  } else if (linea == 1)
    LCD_symbolToLocalBuffer_L2(symbol);
    
}
void LCD_symbolToLocalBuffer_L1(uint8_t symbol)
{
  uint8_t i, valor1, valor2 = 0;
  uint16_t comienzo = 0;
	
  //Linea1
  comienzo = 25 * (symbol - ' '); //Avanza a la linea donde se encuentra la letra que quiero representar
	// todas las letras de arial 12x12 ocupan 25 bytes (1 columna = 1 byte)
  
  for (i = 0; i<12; i++){ //Vamos a escribir las 12 primeras columnas de la letra
   
     valor1 = Arial12x12 [comienzo+i*2+1]; // valor1= arial[1,3,5,7,9,....]
     valor2 = Arial12x12 [comienzo+i*2+2]; // valor2= arial[2,4,6,8,10,....]

     buffer [i+positionL1] = valor1; //pagina 0 escribimos los valores impares de arial
     buffer [i+128 +positionL1] = valor2; //pagina 1 escribimos los valores pares de arial
   
  }
  
  positionL1 = positionL1 + Arial12x12 [comienzo]; // posicionL1 indica la columna donde se debe comenza a escribir  
  }

  void LCD_symbolToLocalBuffer_L2(uint8_t symbol)
  {
      uint8_t i, valor1, valor2 = 0;
      uint16_t comienzo = 0;
      //Linea 2
    comienzo = 25 * (symbol - ' '); //Avanza a la linea donde se encuentra la letra que quiero representar
	// todas las letras de arial 12x12 ocupan 25 bytes (1 columna = 1 byte)
  
  for (i = 0; i<12; i++){ //Vamos a escribir las 12 primeras columnas de la letra
   
     valor1 = Arial12x12 [comienzo+i*2+1]; // valor1= arial[1,3,5,7,9,....]
     valor2 = Arial12x12 [comienzo+i*2+2]; // valor2= arial[2,4,6,8,10,....]

     buffer [i+positionL2] = valor1; //pagina 0 escribimos los valores impares de arial
     buffer [i+128 +positionL2] = valor2; //pagina 1 escribimos los valores pares de arial
   
  }
  
  positionL2 = positionL2 + Arial12x12 [comienzo]; // posicionL1 indica la columna donde se debe comenza a escribir
  
	
	}
  
void LCD_update(void)
{
 int i;
 LCD_wr_cmd(0x00); // 4 bits de la parte baja de la dirección a 0
 LCD_wr_cmd(0x10); // 4 bits de la parte alta de la dirección a 0
 LCD_wr_cmd(0xB0); // Página 0
 
 for(i=0;i<128;i++){
 LCD_wr_data(buffer[i]);
 }
 
 LCD_wr_cmd(0x00); // 4 bits de la parte baja de la dirección a 0
 LCD_wr_cmd(0x10); // 4 bits de la parte alta de la dirección a 0
 LCD_wr_cmd(0xB1); // Página 1
 
 for(i=128;i<256;i++){
 LCD_wr_data(buffer[i]);
 }
 
 LCD_wr_cmd(0x00); 
 LCD_wr_cmd(0x10); 
 LCD_wr_cmd(0xB2); //Página 2
 for(i=256;i<384;i++){
 LCD_wr_data(buffer[i]);
 }
 
 LCD_wr_cmd(0x00); 
 LCD_wr_cmd(0x10); 
 LCD_wr_cmd(0xB3); // Pagina 3
 
 
 for(i=384;i<512;i++){
 LCD_wr_data(buffer[i]);
 }
 
 }


void reset_lcd(void)
{

    //Apagamos el lcd
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);//RESET
		delay(2); // Esperamos dos microsegundos
	//Encendemos el lcd
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);//RESET
		  delay(1000);
}
 void LCD_init(void){
	
	//Display off
	LCD_wr_cmd(0xAE);
	//Polarizacion lcd a 1/9
	LCD_wr_cmd(0xA2);
	//Direccionamiento ram
	LCD_wr_cmd(0xA0);
	//Salidas com 
	LCD_wr_cmd(0xC8);
	//Resistencias internas
	LCD_wr_cmd(0x22);
	//Power on
	LCD_wr_cmd(0x2F);
	//Display en la linea 0
	LCD_wr_cmd(0x40);
	//Dislpay ON
	LCD_wr_cmd(0xAF);
	//Contraste
	LCD_wr_cmd(0x81);
	//Valor contraste
	LCD_wr_cmd(0x13);
	//Display all points normal
	LCD_wr_cmd(0xA4);
	//LCD display normal
	LCD_wr_cmd(0xA6);
	
}


void LCD_wr_cmd(unsigned char cmd)//Escribe un comando en el lcd
{
		ARM_SPI_STATUS stat;
 // Seleccionar CS = 0;
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);//CS
 // Seleccionar A0 = 0;
 HAL_GPIO_WritePin(GPIOF,GPIO_PIN_13,GPIO_PIN_RESET);//A0
 // Escribir un comando (cmd) usando la función SPIDrv->Send(…);
 SPIdrv->Send(&cmd,sizeof(cmd));//Tamaño en bytes
 // Esperar a que se libere el bus SPI;
 do{
	 stat = SPIdrv->GetStatus();
 }while(stat.busy);
 //delay(1000);
 // Seleccionar CS = 1;
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);//CS
}


void LCD_wr_data(unsigned char data)//Escribe un dato en lcd.
{
	ARM_SPI_STATUS stat;
 // Seleccionar CS = 0;
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);//CS
 // Seleccionar A0 = 1;
  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_13,GPIO_PIN_SET);//A0
 // Escribir un dato (data) usando la función SPIDrv->Send(…);
	SPIdrv->Send(&data,sizeof(data));//Tamaño en bytes
 // Esperar a que se libere el bus SPI;
	 do{
	 stat = SPIdrv->GetStatus();
 }while(stat.busy);
	 //delay(1000);
 // Seleccionar CS = 1;
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);//CS
 
}


void init_SPI(void)
  {
    //Configuramos el SPI
  SPIdrv->Initialize(NULL);
	SPIdrv->PowerControl(ARM_POWER_FULL);
	SPIdrv->Control(ARM_SPI_MODE_MASTER|ARM_SPI_CPOL1_CPHA1|ARM_SPI_MSB_LSB|ARM_SPI_SS_MASTER_UNUSED|ARM_SPI_DATA_BITS(8),1000000);
    //Configuramos los pines RESET,A0,CS como salida
      //Instanciamos los pines como salida.
            __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_6;//RESET
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
            __HAL_RCC_GPIOF_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_13;//A0
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
            __HAL_RCC_GPIOD_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_14;//CS
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
		
		//Inicialmente los pines de RESTE, A0, CS :
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);//CS
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET);//RESET
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);//A0

}

//Timer
 void delay(uint32_t n_microsegundos)//Funcion que usaremos para el delay
{
// Configurar y arrancar el timer para generar un evento pasados n_microsegundos
	timer7.Instance = TIM7;
	timer7.Init.Prescaler = 20;//Prescaler=84M/21=4Mhz
	timer7.Init.Period = (n_microsegundos/4)-1;
	__HAL_RCC_TIM7_CLK_ENABLE();
	HAL_TIM_Base_Init(&timer7);//estas dos lineas sirven para arrancar el timer
	HAL_TIM_Base_Start(&timer7);
 // Esperar a que se active el flag del registro de Match
	while(TIM7 -> CNT);
 	HAL_TIM_Base_DeInit(&timer7);
	HAL_TIM_Base_Stop(&timer7);

}

//Pintar barra vertical
void barra_vertical(int posicion)
{
	Borrado_LCD();

	buffer[posicion] = 0xFF;
	buffer[posicion+128] = 0xFF;
	buffer[posicion+256] = 0xFF;
	buffer[posicion+384] = 0xFF;
	buffer[posicion+1] = 0xFF;
	buffer[posicion+1+128] = 0xFF;
	buffer[posicion+1+256] = 0xFF;
	buffer[posicion+1+384] = 0xFF;
	LCD_update();
}
//Escribe Frase
void LCD_EscribeFrase(char *frase, uint8_t linea)
{
  uint8_t i;


  if(linea == 1){
    for(i=0; i<strlen(frase); i++){
      if(positionL1+12 < 127)
        LCD_symbolToLocalBuffer_L1(frase[i]);
    }
    
  }
  if(linea == 2){
    for(i=0; i<strlen(frase); i++){
      if(positionL2+12 < 383)
        LCD_symbolToLocalBuffer_L2(frase[i]);
    }
    
}
  positionL1 = 0;
  positionL2 = 256;
	LCD_update();
}

//Limpiar LCD
void Borrado_LCD(void)
{
    int i;
    for( i=0;i<128;i++)
    {
      buffer[i]=0x00;
    }
    for( i=128;i<256;i++)
    {
      buffer[i]=0x00;
    }
    for( i=256;i<384;i++)
    {
      buffer[i]=0x00;
    }
    for( i=384;i<512;i++)
    {
      buffer[i]=0x00;
    }
    LCD_update();
  }
void gpio_init(void)
{
       //Reloj
    __HAL_RCC_GPIOB_CLK_ENABLE();
       //Pines
            GPIO_InitStruct.Pin = LD1_PIN_0|LD2_PIN_7|LD3_PIN_14;
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
            GPIO_InitStruct.Pull = GPIO_PULLUP;
       //3.Habilitamos los puertos
            HAL_GPIO_Init(PUERTO_LEDS_GPIOB, &GPIO_InitStruct);
       //Inicialmente
       HAL_GPIO_WritePin(PUERTO_LEDS_GPIOB, LD1_PIN_0, GPIO_PIN_RESET);
       HAL_GPIO_WritePin(PUERTO_LEDS_GPIOB, LD2_PIN_7, GPIO_PIN_RESET);
       HAL_GPIO_WritePin(PUERTO_LEDS_GPIOB, LD3_PIN_14, GPIO_PIN_RESET);
}

