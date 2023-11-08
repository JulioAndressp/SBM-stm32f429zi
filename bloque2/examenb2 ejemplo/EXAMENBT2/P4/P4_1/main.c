
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Driver_SPI.h"
#include "Arial12x12.h"
#ifdef _RTE_
#include "RTE_Components.h"             // Component selection
#endif
#ifdef RTE_CMSIS_RTOS2                  // when RTE component CMSIS RTOS2 is used
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#endif

#ifdef RTE_CMSIS_RTOS2_RTX5
/**
  * Override default HAL_GetTick function
  */
uint32_t HAL_GetTick (void) {
  static uint32_t ticks = 0U;
         uint32_t i;

  if (osKernelGetState () == osKernelRunning) {
    return ((uint32_t)osKernelGetTickCount ());
  }

  /* If Kernel is not running wait approximately 1 ms then increment 
     and return auxiliary tick counter value */
  for (i = (SystemCoreClock >> 14U); i > 0U; i--) {
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
  }
  return ++ticks;
}

#endif

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
  extern ARM_DRIVER_SPI Driver_SPI1;
  ARM_DRIVER_SPI* SPIdrv = &Driver_SPI1;
	extern TIM_HandleTypeDef tim7;
	unsigned char buffer[512];
	uint8_t positionL1 = 0;
	
/* Private function prototypes -----------------------------------------------*/
 static void SystemClock_Config(void);
 static void Error_Handler(void);
 
 GPIO_InitTypeDef GPIO_InitStruct;
 void init_SPI(void);
 void reset_lcd(void);
 
 void LCD_wr_data(unsigned char data);
 void LCD_wr_cmd(unsigned char cmd);
 
 void LCD_init(void);
 void LCD_update(void);
 void LCD_symbolToLocalBuffer_L1(uint8_t symbol);
 
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  HAL_Init();
  init_SPI();
  reset_lcd();
	LCD_init();
	LCD_symbolToLocalBuffer_L1('H');
	
	LCD_update();
	
  /* Configure the system clock to 168 MHz */
  SystemClock_Config();
  SystemCoreClockUpdate();

  /* Add your application code here
     */

#ifdef RTE_CMSIS_RTOS2
  /* Initialize CMSIS-RTOS2 */
  osKernelInitialize ();

  /* Create thread functions that start executing, 
  Example: osThreadNew(app_main, NULL, NULL); */

  /* Start thread execution */
  osKernelStart();
#endif

  /* Infinite loop */
  while (1){}
}

  
void LCD_symbolToLocalBuffer_L1(uint8_t symbol)
{
  uint8_t i, valor1, valor2 = 0;
  uint16_t comienzo = 0;
	
  
  comienzo = 25 * (symbol - ' '); //Avanza a la linea donde se encuentra la letra que quiero representar
	// todas las letras de arial 12x12 ocupan 25 bytes (1 columna = 1 byte)
  
  for (i = 0; i<12; i++){ //Vamos a escribir las 12 primeras columnas de la letra
   
     valor1 = Arial12x12 [comienzo+i*2+1]; // valor1= arial[1,3,5,7,9,....]
     valor2 = Arial12x12 [comienzo+i*2+2]; // valor2= arial[2,4,6,8,10,....]

     buffer [i /*+ positionL1*/] = valor1; //pagina 0 escribimos valores impares de arial
     buffer [i+128 /*+ positionL1*/] = valor2; //pagina 1 escribimos  valores pares de arial
   
  }
  
  //positionL1 = positionL1 + Arial12x12 [comienzo]; // posicionL1 indica la columna donde se debe comenza a escribir 
  }

static void delay(uint32_t n_microsegundos)//Funcion que usaremos para el delay
{
// Configurar y arrancar el timer para generar un evento pasados n_microsegundos
	tim7.Instance = TIM7;
	tim7.Init.Prescaler = 20; //Prescaler=84M/21=4Mhz
	tim7.Init.Period = (n_microsegundos/4)-1;
	
	__HAL_RCC_TIM7_CLK_ENABLE();
	HAL_TIM_Base_Init(&tim7);//estas dos lineas sirven para arrancar el timer
	HAL_TIM_Base_Start(&tim7);  
	
 // Esperar a que se active el flag del registro de Match
	while(TIM7 -> CNT){};
	
 	HAL_TIM_Base_DeInit(&tim7);
	HAL_TIM_Base_Stop(&tim7);

}

	static void LCD_update(void)
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


static void reset_lcd(void)
{
  delay(2); // Esperamos dos microsegundos
	
  //Apagamos lcd
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);//RESET
  delay(1000);
	
	//Encendemos lcd
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);//RESET

}
 static void LCD_init(void){
	
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
 delay(1000);
 // Seleccionar CS = 1;
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);//CS
}


static void LCD_wr_data(unsigned char data)//Escribe un dato en lcd.
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
	 delay(1000);
 // Seleccionar CS = 1;
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);//CS
 
}


static void init_SPI(void)
{
    //Configuramos el SPI
  SPIdrv->Initialize(NULL);
	SPIdrv->PowerControl(ARM_POWER_FULL);
	SPIdrv->Control(ARM_SPI_MODE_MASTER|ARM_SPI_CPOL1_CPHA1|ARM_SPI_MSB_LSB|ARM_SPI_SS_MASTER_UNUSED|ARM_SPI_DATA_BITS(8),20000000);
   
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
	

static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported */
  if (HAL_GetREVID() == 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */ 

/**
  * @}
  */ 



