#ifndef __THREADLED2_H
#define __THREADLED2_H

#include "main.h"
#include "cmsis_os2.h"
#include "Thread.h"

#define EMPIEZALED2 0x01

int Init_Thread_LED2 (void);
osThreadId_t tid_Thread_LED2; 



#endif