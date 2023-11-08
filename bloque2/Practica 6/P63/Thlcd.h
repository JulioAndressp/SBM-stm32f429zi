#ifndef _THLCD
 
  #define _THLCD
  #include "cmsis_os2.h"                          // CMSIS RTOS header file
  #define LCDWRITE  0x00000001U
  extern osThreadId_t tid_Thlcd;
  int Init_Thlcd (void);

#endif