/**
  ******************************************************************************
  * @file    Templates/Src/main.c 
  * @author  MCD Application Team
  * @brief   STM32F4xx HAL API Template project 
  *
  * @note    modified by ARM
  *          The modifications allow to use this file as User Code Template
  *          within the Device Family Pack.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Driver_SPI.h" //Necesario para que funcione incluir la biblioteca
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
static  GPIO_InitTypeDef GPIO_InitStruct; //GPIO Struct
extern TIM_HandleTypeDef tim7; //Estanciamos el timer
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

extern ARM_DRIVER_SPI Driver_SPI1;	    //Configuracion del Driver en el SP1
ARM_DRIVER_SPI* SPIdrv = &Driver_SPI1;	//Configuracion del Driver en el SP1

uint32_t n_microsegundos; // Variable para gestionar el retardo


//Deberiamos usar el timer7
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);


static void Reset_LCD(void); //Funcion que solicitan
static void SPI_Init(void);  //Funcion del SPI inicializacion
static void TIM7_HALDelay(uint32_t n_microsegundos); //Funcion Timer
static void GPIO_Init(void); //Inicializar GPIO PINES


//PROYECTO_2
static void LCD_wr_data(unsigned char data);//Realiza la escritura de un dato en el LCD
void LCD_wr_cmd(unsigned char cmd);//Escribe un comando en el panel LCD

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, Flash preread and Buffer caches
       - Systick timer is configured by default as source of time base, but user 
             can eventually implement his proper time base source (a general purpose 
             timer for example or other time source), keeping in mind that Time base 
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
             handled in milliseconds basis.
       - Low Level Initialization
     */
	
	
  HAL_Init();



  /* Configure the system clock to 168 MHz */
  SystemClock_Config();
  SystemCoreClockUpdate();

  /* Add your application code here
     */
		 
		 	
	SPI_Init();
	GPIO_Init();
	Reset_LCD();

#ifdef RTE_CMSIS_RTOS2
  /* Initialize CMSIS-RTOS2 */
  osKernelInitialize ();

  /* Create thread functions that start executing, 
  Example: osThreadNew(app_main, NULL, NULL); */

  /* Start thread execution */
  osKernelStart();
#endif

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 168
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
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

static void Reset_LCD(void){//Cada vez que enciendo el programa tengo que inicializar y resetear
	
		//USAMOS TIMER 7 PARA APAGAR PANTALLA Y ESPERAR UN TIEMPO	DE 1MS
	  TIM7_HALDelay(1000); // Esperamos un milisegundo
    //Apagamos el lcd
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);//RESET
    TIM7_HALDelay(1000);
	//Encendemos el lcd
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);//RESET

}

static void SPI_Init(void){ //CONFIGURACION DEL SPI
	
	
/*o Modo master (ARM_SPI_MODE_MASTER)
	o CPOL1 y CPHA1 (ARM_SPI_CPOL1_CPHA1)
	o Organización de la información MSB a LSB (ARM_SPI_MSB_LSB)
	o 8 bits de datos (ARM_SPI_DATA_BITS(8))
	o Frecuencia del SCLK, 20MHz*/
	
	SPIdrv->Initialize(NULL);
	SPIdrv->PowerControl(ARM_POWER_FULL);
	SPIdrv->Control(ARM_SPI_MODE_MASTER|ARM_SPI_CPOL1_CPHA1|ARM_SPI_MSB_LSB|ARM_SPI_SS_MASTER_UNUSED|ARM_SPI_DATA_BITS(8),20000000);
	
}
static void GPIO_Init(void){
	
	
	/*************************************
							  PUERTO A
	*************************************/
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	//RESET
	GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/*************************************
							  PUERTO D
	*************************************/
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	
	//CS
   GPIO_InitStruct.Pin = GPIO_PIN_14;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_PULLUP;
   HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	
	/*************************************
							  PUERTO F
	*************************************/
	 __HAL_RCC_GPIOF_CLK_ENABLE();
	 
	
	//A0
	  GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	//Inicialmente se colocan los tres pines a 1
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);//CS
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);//RESET
    HAL_GPIO_WritePin(GPIOF,GPIO_PIN_13,GPIO_PIN_RESET);//A0

}


static void TIM7_HALDelay(uint32_t n_microsegundos){
	
	tim7.Instance = TIM7;					//Esta linea instancia el timer
	tim7.Init.Prescaler = 20; //84/21 = 4 MHz
	tim7.Init.Period = (n_microsegundos/4)-1; //Pone el periodo		
	
	__HAL_RCC_TIM7_CLK_ENABLE();		//Encendemos el periferico TIM7 HAL
	
	//Esperamos a que se active el FLAG del MATCH
	
	while(TIM7 -> CNT){};  
	HAL_TIM_Base_Init(&tim7);			//Configura el timer
	HAL_TIM_Base_Start_IT(&tim7);	//Inicializa el timer
	
	//EJEMPLO DE CONFIGURACION
	//Se requiere el LED con un periodo 3s(1.5S apagado y 1.5S encendido)
	//Lo resolveremos sabiendo que el TIMER7 esta conectado a la APB1 segun el manual del STM32F429
	//Entonces nos entran 84 MHz / Prescaler = 84 000 = 10 kHz 
	//Como queremos que el timer salte cada 1.5S entonces Period = 1.5S / 10kHz = 14999

}

void LCD_wr_data(unsigned char data){
	
	ARM_SPI_STATUS status;
	
 // Seleccionar CS = 0;
	
 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);
	
 // Seleccionar A0 = 1;
	
 HAL_GPIO_WritePin(GPIOF,GPIO_PIN_13,GPIO_PIN_SET);
	
 // Escribir un dato (data) usando la función SPIDrv->Send(…);
	
	SPIdrv->Send(&data,sizeof(data));//El tamano es en BYTES
	
 // Esperar a que se libere el bus SPI;
	
			 do{											 //REALIZAMOS ESTE BUCLE HASTA QUE ESTE LIBRE 
			 status = SPIdrv->GetStatus();
		   }while(status.busy);
			 
			 TIM7_HALDelay(1000); //Y esperamos 1 ms hasta que se reinicie el LCD
			 
 // Seleccionar CS = 1;
			 
			 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);

}

void LCD_wr_cmd(unsigned char cmd){
	
	ARM_SPI_STATUS status;
	
 // Seleccionar CS = 0;
	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);
	
 // Seleccionar A0 = 0;

	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_13,GPIO_PIN_RESET);
	
 // Escribir un comando (cmd) usando la función SPIDrv->Send(…);
	
	SPIdrv->Send(&cmd,sizeof(cmd));//El tamano es en BYTES
	
 // Esperar a que se libere el bus SPI;
	
				do{											 //REALIZAMOS ESTE BUCLE HASTA QUE ESTE LIBRE 
			 status = SPIdrv->GetStatus();
		   }while(status.busy);
	
			TIM7_HALDelay(1000);  //Y esperamos 1 ms hasta que se reinicie el LCD
			 
 // Seleccionar CS = 1;
			 
			 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
