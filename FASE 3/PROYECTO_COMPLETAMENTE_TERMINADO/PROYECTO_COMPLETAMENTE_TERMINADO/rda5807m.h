 #ifndef __RDA5807M_H
#define __RDA5807M_H
 
#include "Driver_I2C.h"



#define EndingFreq      108.00
#define DefaultFreq     87.50


#define RDA_TUNE_ON     0x0010
#define RDA_TUNE_OFF    0xFFEF
#define RDA_POWER_OFF   0x1500
#define RDA_SEEK_UP     0x0300
#define RDA_SEEK_DOWN   0x0100
#define RDA_SEEK_STOP   0xFCFF
#define RDA_RDS_ON      0x0008
#define RDA_RDS_OFF     0xFFF7
#define FRECUENCIA_SIGNAL 0x68
#define StartingFreq    87.00
#define FLAG_MAIN_RDA 0x255
#define FRECUENCIA_UPDATE 0x96
#define VOLUMEN_UPDATE 		0x56

#define RDA_BASS_ON     0x1000
#define RDA_BASS_OFF    0xEFFF

#define RDA_DHIZ        0x8000
#define RDA_MUTE        0x8000
#define RDA_MONO_ON     0x2000
#define RDA_MONO_OFF    0xDFFF


#define RDA_RCLK_MODE   0x0800
#define RDA_RCLK_DIRECT 0x0400

#define RDA_SEEK_DOWN   0x0100
#define RDA_SEEK_STOP   0xFCFF
#define RDA_SEEK_WRAP   0x0080
#define RDA_SEEK_NOWRAP 0xFF7F
#define RDA_CLK_0327    0x0000
#define RDA_CLK_1200    0x0010
#define RDA_CLK_2400    0x0050
#define RDA_CLK_1300    0x0020
#define RDA_CLK_2600    0x0060
#define RDA_CLK_1920    0x0030
#define RDA_CLK_3840    0x0070
#define RDA_NEW         0x0004
#define RDA_RESET       0x0002
#define RDA_POWER       0x0001

/* Register 0x03H (16Bits) */
#define RDA_TUNE_OFF    0xFFEF

/* Register 0x03H (16Bits) */
#define RDA_TUNE_ON     0x0010
#define RDA_TUNE_OFF    0xFFEF

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
void RDA5807M_BassBoost(void);
void RDA5807M_RDS(void);

  typedef struct {                                // object data type
  int Id_RDA;
  float freq; //parametro para actualizar la frecuencia
	int vol;
} MSGQUEUE_OBJ_t_RDA;
	
  typedef struct {        
		// object data type
	float freq_ultima; //frecuencia actual
} MSGQUEUE_OBJ_t_RDA_pp;
	

	  typedef struct {        
	uint8_t RDA_REG_WRITE[12];
} MSGQUEUE_OBJ_t_RDA_pp_com;


#define FLAG_RDA 0x2002
#define FLAG_RDA2 0x222002

#define MSGQUEUE_OBJECTS_RDA 35                     // number of Message Queue Objects
#define MSGQUEUE_OBJECTS_RDA_pp 39                     // number of Message Queue Objects

int Init_MsgQueue_RDA (void);
void Thread_MsgQueue_RDA (void *argument);

void callback_rda (uint32_t event); //Equivalente a una rutina de interrupcion
#endif
