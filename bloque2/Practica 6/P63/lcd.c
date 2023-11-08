
#include "lcd.h"
#include "Driver_SPI.h"
#include "Arial12x12.h"
#include "stm32f4xx_hal.h"
#include "RTE_Components.h"


extern ARM_DRIVER_SPI Driver_SPI1;


void delay (uint32_t n_microsegundos);
void LCD_reset(void);
void LCD_update(void);
void  LCD_Init(void);
void LCD_clearbuffer(void);
void symbolToLocalBuffer(uint8_t line, uint8_t symbol);


const unsigned char initLCD[12] ={0xAE,0xA2,0xA0,0xC8,0x22,0x2F,0x40,0xAF,0x81,0x11,0xA4,0xA6};
unsigned char g_buffer[512];

static ARM_DRIVER_SPI* SPIdrv= &Driver_SPI1;
static TIM_HandleTypeDef htim7;
uint8_t position=0;
uint8_t positionL1=0;
uint8_t positionL2=0;




static void initLCDPins(void);
static void LCD_wr_data(unsigned char data);
static void LCD_wr_cmd(unsigned char cmd);





//  Inicializacion de todos los pines GPIO
//  ---------------------------------------------------------
//  ---------------------------------------------------------
//  ---------------------------------------------------------
//  ---------------------------------------------------------
  static void initLCDPins(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPIO_PIN_6;                     // Config A6 RESET
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);  //pin reset set a 1
  
  HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);
  
__HAL_RCC_GPIOD_CLK_ENABLE();                   
  
  GPIO_InitStruct.Pin = GPIO_PIN_14;       //Config D14 CS D15 AO
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  
  HAL_GPIO_Init(GPIOD,&GPIO_InitStruct);
  
  __HAL_RCC_GPIOF_CLK_ENABLE();                   
  
  GPIO_InitStruct.Pin = GPIO_PIN_13;       //Config D14 CS D15 AO
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  
  HAL_GPIO_Init(GPIOF,&GPIO_InitStruct);
  
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);    //Pin cs set a 1
  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_13,GPIO_PIN_RESET);  //Pin A0 set a 0
}






//  Funcion delay para las esperas configurando el lcd
//  ---------------------------------------------------------
//  ---------------------------------------------------------
//  ---------------------------------------------------------
//  ---------------------------------------------------------

void delay (uint32_t n_microsegundos)     
{

  __HAL_RCC_TIM7_CLK_ENABLE();
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 83;
  htim7.Init.Period = n_microsegundos - 1;

  HAL_TIM_Base_Init(&htim7);
  HAL_TIM_Base_Start(&htim7);
  
  while(!__HAL_TIM_GET_FLAG(&htim7, TIM_FLAG_UPDATE)){} //Bloqueante
  __HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);
  
  HAL_TIM_Base_Stop(&htim7);
  HAL_TIM_Base_DeInit(&htim7);
  
  __HAL_RCC_TIM7_CLK_DISABLE();
    
}



//  Condifuracion del SPI
//  ---------------------------------------------------------
//  ---------------------------------------------------------
//  ---------------------------------------------------------
//  ---------------------------------------------------------

void LCD_reset(void)
{
  initLCDPins();
  SPIdrv->Initialize(NULL);
  SPIdrv->PowerControl(ARM_POWER_FULL);
  SPIdrv->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA1 | ARM_SPI_MSB_LSB | ARM_SPI_DATA_BITS(8),2000000);

  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET); //RESET
  delay(1);
  
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);
  delay(1000);
  
}



//  Escritura dato LCD
//  ---------------------------------------------------------
//  ---------------------------------------------------------
//  ---------------------------------------------------------
//  ---------------------------------------------------------

static void LCD_wr_data(unsigned char data)
{


  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_13,GPIO_PIN_SET);

  SPIdrv->Send(&data,sizeof(data));
  while(SPIdrv->GetStatus().busy)
  {
  }
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);
  
}


