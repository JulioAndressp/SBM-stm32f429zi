#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "vol.h"

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thread_Vol;                        // thread id
 
osThreadId_t tid_Thread_MsgQueue_Vol;              // thread id 1
osMessageQueueId_t mid_Vol_Queue;
MSGQUEUE_OBJ_t_Vol msg_vol;

int Init_Thread_Vol (void) {

  mid_Vol_Queue = osMessageQueueNew(MSGQUEUE_OBJECTS_VOL, sizeof(MSGQUEUE_OBJ_t_Vol), NULL);

  tid_Thread_Vol = osThreadNew(Thread_Vol, NULL, NULL);

	
  return(0);
	
}

float value;

int valor;
void Thread_Vol (void *argument) {
	
ADC_HandleTypeDef adchandle; //handler definition
ADC1_pins_F429ZI_config(); //specific PINS configuration
	
int valor_anterior=0;
while (1) {
	
			osDelay(250);
			
			ADC_Init_Single_Conversion(&adchandle , ADC1);
			value=ADC_getVoltage(&adchandle,10);
			valor = (value*4.3)+1;


			if (valor_anterior != valor){
				valor_anterior = valor;
				msg_vol.vol= valor;
				osMessageQueuePut(mid_Vol_Queue, &msg_vol, NULL, 0U);	
				
				if(valor_anterior > 0 && valor_anterior <6)	{

					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13,GPIO_PIN_SET); //Apago Rojo
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11,GPIO_PIN_SET); //Apago Azul
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12,GPIO_PIN_RESET);//Verde
				}
				
				if(valor_anterior > 6 && valor_anterior < 11)	{

					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13,GPIO_PIN_RESET); //Rojo
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11,GPIO_PIN_SET); //Apago Azul
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12,GPIO_PIN_RESET);//Verde
				}
				
				if(valor_anterior > 10 && valor_anterior < 16)	{
					
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13,GPIO_PIN_RESET); //Rojo
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11,GPIO_PIN_SET); //Apago Azul
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12,GPIO_PIN_SET);//Apago Verde
				}
				
		}
			osThreadYield(); 
		}

	}





int ADC_Init_Single_Conversion(ADC_HandleTypeDef *hadc, ADC_TypeDef  *ADC_Instance)
{
	
   /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc->Instance = ADC_Instance;
  hadc->Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc->Init.Resolution = ADC_RESOLUTION_12B;
  hadc->Init.ScanConvMode = DISABLE;
  hadc->Init.ContinuousConvMode = DISABLE;
  hadc->Init.DiscontinuousConvMode = DISABLE;
  hadc->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc->Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc->Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc->Init.NbrOfConversion = 1;
  hadc->Init.DMAContinuousRequests = DISABLE;
 hadc->Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(hadc) != HAL_OK)
  {
    return -1;
  }
 
	
	return 0;
}

float ADC_getVoltage(ADC_HandleTypeDef *hadc, uint32_t Channel)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	HAL_StatusTypeDef status;
	uint32_t raw = 0;
	float voltage = 0;
	sConfig.Channel = Channel;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK) {
		return -1;
}
	HAL_ADC_Start(hadc);
	do {
		status = HAL_ADC_PollForConversion(hadc, 0); //This funtion uses the HAL_GetTick(), then it only can be executed when the OS is running
	}while (status != HAL_OK);
	raw = HAL_ADC_GetValue(hadc);
	voltage = raw*VREF/RESOLUTION_12B;
	return voltage;
}

void ADC1_pins_F429ZI_config(void){
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	/*PC0     ------> ADC1_IN10
    PC3     ------> ADC1_IN13
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		
		
  }
