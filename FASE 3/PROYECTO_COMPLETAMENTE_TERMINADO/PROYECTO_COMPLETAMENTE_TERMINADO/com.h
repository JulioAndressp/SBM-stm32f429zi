#ifndef __COM_H
#define __COM_H

#include "stdio.h"
#include "string.h"
#include "RTE_Device.h"
#include "Driver_USART.h"
#include "stm32f4xx_hal.h"


//************************************QUEUE COM******************************************************

#define MSGQUEUE_OBJECTS_COM 10                     // number of Message Queue Objects

#define FLAG_COM 0x00455

int Init_MsgQueue_COM (void);
void Thread_MsgQueue_COM (void *argument);        // thread function 1
void Thread_COM (void *argument);
void myUSART_callback(uint32_t event);

 typedef struct {  // object data type 
	uint8_t RDA_REG_WRITE[12];
} MSGQUEUE_OBJ_t_COM;


#endif

