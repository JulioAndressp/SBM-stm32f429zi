#ifndef __COM_H
#define __COM_H

#include "stdio.h"
#include "string.h"
#include "RTE_Device.h"
#include "Driver_USART.h"
#include "stm32f4xx_hal.h"


//************************************QUEUE COM******************************************************

#define MSGQUEUE_OBJECTS_COM 50                     // number of Message Queue Objects

#define GESTION_COM 0x00000004
#define FLAG_COM 0x00000008

int Init_MsgQueue_COM (void);
void Thread_MsgQueue_COM (void *argument);        // thread function 1
void Thread_COM (void *argument);

 typedef struct {  // object data type 
 char message[32];
} MSGQUEUE_OBJ_t_COM;

#endif