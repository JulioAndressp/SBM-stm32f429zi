#ifndef __VOL_H
#define __VOL_H

#include "stm32f4xx_hal.h"

#define RESOLUTION_12B 4096U
#define VREF 3.3


#define MSGQUEUE_OBJECTS_VOL 4

 typedef struct {                                // object data type
  int vol;
} MSGQUEUE_OBJ_t_Vol;
 
void ADC1_pins_F429ZI_config(void);
float ADC_getVoltage(ADC_HandleTypeDef *hadc, uint32_t Channel);
int ADC_Init_Single_Conversion(ADC_HandleTypeDef *hadc, ADC_TypeDef  *ADC_Instance);

void Thread_Vol (void *argument);                   // thread function
int Init_Thread_Vol (void);
#endif



