#include "Principal.h"
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_pwm;                        // thread id
osThreadId_t tid_pwm_tarjeta;
 bool iniciado = false;//Variable para mandar únicamente una señal al pwm y que no se esté mandando indefinidamente durante el modo reposo_tarjeta.
/*------- TIMER ----------*/
  TIM_HandleTypeDef tim2;//Timer
  TIM_OC_InitTypeDef octim4;//Canal
  TIM_HandleTypeDef tim2_tarjeta;//Timer_TF
  TIM_OC_InitTypeDef octim4_tarjeta;//Canal_TF
 
int Init_Pwm(void) {
 
  tid_pwm = osThreadNew(ThPwm, NULL, NULL);
  if (tid_pwm == NULL) {
    return(-1);
  }
   tid_pwm_tarjeta = osThreadNew(ThPwmTarjeta, NULL, NULL);
  if (tid_pwm_tarjeta == NULL) {
    return(-1);
  }
  return(0);
}

void ThPwmTarjeta (void *argument)
{
	Init_Zumb_TF();
	  while (1) {

		osThreadFlagsWait(INSERTAR_TARJETA,osFlagsWaitAll,osWaitForever);

    HAL_TIM_OC_Init(&tim2_tarjeta);//Instanciamos el timer
		HAL_TIM_OC_Start(&tim2_tarjeta,TIM_CHANNEL_4);//arrancamos el timer
		iniciado = true;
    osThreadYield();                          
  }
}



void ThPwm (void *argument) {
	
  Init_Zumb();
	
  while (1) {
		
		osThreadFlagsWait(ZUMBIDO,osFlagsWaitAll,osWaitForever);
   
		HAL_TIM_OC_Init(&tim2);//Instanciamos el timer
		HAL_TIM_OC_Start(&tim2,TIM_CHANNEL_4);//arrancamos el timer
		osDelay(200);
	  HAL_TIM_OC_Stop(&tim2,TIM_CHANNEL_4);
    osThreadYield();                          
  }
}
/*----------------------------------------------------------------------------
 *      Configuración pin del zumbador
 *---------------------------------------------------------------------------*/
void Init_Zumb(void)
	{
		      __HAL_RCC_GPIOA_CLK_ENABLE();
       //Pines
            GPIO_InitStruct.Pin = PWM_PIN_3;
            GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
            GPIO_InitStruct.Pull = GPIO_PULLUP ;
						GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
       //Habilitamos los puertos
            HAL_GPIO_Init(PUERTO_PWM_GPIOA, &GPIO_InitStruct);
			 //Configuramos el timer
  				__HAL_RCC_TIM2_CLK_ENABLE(); // Habilitamos el reloj del timer
    
						tim2.Instance = TIM2;
						tim2.Init.Prescaler = 999;//El valor del prescaler se calcula realizando : prescaler = (frecuencia del reloj(84M)/el valor que pones en el prescaler+1)-1. Tiene que ser menor de 2^[16]
						tim2.Init.Period = (84000/500)-1;//El valor del periodo se calcula realizando: periodo= solucion de arriba/frecuencia que tu quieres)-1.

			//Configuramos el canal
						octim4.OCMode = TIM_OCMODE_PWM2; // Ponemos el channel en modo toggle
						HAL_TIM_OC_ConfigChannel(&tim2,&octim4,TIM_CHANNEL_4);// configuramos el canal.
    
						HAL_TIM_OC_Init(&tim2);//Instanciamos el timer
				
	
	}
	
		void Init_Zumb_TF(void)
	{
		      __HAL_RCC_GPIOA_CLK_ENABLE();
       //Pines
            GPIO_InitStruct.Pin = PWM_PIN_3;
            GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
            GPIO_InitStruct.Pull = GPIO_PULLUP ;
						GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
       //Habilitamos los puertos
            HAL_GPIO_Init(PUERTO_PWM_GPIOA, &GPIO_InitStruct);
			 //Configuramos el timer
  				__HAL_RCC_TIM2_CLK_ENABLE(); // Habilitamos el reloj del timer
    
						tim2_tarjeta.Instance = TIM2;
						tim2_tarjeta.Init.Prescaler = 999;//El valor del prescaler se calcula realizando : prescaler = (frecuencia del reloj(84M)/el valor que pones en el prescaler+1)-1. Tiene que ser menor de 2^[16]
						tim2_tarjeta.Init.Period = (84000/2500)-1;//El valor del periodo se calcula realizando: periodo= solucion de arriba/frecuencia que tu quieres)-1.

			//Configuramos el canal
						octim4_tarjeta.OCMode = TIM_OCMODE_TOGGLE; // Ponemos el channel en modo toggle
						HAL_TIM_OC_ConfigChannel(&tim2_tarjeta,&octim4_tarjeta,TIM_CHANNEL_4);// configuramos el canal.
 
	}
	