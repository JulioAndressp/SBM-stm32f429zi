#include "cmsis_os2.h"                          // CMSIS RTOS header file

#include "joystick.h"
#include "clock.h"
#include "rda5807m.h"
#include "lcd.h"
#include "temperatura.h"
#include "principal.h"
#include "pwm.h"
#include "com.h"

extern int horas,minutos,segundos;

extern MSGQUEUE_OBJ_t_LCD msg_lcd;
extern MSGQUEUE_OBJ_temp msg_temp;
extern MSGQUEUE_OBJ_t_joy msg_joystick;
extern MSGQUEUE_OBJ_t_RDA msg_RDA;
extern MSGQUEUE_OBJ_t_COM msg_com;

extern osMessageQueueId_t mid_MsgQueue_temp;
extern osMessageQueueId_t mid_LCD_Queue; 
extern osMessageQueueId_t mid_PulsadorQueue; 
extern osMessageQueueId_t mid_MsgQueue_RDA;
extern osMessageQueueId_t mid_COM_Queue;

extern osThreadId_t tid_ThreadLCD_MsgQueue1;     
extern osThreadId_t tid_Thread_MsgQueue_RDA;
extern osThreadId_t tid_Thread_Zumbador;




/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thread_Principal;                        // thread id
 
 
int MODO = REPOSO;

int Init_Thread_Principal (void) {
 
  tid_Thread_Principal = osThreadNew(Thread_Principal, NULL, NULL);

  return(0);
}
 
extern osTimerId_t tim_id2_rebotes;

osStatus_t status_joy;
osStatus_t status_rda;


void Thread_Principal (void *argument) {
 
		Init_Timers_Joystick();
		Init_Timers_Clock();
	   
		
  while (1) {
		
		if(MODO == REPOSO) {
			
			//Recogida de la temperatura constante
			osMessageQueueGet(mid_MsgQueue_temp,&msg_temp, NULL, 1000U);
				
			//Mensajes al LCD
			sprintf(msg_lcd.frase1,"      %0.2d:%0.2d:%0.2d",horas, minutos,segundos);
			msg_lcd.line1=1;
			sprintf(msg_lcd.frase0, "  SBM 2022  T:%0.1lf ",msg_temp.temp);
			msg_lcd.line0=0;
			osMessageQueuePut(mid_LCD_Queue,&msg_lcd,0U,0U);		
			osThreadFlagsSet(tid_ThreadLCD_MsgQueue1,GESTION_LCD);
			
			//sprintf(msg_com.mensaje, "\n\rEsto workea");
			//osMessageQueuePut(mid_COM_Queue,&msg_com,0U,0U);		
			//osThreadFlagsSet(tid_ThreadCOM_MsgQueue,GESTION_COM);
			
			
			
			//Comprobacion de la pulsacion
			status_joy = osMessageQueueGet(mid_PulsadorQueue,&msg_joystick,NULL,50U);
		
			if (status_joy == osOK){
				
				osThreadFlagsSet(tid_Thread_Zumbador,PWM_SIGNAL);
				
				if(msg_joystick.Idx == IDENTIFICADOR_CENTRO_LARGA){  //Si el status esta okey y listo para recibir


					msg_RDA.Id_RDA = RDA_POWER;
					osMessageQueuePut(mid_MsgQueue_RDA, &msg_RDA, NULL, osWaitForever);
					osThreadFlagsSet(tid_Thread_MsgQueue_RDA,FLAG_MAIN_RDA);
								
					MODO = MANUAL;
		
				}
			}
		}//FIN MODO REPOSO
		
		if (MODO == MANUAL) {
			
			status_joy = osMessageQueueGet(mid_PulsadorQueue,&msg_joystick,NULL,50U);
			
			if(status_joy == osOK){ 
			
				osThreadFlagsSet(tid_Thread_Zumbador,PWM_SIGNAL);
				
				if(msg_joystick.Idx == IDENTIFICADOR_CENTRO_LARGA){
		
				MODO = MEMORIA;

				}
								
				if(msg_joystick.Idx == IDENTIFICADOR_ABAJO){

					msg_RDA.Id_RDA = RDA_SEEK_DOWN;
					osMessageQueuePut(mid_MsgQueue_RDA, &msg_RDA, NULL, osWaitForever);
					osThreadFlagsSet(tid_Thread_MsgQueue_RDA,FLAG_MAIN_RDA);

				}
								
				if(msg_joystick.Idx == IDENTIFICADOR_ARRIBA){
		

					msg_RDA.Id_RDA = RDA_SEEK_UP;
					osMessageQueuePut(mid_MsgQueue_RDA, &msg_RDA, NULL, osWaitForever);
					osThreadFlagsSet(tid_Thread_MsgQueue_RDA,FLAG_MAIN_RDA);

				}
								
				if(msg_joystick.Idx == IDENTIFICADOR_IZQUIERDA){
					
					status_rda = osMessageQueueGet(mid_MsgQueue_RDA,&msg_RDA,NULL,0U);
					
					if (status_rda == osOK){
						msg_RDA.Id_RDA = FRECUENCIA_UPDATE;
						msg_RDA.freq = msg_RDA.freq + 0.1;
						osMessageQueuePut(mid_MsgQueue_RDA ,&msg_RDA, 0U, 0U);		
						osThreadFlagsSet(tid_Thread_MsgQueue_RDA,FLAG_MAIN_RDA);						
					}

				}
				
				if(msg_joystick.Idx == IDENTIFICADOR_DERECHA){
					
					status_rda = osMessageQueueGet(mid_MsgQueue_RDA,&msg_RDA,NULL,50U);
					
					if (status_rda == osOK){
						msg_RDA.Id_RDA = FRECUENCIA_UPDATE;
						msg_RDA.freq = msg_RDA.freq - 0.1;
						osMessageQueuePut(mid_MsgQueue_RDA ,&msg_RDA, 0U, 0U);
						osThreadFlagsSet(tid_Thread_MsgQueue_RDA,FLAG_MAIN_RDA);
					}

				}
		
			}

		}//FIN MANUAL
		
		if(MODO == MEMORIA){
	
		sprintf(msg_lcd.frase0,"Modo memoria     ");
		msg_lcd.line0=0;
		sprintf(msg_lcd.frase1,"Esto no workea bien");
		msg_lcd.line1=1;

		osMessageQueuePut(mid_LCD_Queue,&msg_lcd,0U,0U);
		osThreadFlagsSet(tid_ThreadLCD_MsgQueue1,GESTION_LCD);
			
			}
			
		osThreadYield(); 
		}
		
	}


//		sprintf(msg_LCD.frase, "SBM 2022   T:%lf ",temperatura);
//		msg_LCD.line=0;
//		osMessageQueuePut(mid_LCD_Queue,&msg_LCD,0U,0U);
//		
//		osDelay(30);
//		sprintf(msg_LCD.frase,"      %0.2d:%0.2d:%0.2d",horas, minutos,segundos);
//		msg_LCD.line=1;
//		osMessageQueuePut(mid_LCD_Queue,&msg_LCD,0U,0U);

