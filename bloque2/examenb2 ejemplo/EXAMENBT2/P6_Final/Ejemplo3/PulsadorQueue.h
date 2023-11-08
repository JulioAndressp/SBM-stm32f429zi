#ifndef __PULSADOR_QUEUE_H
#define __PULSADOR_QUEUE_H

#include "stm32f4xx_hal.h"

#define MSGQUEUE_OBJECTS 16                     // number of Message Queue Objects
 
typedef struct {                                // object data type
  uint8_t Buf[32];
  uint8_t Idx;
} MSGQUEUE_OBJ_t;

void Thread_MsgQueue1 (void *argument);         // thread function 1
void Thread_MsgQueue2 (void *argument);         // thread function 2

#endif
