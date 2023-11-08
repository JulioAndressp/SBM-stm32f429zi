/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VOL_H
#define __VOL_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"  

/* Exported types ------------------------------------------------------------*/
typedef struct {                                // object data type
  uint8_t Buf[32];
  uint8_t Idx;
} MSGQUEUE_VOL_t;
/* Exported constants --------------------------------------------------------*/
#define RESOLUTION_12B 4096U
#define VREF 3.3f
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
	void ADC1_pins_F429ZI_config(void);
	int ADC_Init_Single_Conversion(ADC_HandleTypeDef *, ADC_TypeDef  *);
	float ADC_getVoltage(ADC_HandleTypeDef * , uint32_t );
  /* Exported thread functions,  
  Example: extern void app_main (void *arg); */
  int Init_Vol (void);
  void Th_ColaVol (void *argument);
/* Exported Variables ------------------------------------------------------- */
	extern MSGQUEUE_VOL_t msgvol;
  extern osThreadId_t tid_vol;
#endif /* __VOL_H */