//  Escritura comando LCD
//  ---------------------------------------------------------
//  ---------------------------------------------------------
//  ---------------------------------------------------------
//  ---------------------------------------------------------

static void LCD_wr_cmd(unsigned char cmd)//manda el comando a implementar a la pantalla wr_cmd(0xAF);       //se enciende el LCD(LCD on)  
{																					// wr_cmd(0xAE);       //Mantenemos apagado el LCD
 
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_13,GPIO_PIN_RESET);
  
  SPIdrv->Send(&cmd,sizeof(cmd));
  while(SPIdrv->GetStatus().busy)
  {
  }
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);
  
}


//  Inicializacion LCD
//  ---------------------------------------------------------
//  ---------------------------------------------------------
//  ---------------------------------------------------------
//  ---------------------------------------------------------
void  LCD_Init(void)
{
  int i;
  for(i=0;i<sizeof(initLCD);i++)
  {
    LCD_wr_cmd(initLCD[i]);
  }
  LCD_clearbuffer();
  i=0;
  LCD_update();
}

// Actualizacion LCD
//  ---------------------------------------------------------
//  ---------------------------------------------------------
//  ---------------------------------------------------------
//  ---------------------------------------------------------

void LCD_update(void)
{
  int i;
  LCD_wr_cmd(0x00);
  LCD_wr_cmd(0x10);
  LCD_wr_cmd(0xB0);
  for(i=0;i<128;i++)
  {
    LCD_wr_data(g_buffer[i]);
  }
  

  LCD_wr_cmd(0x00);
  LCD_wr_cmd(0x10);
  LCD_wr_cmd(0xB1);
  for(i=128;i<256;i++)
  {
    LCD_wr_data(g_buffer[i]);
  }
  

  LCD_wr_cmd(0x00);
  LCD_wr_cmd(0x10);
  LCD_wr_cmd(0xB2);
  for(i=256;i<384;i++)
  {
    LCD_wr_data(g_buffer[i]);
  }
  

  LCD_wr_cmd(0x00);
  LCD_wr_cmd(0x10);
  LCD_wr_cmd(0xB3);
  for(i=384;i<512;i++)
  {
    LCD_wr_data(g_buffer[i]);
  }
  
}


void symbolToLocalBuffer(uint8_t line, uint8_t symbol)
{
  uint8_t i, value1, value2;
  uint16_t offset=0;
  
  offset=25*(symbol-' ');

 if (line==1)
 {
  for (i=0; i<12;i++)
    {
     //contador=i+
        if((i+positionL1)<128)
        {
        value1=Arial12x12[offset+i*2+1];
        value2=Arial12x12[offset+i*2+2];
        g_buffer[i+positionL1]=value1;
        g_buffer[i+128+positionL1]=value2;
        }
      
    }
  positionL1=positionL1+Arial12x12[offset];
 }
 else
  {
    offset=25*(symbol-' ');
    for (i=0; i<12;i++)
    {
      if((i+positionL2)<128)
      {
        value1=Arial12x12[offset+i*2+1];
        value2=Arial12x12[offset+i*2+2];
        g_buffer[i+positionL2+256]=value1;
        g_buffer[i+positionL2+384]=value2;
      }
    }
    positionL2=positionL2+Arial12x12[offset];
  }


}

void LCD_write(char *text)
{
  static uint16_t i=0;

    while(text[i]!=NULL)
    {
      if((positionL1+12)<128)
      {
        symbolToLocalBuffer(1,text[i]);
      }
      else
      {
        symbolToLocalBuffer(2,text[i]);
      }
      
      i++;
     }
     LCD_update();
//     if((positionL2+12)>128)
//     {
        LCD_clearbuffer();
        positionL1=0;
        positionL2=0;
//     }
     
  i=0;
  
}
static void LCD_clearbuffer(void)
{
  uint16_t i=0;
  
  for(i=0;i<sizeof(g_buffer);i++)
  {
    g_buffer[i]=0x00;
  }
}

  










  



