#ifndef __CLOCK_H
#define __CLOCK_H
/* Includes ------------------------------------------------------------------*/
#include "cmsis_os2.h" 
/* Variables ------------------------------------------------------------------*/
extern int horas   ;
extern int minutos ;
extern int segundos;
/* Señales ------------------------------------------------------------------*/
#define START_CLOCK 0x01
#define ACTUALIZALCD 0x02
/* Prototipos de funciones --------------------------------------------------*/
int clock_timer (void);
int Init_clock (void);
#endif /* __CLOCK_H */
