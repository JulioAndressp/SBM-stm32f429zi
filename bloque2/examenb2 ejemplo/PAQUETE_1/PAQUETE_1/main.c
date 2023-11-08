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
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

void GPIO_Init(void);

static void TIM2_Init(void);
static void TIM3_Init(void);
static void TIM4_Init(void);
static void TIM5_Init(void);
static void TIM6_Init(void);
static void TIM7_Init(void);


extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;

GPIO_InitTypeDef GPIO_InitStruct;







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

	GPIO_Init();

	//TIM2_Init();
	//TIM3_Init();
	TIM4_Init();
	//TIM5_Init();
	//TIM6_Init();
	//TIM7_Init();

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
  *            PLL_M                          = 25
  *            PLL_N                          = 336
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
																				//SYSCLK=(HSE_VALUE/4)*(N/P) 
  RCC_OscInitStruct.PLL.PLLM = 4;				//SI QUEREMOS UN SYSCLK DE 168 MHz M=4 N=168 y P=2
  RCC_OscInitStruct.PLL.PLLN = 168;			//SI QUEREMOS UN SYSCLK DE 96 MHz M=4 N=96 y P=2
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

void GPIO_Init(void){  //CONFIGURA LEDS Y PULSADOR

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    GPIO_InitStruct.Pin = GPIO_PIN_0;        //LED1
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = GPIO_PIN_7;        //LED2
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = GPIO_PIN_14;        //LED3
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


  //Inicializacion del puerto C
    __HAL_RCC_GPIOC_CLK_ENABLE();


  //Configuracion del pulsador B1 PUERTO 13
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode= GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


   HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}



//TIMER DE PROPOSITO GENERAL 2,3,4,5
//TIMER BASICOS 6 - 7

//Como configurar el TIMER

//Imaginamos que queremos que el LED se encienda y apague cada 1 s, es decir T = 2s y F = 0'5 Hz y SYSCLK = 96MHz
//Calculamos el preescaler que sera la busqueda de un numero que nos convenga, el preescaler tiene que estar entre 0 -- 2^16 (65535)
//SYSCLK/Prescaler = 10K  , a la hora de introducir el prescaler en la linea htim2.Init.Prescaler = X-1; HAY QUE RESTARLE UNO SI EL PRESCALER ES 9600 , PONDREMOS 9599
//Una vez con esa salida haremos 10K/Y = Frecuencia deseada(en este caso 0,5 Hz), Y sera lo que coloquemos en la linea del period
//�Y-1! sera lo que pongamos en la linea de : htim2.Init.Period = Y-1; EN ESTE CASO ES 19999

static void TIM2_Init(void){  //	Como usar el tim7

    htim2.Instance = TIM2;                    //Esta linea instancia el timer
    htim2.Init.Prescaler = 83999;        //Pone el valor al preescaler // Apartado A = 47999 // Apartado B = 8399
    htim2.Init.Period = 4999;              //Pone el periodo // Apartado A = 499 // Apartado B = 14999

    HAL_NVIC_EnableIRQ(TIM2_IRQn);    //Encendemos el periferico IRQ
    __HAL_RCC_TIM2_CLK_ENABLE();        //Encendemos el periferico TIM7

    HAL_TIM_Base_Init(&htim2);            //Configura el timer
    HAL_TIM_Base_Start_IT(&htim2);    //Inicializa el timer
	
}

static void TIM3_Init(void){  //	Como usar el tim7

    htim3.Instance = TIM3;                    //Esta linea instancia el timer
    htim3.Init.Prescaler = 83999;        //Pone el valor al preescaler // Apartado A = 47999 // Apartado B = 8399
    htim3.Init.Period = 4999;              //Pone el periodo // Apartado A = 499 // Apartado B = 14999

    HAL_NVIC_EnableIRQ(TIM3_IRQn);    //Encendemos el periferico IRQ
    __HAL_RCC_TIM3_CLK_ENABLE();        //Encendemos el periferico TIM7

    HAL_TIM_Base_Init(&htim3);            //Configura el timer
    HAL_TIM_Base_Start_IT(&htim3);    //Inicializa el timer
}

static void TIM4_Init(void){  //	Como usar el tim7

    htim4.Instance = TIM4;                    //Esta linea instancia el timer
    htim4.Init.Prescaler = 83999;        //Pone el valor al preescaler // Apartado A = 47999 // Apartado B = 8399
    htim4.Init.Period = 4999;              //Pone el periodo // Apartado A = 499 // Apartado B = 14999

    HAL_NVIC_EnableIRQ(TIM4_IRQn);    //Encendemos el periferico IRQ
    __HAL_RCC_TIM4_CLK_ENABLE();        //Encendemos el periferico TIM7

    HAL_TIM_Base_Init(&htim4);            //Configura el timer
    HAL_TIM_Base_Start_IT(&htim4);    //Inicializa el timer
}

static void TIM5_Init(void){  //	Como usar el tim7

    htim5.Instance = TIM5;                    //Esta linea instancia el timer
    htim5.Init.Prescaler = 83999;        //Pone el valor al preescaler // Apartado A = 47999 // Apartado B = 8399
    htim5.Init.Period = 4999;              //Pone el periodo // Apartado A = 499 // Apartado B = 14999

    HAL_NVIC_EnableIRQ(TIM5_IRQn);    //Encendemos el periferico IRQ
    __HAL_RCC_TIM5_CLK_ENABLE();        //Encendemos el periferico TIM7

    HAL_TIM_Base_Init(&htim5);            //Configura el timer
    HAL_TIM_Base_Start_IT(&htim5);    //Inicializa el timer
}

static void TIM6_Init(void){  //	Como usar el tim6

    htim6.Instance = TIM6;                    //Esta linea instancia el timer
    htim6.Init.Prescaler = 83999;        //Pone el valor al preescaler // Apartado A = 47999 // Apartado B = 8399
    htim6.Init.Period = 4999;              //Pone el periodo // Apartado A = 499 // Apartado B = 14999

    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);    //Encendemos el periferico IRQ
    __HAL_RCC_TIM6_CLK_ENABLE();        //Encendemos el periferico TIM7

    HAL_TIM_Base_Init(&htim6);            //Configura el timer
    HAL_TIM_Base_Start_IT(&htim6);    //Inicializa el timer
}

static void TIM7_Init(void){  //	Como usar el tim7

    htim7.Instance = TIM7;                    //Esta linea instancia el timer
    htim7.Init.Prescaler = 83999;        //Pone el valor al preescaler // Apartado A = 47999 // Apartado B = 8399
    htim7.Init.Period = 4999;              //Pone el periodo // Apartado A = 499 // Apartado B = 14999

    HAL_NVIC_EnableIRQ(TIM7_IRQn);    //Encendemos el periferico IRQ
    __HAL_RCC_TIM7_CLK_ENABLE();        //Encendemos el periferico TIM7

    HAL_TIM_Base_Init(&htim7);            //Configura el timer
    HAL_TIM_Base_Start_IT(&htim7);    //Inicializa el timer
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
