#include "cmsis_os2.h"
#include "rda5807m.h"
#include "string.h"
/*----------------------------------------------------------------------------
 *      Message Queue creation & usage
 *---------------------------------------------------------------------------*/
 
uint16_t RDA5807M_WriteRegDef[6] ={0xC004,0x0000,0x0100,0x84D4,0x4000,0x0000}; // initial data

uint16_t RDA5807M_WriteReg[8];

uint16_t RDA5807M_ReadReg[8];

int power,volume,mute,afc,block1,block2,block3,block4;
int bass=0;
int rds = 1;
int rdsready,tuneok,nochannel,rdssynchro,stereo,signal,fmready,fmstation,rdsblockerror;
float freq;
float freq2;
 
osMessageQueueId_t mid_MsgQueue_RDA;               		 // message queue id
osMessageQueueId_t mid_MsgQueue_RDA_pp;                // message queue id
osMessageQueueId_t mid_MsgQueue_RDA_pp_com;

osThreadId_t tid_Thread_MsgQueue_RDA;              
 
MSGQUEUE_OBJ_t_RDA msg_RDA;			
MSGQUEUE_OBJ_t_RDA_pp msg_RDA_pp; //Cola que va desde la radio al principal para pasar la frecuencia
MSGQUEUE_OBJ_t_RDA_pp_com msg_RDA_pp_com;

int Init_MsgQueue_RDA (void) {
 
  mid_MsgQueue_RDA = osMessageQueueNew(MSGQUEUE_OBJECTS_RDA, sizeof(MSGQUEUE_OBJ_t_RDA), NULL);
	mid_MsgQueue_RDA_pp = osMessageQueueNew(MSGQUEUE_OBJECTS_RDA_pp, sizeof(MSGQUEUE_OBJ_t_RDA_pp), NULL);
	mid_MsgQueue_RDA_pp_com = osMessageQueueNew(MSGQUEUE_OBJECTS_RDA_pp, sizeof(MSGQUEUE_OBJ_t_RDA_pp_com), NULL);
	
  tid_Thread_MsgQueue_RDA = osThreadNew(Thread_MsgQueue_RDA, NULL, NULL);

 
  return(0);
}
 
 
void Thread_MsgQueue_RDA (void *argument) {
	
  osStatus_t status;
	I2C_RDA_Init();

	
  while (1) {
		
		
    status = osMessageQueueGet(mid_MsgQueue_RDA, &msg_RDA, NULL, osWaitForever);   // wait for message
		
		
    if (status == osOK) {
						
			
			if(msg_RDA.Id_RDA == RDA_POWER) {
					RDA5807M_Reset();
					RDA5807M_Readregisters();

			}
			if(msg_RDA.Id_RDA == RDA_SEEK_UP){
					RDA5807M_SeekUp();
					RDA5807M_Readregisters();
				

			}
			if(msg_RDA.Id_RDA == RDA_SEEK_DOWN){
					RDA5807M_SeekDown();
					RDA5807M_Readregisters();

					
			}
			if(msg_RDA.Id_RDA == VOLUMEN_UPDATE){
					RDA5807M_Volume(msg_RDA.vol);
					RDA5807M_Readregisters();

			}

			if (msg_RDA.Id_RDA == FRECUENCIA_UPDATE){
					RDA5807M_Frequency(msg_RDA.freq);
					RDA5807M_Readregisters();
			}

			if (msg_RDA.Id_RDA == RDA_POWER_OFF){
					RDA5807M_PowerOff();
					RDA5807M_Readregisters();
			}

    }
								
  }
}


extern ARM_DRIVER_I2C Driver_I2C2;
static ARM_DRIVER_I2C *I2Crda = &Driver_I2C2;


//RDA5807M(PinName sda, PinName scl) : i2c(sda, scl)

void I2C_RDA_Init(void)
{
	  I2Crda-> Initialize(callback_rda);
	  I2Crda-> PowerControl(ARM_POWER_FULL);
		I2Crda-> Control (ARM_I2C_BUS_SPEED,ARM_I2C_BUS_SPEED_FAST);
		I2Crda-> Control (ARM_I2C_BUS_CLEAR,0);
}

void RDA5807M_WriteAll(void)
{ 
    uint8_t buf[12];  
    int i,x = 0;
    for(i=0; i<12; i=i+2){
        buf[i] = RDA5807M_WriteReg[x] >> 8;
        x++;}
    x = 0;
    for(i=1; i<13; i=i+2){
        buf[i] = RDA5807M_WriteReg[x] & 0xFF;
        x++;}
    I2Crda->MasterTransmit(0x10, buf, 12, false); 	
		osThreadFlagsWait(FLAG_RDA,osFlagsWaitAny,osWaitForever); //Esperamos al callback para que no este todo el rato consumiendo

}

void RDA5807M_Init(void){ 
    int i;
    for(i=0; i<6; i++){
        RDA5807M_WriteReg[i] = RDA5807M_WriteRegDef[i];
        RDA5807M_WriteAll();
				memcpy(msg_RDA_pp_com.RDA_REG_WRITE,RDA5807M_WriteReg,sizeof(RDA5807M_WriteReg));	
				osMessageQueuePut(mid_MsgQueue_RDA_pp_com, &msg_RDA_pp_com, NULL, 0U);
    }
}

void RDA5807M_PowerOn(void){
    RDA5807M_WriteReg[1] = RDA5807M_WriteReg[1] | RDA_TUNE_ON;
    RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] | RDA_POWER;
    RDA5807M_WriteAll(); power=1;
	
		memcpy(msg_RDA_pp_com.RDA_REG_WRITE,RDA5807M_WriteReg,sizeof(RDA5807M_WriteReg));	
		osMessageQueuePut(mid_MsgQueue_RDA_pp_com, &msg_RDA_pp_com, NULL, 0U);
    RDA5807M_WriteReg[1] = RDA5807M_WriteReg[1] & 0xFFEF;   //Disable tune after PowerOn operation
}

