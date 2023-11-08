#ifndef __TEMPERATURA_H
#define __TEMPERATURA_H

#include "stm32f4xx_hal.h"
#include "Driver_I2C.h"

#define REG_CONF 0x01
#define REG_TEM  0x00
#define LM75B 0x48


typedef struct {                                // object data type
  float temp;
} MSGQUEUE_OBJ_temp;


void Thread_MsgQueue_Temp (void *argument);         // thread function 
int Init_MsgQueue_Temperatura (void);
void I2C_Temp_Init(void);

 #define TEMPERATURA 0x02
 #define FLAG_TEMP 0x00006
 
 #define MSGQUEUE_OBJECTS_temp 5                     // number of Message Queue Objects
 
 
#endif 




