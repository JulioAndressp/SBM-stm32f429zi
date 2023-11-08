 #ifndef __RDA5807M_H
#define __RDA5807M_H
 
#include "Driver_I2C.h"


#define StartingFreq    87.00
#define EndingFreq      108.00
#define DefaultFreq     87.50


#define RDA_TUNE_ON     0x0010
#define RDA_TUNE_OFF    0xFFEF
#define RDA_POWER       0x1001
#define RDA_SEEK_UP     0x0300
#define RDA_SEEK_DOWN   0x0100
#define RDA_SEEK_STOP   0xFCFF
#define RDA_RDS_ON      0x0008
#define RDA_RDS_OFF     0xFFF7
#define FRECUENCIA_SIGNAL 0x68
#define StartingFreq    87.00
#define FLAG_MAIN_RDA 0x255
#define FRECUENCIA_UPDATE 0x96

void RDA5807M_Readregisters(void);
void RDA5807M_Frequency(float Freq);
void RDA5807M_SeekDown(void);
void RDA5807M_SeekUp(void);
void RDA5807M_AFC(void);
void RDA5807M_Reset(void);
void RDA5807M_Volume(int vol);
void RDA5807M_PowerOn(void);
void RDA5807M_Init(void);
void RDA5807M_WriteAll(void);
void I2C_RDA_Init(void);
void RDA5807M_Mute(void);
void RDA5807M_PowerOff(void);

  typedef struct {                                // object data type
  int Id_RDA;
  float freq;
	int vol;
} MSGQUEUE_OBJ_t_RDA;

#define FLAG_RDA 0x2002
#define FLAG_RDA2 0x222002

#define MSGQUEUE_OBJECTS_RDA 16                     // number of Message Queue Objects

int Init_MsgQueue_RDA (void);
void Thread_MsgQueue_RDA (void *argument);

void callback_rda (uint32_t event); //Equivalente a una rutina de interrupcion
#endif
