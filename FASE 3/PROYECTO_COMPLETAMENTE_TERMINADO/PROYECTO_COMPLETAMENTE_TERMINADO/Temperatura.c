#include "cmsis_os2.h"
#include "temperatura.h"   

extern ARM_DRIVER_I2C Driver_I2C1;
static ARM_DRIVER_I2C *I2Ctemp = &Driver_I2C1;
	
float temperatura;

uint8_t 	cmd[2];
uint8_t   temp[2];
uint16_t  temperatura_bytes;
	 
/*----------------------------------------------------------------------------
 *      Message Queue creation & usage
 *---------------------------------------------------------------------------*/
 
osMessageQueueId_t mid_MsgQueue_temp;                // message queue id
 
osThreadId_t tid_Thread_MsgQueue_Temp;              // thread id 2
 
MSGQUEUE_OBJ_temp msg_temp;	//Lo unico que tiene es un float con temperatura


 
int Init_MsgQueue_Temperatura (void) {
 
  mid_MsgQueue_temp = osMessageQueueNew(MSGQUEUE_OBJECTS_temp, sizeof(MSGQUEUE_OBJ_temp), NULL);

  tid_Thread_MsgQueue_Temp = osThreadNew(Thread_MsgQueue_Temp, NULL, NULL);

 
  return(0);
}
 
void Thread_MsgQueue_Temp (void *argument) {
	
	/*--------INICIALIZACION DE LA TEMPERATURA*/
	
  I2C_Temp_Init();
	
	cmd[0]=REG_CONF;
	cmd[1]=0x00;
	
	I2Ctemp->MasterTransmit(LM75B,cmd,2,false);
	
	while(I2Ctemp->GetStatus().busy){};
	cmd[0]=REG_TEM;
	
	while(1) {
		
		osDelay(1000);
		
		//MEDICION DE LA TEMPERATURA
		I2Ctemp -> MasterTransmit(LM75B,cmd,2,true); //Inicializamos la comunicacion
		osThreadFlagsWait(FLAG_TEMP,osFlagsWaitAny,osWaitForever); //Esperamos al callback para que no este todo el rato consumiendo
		
		I2Ctemp -> MasterReceive(LM75B,temp,2,false);
		osThreadFlagsWait(FLAG_TEMP,osFlagsWaitAny,osWaitForever);
		
		temperatura_bytes = ((temp[0]<<8|temp[1])>>5);
		
		if(temperatura_bytes>>10==1)
		{
			temperatura = !(temperatura_bytes+1)*0.125;//Negativa msg_temp.temp
			msg_temp.temp = temperatura;
			osMessageQueuePut(mid_MsgQueue_temp, &msg_temp, 0U, 0U);//Pasamos el mensaje por la cola de temp
			
		}	else 
		{
			temperatura= temperatura_bytes*0.125;//Positiva msg_temp.temp
			msg_temp.temp = temperatura;
			osMessageQueuePut(mid_MsgQueue_temp, &msg_temp, 0U, 0U);//Pasamos el mensaje por la cola de temp
		}
			osThreadYield(); 
	}
}

//Esto lo hacemos para que el programa no este consumiendo CPU constantemente haciendo cosas para no pararse.
//El SPI si que hace eso y por eso tarda tanto en escribir en el LCD y por lo tanto, necesitamos al inicializarlo
//un callback en la linea I2Ctemp -> Initialize(callback_temp)

void callback_temp (uint32_t event) //Equivalente a una rutina de interrupcion
	{
		uint32_t mask;
		mask = ARM_I2C_EVENT_TRANSFER_DONE;
		if (event & mask){
			osThreadFlagsSet(tid_Thread_MsgQueue_Temp,FLAG_TEMP);
		}	
}
	
void I2C_Temp_Init(void)
{
	  I2Ctemp-> Initialize(callback_temp);
	  I2Ctemp-> PowerControl(ARM_POWER_FULL);
		I2Ctemp-> Control (ARM_I2C_BUS_SPEED,ARM_I2C_BUS_SPEED_FAST);
		I2Ctemp-> Control (ARM_I2C_BUS_CLEAR,0);
}
