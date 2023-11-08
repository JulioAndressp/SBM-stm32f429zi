
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TEMP_H
#define __TEMP_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "Driver_I2C.h"
#include "cmsis_os2.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define REG_CONF 0x01
#define REG_TEM  0x00
#define LM75B_ADD 0x48
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
 int Init_Temp (void);
 void ThtEmp (void *argument);
 void I2C_Temp_Init(void);
/* Exported variables ------------------------------------------------------- */
 extern float temperatura;
 extern ARM_DRIVER_I2C Driver_I2C1;
 /* Exported Signals ------------------------------------------------------- */
 #define TEMPERATURA 0x02
#endif /* __TEMP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

