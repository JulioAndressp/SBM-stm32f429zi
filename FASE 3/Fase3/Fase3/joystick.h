#ifndef __JOYSTICK_H
#define __JOYSTICK_H


#include "stm32f4xx_hal.h"

//ESTOS DEFINES SON: LOS DOS PRIMEROS BITS INDICAN SI SON LARGAS O CORTAS(10-LARGA 01-CORTA) Y LOS DEMAS INDICAN QUE BOTON PULSADO

#define IDENTIFICADOR_IZQUIERDA 10
#define IDENTIFICADOR_DERECHA 11
#define IDENTIFICADOR_ABAJO 12
#define IDENTIFICADOR_ARRIBA 13
#define IDENTIFICADOR_CENTRO_CORTA 14
#define IDENTIFICADOR_CENTRO_LARGA 15

#define GESTION_PULSADOR 0x203040

#define PWM_SIGNAL 0x0013

static void Timer1_Callback (void const *arg);
static void Timer2_Callback (void const *arg);
int Init_Timers_Joystick (void);

//QUEUE

#define MSGQUEUE_OBJECTS_JOY 20                     // number of Message Queue Objects

int Init_MsgQueue_Pulsadores (void);
void Thread_MsgQueue_Joystick (void *argument);         // thread function 1

 typedef struct {                                // object data type
  int Idx;
} MSGQUEUE_OBJ_t_joy;
 

#endif
