#include "Thlcd.h"
#include "Thjoy.h"
#include "stdio.h"
#include "lcd.h"
 
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 //resetea el LCD y lo inicia para recoger la cola mandada por el joystick y lanzar un printf
 
osThreadId_t  tid_Thlcd;                        // thread id
static int8_t       joystick_value; 
static char   gesture[40];
 
void Threadlcd (void *argument);                   // thread function
static void writeValue(void);
 
int Init_Thlcd (void) {
  LCD_reset();
  LCD_Init();
  tid_Thlcd = osThreadNew(Threadlcd, NULL, NULL);
  if (tid_Thlcd == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Threadlcd (void *argument) {
 
  while (1) {
    //osThreadFlagsWait(LCDWRITE,osFlagsWaitAny,osWaitForever);
    osMessageQueueGet(mid_joyQueue,&joystick_value,NULL,osWaitForever);
   
    writeValue();
    
    ; // Insert thread code here...
    osThreadYield();                            // suspend thread
  }
}

static void writeValue(void)
{

  switch(joystick_value)
  {
    case 1:
      sprintf(gesture,"UP ");
      break;
    case 2:
      sprintf(gesture,"RIGHT ");
      break;
    case 3:
      sprintf(gesture,"DOWN ");
      break;
    case 4:
      sprintf(gesture,"LEFT ");
      break;
    case 5:
      sprintf(gesture,"CENTER cenyter gentub jhjgjbljki");
      break;
  }
  
  LCD_write(gesture);
  
}
