#include "cmsis_os2.h"                          // CMSIS RTOS header file

#ifndef _THJOY
 
  #define _THJOY
  
  #define JOYINTERRUPT            0x00000001U
  extern osThreadId_t             tid_Thjoy;
  extern osMessageQueueId_t       mid_joyQueue;
  
  int Init_Thjoy (void);

#endif