void RDA5807M_Reset(void){
  RDA5807M_Init();
  RDA5807M_PowerOn();
	//RDA5807M_Volume(4);
		osDelay(150);
}

void RDA5807M_Volume(int vol){
    if(vol > 15){
        vol = 15;
    }
    if(vol < 0){
        vol = 0;
    }
    RDA5807M_WriteReg[3] = (RDA5807M_WriteReg[3] & 0xFFF0)| vol;   // Set New Volume
    volume=vol;
    RDA5807M_WriteAll();
		
		memcpy(msg_RDA_pp_com.RDA_REG_WRITE,RDA5807M_WriteReg,sizeof(RDA5807M_WriteReg));	
		osMessageQueuePut(mid_MsgQueue_RDA_pp_com, &msg_RDA_pp_com, NULL, 0U);
}

void RDA5807M_Mute(void){
    if ((RDA5807M_WriteReg[0] & 0x8000)==0){
        RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] | 0x8000;
        mute=0;
    }
    else{
        RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] & 0x7FFF;
        mute=1;
    }
    RDA5807M_WriteAll();
}


void RDA5807M_SeekUp(void){
    RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] | RDA_SEEK_UP;   // Set Seek Up
    RDA5807M_WriteAll();
	
		memcpy(msg_RDA_pp_com.RDA_REG_WRITE,RDA5807M_WriteReg,sizeof(RDA5807M_WriteReg));	
		osMessageQueuePut(mid_MsgQueue_RDA_pp_com, &msg_RDA_pp_com, NULL, 0U);
	
    RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] & RDA_SEEK_STOP;   // Disable Seek
		osDelay(400);

}
 
void RDA5807M_SeekDown(void){
    RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] | RDA_SEEK_DOWN;   // Set Seek Down
    RDA5807M_WriteAll();
	
		memcpy(msg_RDA_pp_com.RDA_REG_WRITE,RDA5807M_WriteReg,sizeof(RDA5807M_WriteReg));	
		osMessageQueuePut(mid_MsgQueue_RDA_pp_com, &msg_RDA_pp_com, NULL, 0U);
	
    RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] & RDA_SEEK_STOP;   // Disable Seek
		osDelay(400);
}

void RDA5807M_Frequency(float Freq){
    int Channel;
    Channel = ((Freq-StartingFreq)/0.1)+0.05;
    Channel = Channel & 0x03FF;
    RDA5807M_WriteReg[1] = Channel*64 + 0x10;  // Channel + TUNE-Bit + Band=00(87-108) + Space=00(100kHz)
    RDA5807M_WriteAll();
	
	
	  memcpy(msg_RDA_pp_com.RDA_REG_WRITE,RDA5807M_WriteReg,sizeof(RDA5807M_WriteReg));	
		osMessageQueuePut(mid_MsgQueue_RDA_pp_com, &msg_RDA_pp_com, NULL, 0U);
	
	
    RDA5807M_WriteReg[1] = RDA5807M_WriteReg[1] & RDA_TUNE_OFF;
		osDelay(200);
}

void RDA5807M_PowerOff(void){
    RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] ^ RDA_POWER;
    RDA5807M_WriteAll();
	
		memcpy(msg_RDA_pp_com.RDA_REG_WRITE,RDA5807M_WriteReg,sizeof(RDA5807M_WriteReg));	
		osMessageQueuePut(mid_MsgQueue_RDA_pp_com, &msg_RDA_pp_com, NULL, 0U);
	
		power=0;
}

void RDA5807M_Readregisters(void){
   int i;
   uint8_t rcv[12];    
   I2Crda->MasterReceive(0x10, rcv,12,false);   // read 12 bytes for reg 0x0A .. reg 0x0F
	 osThreadFlagsWait(FLAG_RDA,osFlagsWaitAny,osWaitForever); //Esperamos al callback para que no este todo el rato consumiendo
	
        for(i=0; i<6; i++){
            RDA5807M_ReadReg[i] = ((rcv[i *2] << 8) | rcv [(i*2) +1] );
        }
   

    freq = (((RDA5807M_ReadReg[0] & 0x03FF) * 0.1) + 87);    //return freq ex 102600KHz > 102.6MHz
				
		msg_RDA_pp.freq_ultima=freq;
		osMessageQueuePut(mid_MsgQueue_RDA_pp, &msg_RDA_pp, NULL, 0U);
}


void callback_rda (uint32_t event) //Equivalente a una rutina de interrupcion
	{
		uint32_t mask;
		mask = ARM_I2C_EVENT_TRANSFER_DONE;
		if (event & mask){
			osThreadFlagsSet(tid_Thread_MsgQueue_RDA,FLAG_RDA);
		}	
}

void RDA5807M_BassBoost(void){
    if ((RDA5807M_WriteReg[0] & 0x1000)==0){
        RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] | RDA_BASS_ON;
        bass=1;
    }
    else{
        RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] & RDA_BASS_OFF;
        bass=0;
    }
    RDA5807M_WriteAll();
}

void RDA5807M_RDS(){
    if ((RDA5807M_WriteReg[0] & RDA_RDS_ON)==0){
        RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] | RDA_RDS_ON;
        rds=1;
    }
    else{
        RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] & RDA_RDS_OFF;
        rds=0;
    }
    RDA5807M_WriteAll();
}
