#include "Principal.h"
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
 osThreadId_t tid_Temp;                        // thread id

 
 static ARM_DRIVER_I2C *I2Ctemp = &Driver_I2C1;
 float temperatura = 00.0;

 
void ThtEmp (void *argument);                   // thread function
 
int Init_Temp (void) {
 
  tid_Temp = osThreadNew(ThtEmp, NULL, NULL);
  if (tid_Temp == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void ThtEmp (void *argument) {
	/*--------INICIALIZACION DE LA TEMPERATURA*/
  I2C_Temp_Init();
	
	uint16_t temperatura_bytes;
	uint8_t cmd[2];
  uint8_t temp[2];
	cmd[0]=REG_CONF;
	cmd[1]=0x00;
	I2Ctemp->MasterTransmit(LM75B_ADD,cmd,2,false);
	while(I2Ctemp->GetStatus().busy);
	cmd[0]=REG_TEM;
	while(1)
	{
		osThreadFlagsWait(0,osFlagsWaitAny,1000);
		//-------MEDICION TEMPERATURA--------//
		I2Ctemp -> MasterTransmit(LM75B_ADD,cmd,2,true);//Inicia la comunicacion
		while(I2Ctemp -> GetStatus().busy);
		
		I2Ctemp -> MasterReceive(LM75B_ADD,temp,2,false);
		while(I2Ctemp->GetStatus().busy);
		
		temperatura_bytes = ((temp[0]<<8|temp[1])>>5);
		
		if(temperatura_bytes>>10==1)
		{
			temperatura = !(temperatura_bytes+1)*0.125;//Negativa
		}else
		{
			temperatura = temperatura_bytes*0.125;//Positiva
		}
			osThreadYield(); 
	}
}

void I2C_Temp_Init(void)
{
	  I2Ctemp-> Initialize(NULL);
	  I2Ctemp-> PowerControl(ARM_POWER_FULL);
		I2Ctemp-> Control (ARM_I2C_BUS_SPEED,ARM_I2C_BUS_SPEED_FAST);
		I2Ctemp-> Control (ARM_I2C_BUS_CLEAR,0);
}
