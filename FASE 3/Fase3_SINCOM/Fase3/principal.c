#include "cmsis_os2.h"                          // CMSIS RTOS header file

#include "joystick.h"
#include "clock.h"
#include "rda5807m.h"
#include "lcd.h"
#include "temperatura.h"
#include "principal.h"
#include "pwm.h"
#include "vol.h"
#include "com.h"

extern int horas,minutos,segundos;

extern osMessageQueueId_t mid_Vol_Queue;
extern osMessageQueueId_t mid_MsgQueue_temp;
extern osMessageQueueId_t mid_LCD_Queue; 
extern osMessageQueueId_t mid_PulsadorQueue; 
extern osMessageQueueId_t mid_MsgQueue_RDA;
extern osMessageQueueId_t mid_MsgQueue_RDA_pp;    

extern osThreadId_t tid_ThreadLCD_MsgQueue1;     
extern osThreadId_t tid_Thread_MsgQueue_RDA;
extern osThreadId_t tid_Thread_Zumbador;

extern osTimerId_t tim_id2_rebotes;

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thread_Principal;
 
 
int MODO = SELECTOR;

int ESTADO_PROG_HORA = MOD_HORAS;
int MODO_SELECTOR = SELECTOR_REPOSO;

int horas_aux,minutos_aux,segundos_aux;
bool boolean_aux = false;

int Init_Thread_Principal (void) {
 
  tid_Thread_Principal = osThreadNew(Thread_Principal, NULL, NULL);

  return(0);
}
 




void Thread_Principal (void *argument) {
	
	osStatus_t status_vol;
	osStatus_t status_joy;
	osStatus_t status_rda;
	osStatus_t status_temp;
	
	MSGQUEUE_OBJ_t_LCD msg_lcd;
	MSGQUEUE_OBJ_temp msg_temp;
	MSGQUEUE_OBJ_t_joy msg_joystick;
	MSGQUEUE_OBJ_t_RDA msg_RDA;		//Cola que va del RDA -> PRINCIPAL
	MSGQUEUE_OBJ_t_RDA_pp msg_RDA_pp;//Cola que va de PRINCIPAL -> RDA
	MSGQUEUE_OBJ_t_Vol msg_vol;
	
	float array_freq[16]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
	int pos_radio=0;
	float frecuencia_actual;
	int volumen_actual;

	int j=0;
	Init_Timers_Joystick();
	Init_Timers_Clock();
		
		while (1) {
			
			if(MODO == SELECTOR){
				
				//Recoger el volumen constantemente
				status_vol = osMessageQueueGet(mid_Vol_Queue,&msg_vol,NULL,50U);
						
				if (status_vol == osOK) {
								
							volumen_actual = msg_vol.vol;
							msg_RDA.vol=volumen_actual;
							msg_RDA.Id_RDA=VOLUMEN_UPDATE;
							osMessageQueuePut(mid_MsgQueue_RDA, &msg_RDA, NULL, 0U);
				}
			
				if(MODO_SELECTOR == SELECTOR_REPOSO)	{

				status_temp = osMessageQueueGet(mid_MsgQueue_temp,&msg_temp, NULL, 10U);
				
				if (status_temp == osOK) {
					
					//Mensajes al LCD
					sprintf(msg_lcd.frase0,"HORA : %0.2d:%0.2d:%0.2d       ",horas,minutos,segundos);
					msg_lcd.line0=0;
					sprintf(msg_lcd.frase1, "Accede REPOSO          ");
					msg_lcd.line1=1;
					osMessageQueuePut(mid_LCD_Queue,&msg_lcd,0U,0U);		
					osThreadFlagsSet(tid_ThreadLCD_MsgQueue1,GESTION_LCD);				

				}
					//Recogemos la pulsacion
					status_joy = osMessageQueueGet(mid_PulsadorQueue,&msg_joystick,NULL,50U);
				
					if(status_joy == osOK) { 
				
						osThreadFlagsSet(tid_Thread_Zumbador,PWM_SIGNAL);
					
						if(msg_joystick.Idx == IDENTIFICADOR_CENTRO_LARGA) {
							msg_RDA.Id_RDA = RDA_POWER_OFF;
							osMessageQueuePut(mid_MsgQueue_RDA, &msg_RDA, NULL, 0U);
							MODO = REPOSO;

						}
																								
						if(msg_joystick.Idx == IDENTIFICADOR_IZQUIERDA) {
									
							MODO_SELECTOR = SELECTOR_HORA;

						}
					
						if(msg_joystick.Idx == IDENTIFICADOR_DERECHA){
								
							MODO_SELECTOR = SELECTOR_MANUAL;
						}
						
					}
				
				}
				
				
				
				if(MODO_SELECTOR == SELECTOR_MANUAL)	{

					status_temp = osMessageQueueGet(mid_MsgQueue_temp,&msg_temp, NULL, 10U);
				
					if (status_temp == osOK) {
					
						//Mensajes al LCD
						sprintf(msg_lcd.frase0,"HORA : %0.2d:%0.2d:%0.2d       ",horas,minutos,segundos);
						msg_lcd.line0=0;
						sprintf(msg_lcd.frase1, "Accede MANUAL           ");
						msg_lcd.line1=1;
						osMessageQueuePut(mid_LCD_Queue,&msg_lcd,0U,0U);		
						osThreadFlagsSet(tid_ThreadLCD_MsgQueue1,GESTION_LCD);				

					}
					//Recogemos la pulsacion
					status_joy = osMessageQueueGet(mid_PulsadorQueue,&msg_joystick,NULL,50U);
				
					if(status_joy == osOK) { 
				
						osThreadFlagsSet(tid_Thread_Zumbador,PWM_SIGNAL);
					
						if(msg_joystick.Idx == IDENTIFICADOR_CENTRO_LARGA) {
							msg_RDA.Id_RDA = RDA_POWER;
							osMessageQueuePut(mid_MsgQueue_RDA, &msg_RDA, NULL,0U);
						
							status_rda = osMessageQueueGet(mid_MsgQueue_RDA_pp,&msg_RDA_pp,NULL,osWaitForever);
						
							if (status_rda == osOK) {
								
								frecuencia_actual = msg_RDA_pp.freq_ultima;
							}
							
							MODO = MANUAL;

						}
																								
						if(msg_joystick.Idx == IDENTIFICADOR_IZQUIERDA) {
							
							MODO_SELECTOR = SELECTOR_REPOSO;

						}
					
						if(msg_joystick.Idx == IDENTIFICADOR_DERECHA){
								
							MODO_SELECTOR = SELECTOR_MEMORIA;
						}
					
					}
				
				}
				
				
			
				if(MODO_SELECTOR == SELECTOR_MEMORIA)	{

				status_temp = osMessageQueueGet(mid_MsgQueue_temp,&msg_temp, NULL, 10U);
				
				if (status_temp == osOK) {
					
					//Mensajes al LCD
					sprintf(msg_lcd.frase0,"HORA : %0.2d:%0.2d:%0.2d       ",horas,minutos,segundos);
					msg_lcd.line0=0;
					sprintf(msg_lcd.frase1, "Accede MEMORIA            ");
					msg_lcd.line1=1;
					osMessageQueuePut(mid_LCD_Queue,&msg_lcd,0U,0U);		
					osThreadFlagsSet(tid_ThreadLCD_MsgQueue1,GESTION_LCD);				

				}
					//Recogemos la pulsacion
				status_joy = osMessageQueueGet(mid_PulsadorQueue,&msg_joystick,NULL,50U);
				
				if(status_joy == osOK) { 
				
					osThreadFlagsSet(tid_Thread_Zumbador,PWM_SIGNAL);
					
					if(msg_joystick.Idx == IDENTIFICADOR_CENTRO_LARGA) {
			
						MODO = MEMORIA;

					}
																								
					if(msg_joystick.Idx == IDENTIFICADOR_IZQUIERDA) {
							
						MODO_SELECTOR = SELECTOR_MANUAL;

					}
					
					if(msg_joystick.Idx == IDENTIFICADOR_DERECHA){
								
						MODO_SELECTOR = SELECTOR_HORA;
					}
					
				}
				
			}
				
			
			if(MODO_SELECTOR == SELECTOR_HORA)	{

				status_temp = osMessageQueueGet(mid_MsgQueue_temp,&msg_temp, NULL, 10U);
				
				if (status_temp == osOK) {
					
					//Mensajes al LCD
					sprintf(msg_lcd.frase0,"HORA : %0.2d:%0.2d:%0.2d       ",horas,minutos,segundos);
					msg_lcd.line0=0;
					sprintf(msg_lcd.frase1, "Accede HORA MOD            ");
					msg_lcd.line1=1;
					osMessageQueuePut(mid_LCD_Queue,&msg_lcd,0U,0U);		
					osThreadFlagsSet(tid_ThreadLCD_MsgQueue1,GESTION_LCD);				

				}
					//Recogemos la pulsacion
				status_joy = osMessageQueueGet(mid_PulsadorQueue,&msg_joystick,NULL,50U);
				
				if(status_joy == osOK) { 
				
					osThreadFlagsSet(tid_Thread_Zumbador,PWM_SIGNAL);
					
					if(msg_joystick.Idx == IDENTIFICADOR_CENTRO_LARGA) {
			
						MODO = PROG_HORA;

					}
																								
					if(msg_joystick.Idx == IDENTIFICADOR_IZQUIERDA) {
							
						MODO_SELECTOR = SELECTOR_MANUAL;

					}
					
					if(msg_joystick.Idx == IDENTIFICADOR_DERECHA){
								
						MODO_SELECTOR = SELECTOR_REPOSO;
					}
					
				}
				
			}
				

				
			}
			if(MODO == REPOSO)	{
				
				//Recogida de la temperatura cada segundo
				
				status_temp = osMessageQueueGet(mid_MsgQueue_temp,&msg_temp, NULL, 100U);
				
				if (status_temp == osOK) {
					
				//Mensajes al LCD cada segundo de la temperatura
					
					sprintf(msg_lcd.frase1,"      %0.2d:%0.2d:%0.2d        ",horas, minutos,segundos);
					msg_lcd.line1=1;
					sprintf(msg_lcd.frase0, "  SBM 2022  T:%0.1lf        ",msg_temp.temp);
					msg_lcd.line0=0;
					
					osMessageQueuePut(mid_LCD_Queue,&msg_lcd,0U,0U);		
					osThreadFlagsSet(tid_ThreadLCD_MsgQueue1,GESTION_LCD);				

				}

				//Comprobacion de la pulsacion
				
				status_joy = osMessageQueueGet(mid_PulsadorQueue,&msg_joystick,NULL,50U);
			
				if (status_joy == osOK){
					
					osThreadFlagsSet(tid_Thread_Zumbador,PWM_SIGNAL);
					
					if(msg_joystick.Idx == IDENTIFICADOR_CENTRO_LARGA){  //Si el status esta okey y listo para recibir
						MODO = SELECTOR;

					}
				}
			}//FIN MODO REPOSO
			
			if (MODO == MANUAL) {
			
					//Medicion de la frecuencia
				status_rda = osMessageQueueGet(mid_MsgQueue_RDA_pp,&msg_RDA_pp,NULL,75U);
						
				if (status_rda == osOK) {
								
					frecuencia_actual = msg_RDA_pp.freq_ultima;
								
				}
				
					//Recoger el volumen constantemente
				status_vol = osMessageQueueGet(mid_Vol_Queue,&msg_vol,NULL,50U);
						
				if (status_vol == osOK) {
								
							volumen_actual = msg_vol.vol;
							msg_RDA.vol=volumen_actual;
							msg_RDA.Id_RDA=VOLUMEN_UPDATE;
							osMessageQueuePut(mid_MsgQueue_RDA, &msg_RDA, NULL, 0U);
				}
				
				//Recogemos y pintamos
				status_temp = osMessageQueueGet(mid_MsgQueue_temp,&msg_temp, NULL, 10U);
				
				if (status_temp == osOK) {
					
				//Mensajes al LCD
					sprintf(msg_lcd.frase0,"  %0.2d:%0.2d:%0.2d   T:%0.1lf ",horas, minutos,segundos,msg_temp.temp);
					msg_lcd.line1=1;
					sprintf(msg_lcd.frase1, "  FM:%0.1lf    VOL:%d ",frecuencia_actual,msg_vol.vol);
					msg_lcd.line0=0;
					
					osMessageQueuePut(mid_LCD_Queue,&msg_lcd,0U,0U);		
					osThreadFlagsSet(tid_ThreadLCD_MsgQueue1,GESTION_LCD);				

				}
				
				//Recogemos la pulsacion
				status_joy = osMessageQueueGet(mid_PulsadorQueue,&msg_joystick,NULL,50U);
				
				if(status_joy == osOK) { 
				
					osThreadFlagsSet(tid_Thread_Zumbador,PWM_SIGNAL);
					
					if(msg_joystick.Idx == IDENTIFICADOR_CENTRO_LARGA) {
			
						MODO = SELECTOR;

					}
									
					if(msg_joystick.Idx == IDENTIFICADOR_ABAJO) {

						msg_RDA.Id_RDA = RDA_SEEK_DOWN;
						osMessageQueuePut(mid_MsgQueue_RDA, &msg_RDA, NULL, 0U);

					}
									
					if(msg_joystick.Idx == IDENTIFICADOR_ARRIBA) {
			
						msg_RDA.Id_RDA = RDA_SEEK_UP;
						osMessageQueuePut(mid_MsgQueue_RDA, &msg_RDA, NULL, 0U);

					}
									
					if(msg_joystick.Idx == IDENTIFICADOR_IZQUIERDA) {
							
						msg_RDA.Id_RDA = FRECUENCIA_UPDATE;
						msg_RDA.freq = (frecuencia_actual - 0.1);
						osMessageQueuePut(mid_MsgQueue_RDA ,&msg_RDA, NULL, 0U);		

					}
					
					if(msg_joystick.Idx == IDENTIFICADOR_DERECHA){
								
						msg_RDA.Id_RDA = FRECUENCIA_UPDATE;
						msg_RDA.freq = (frecuencia_actual + 0.1);
						osMessageQueuePut(mid_MsgQueue_RDA ,&msg_RDA, NULL, 0U);						

					}
					
				}

			}//FIN MANUAL
			
			if(MODO == MEMORIA)	{
				
				//Medicion de la frecuencia
				status_rda = osMessageQueueGet(mid_MsgQueue_RDA_pp,&msg_RDA_pp,NULL,50U);
						
				if (status_rda == osOK) {
								
					frecuencia_actual = msg_RDA_pp.freq_ultima;
								
				}
						
					//Recoger el volumen constantemente
				status_vol = osMessageQueueGet(mid_Vol_Queue,&msg_vol,NULL,50U);
						
					if (status_vol == osOK) {
								
						volumen_actual = msg_vol.vol;
						msg_RDA.vol=volumen_actual;
						msg_RDA.Id_RDA=VOLUMEN_UPDATE;
						osMessageQueuePut(mid_MsgQueue_RDA, &msg_RDA, NULL, 0U);
							
					}
						
				//Recogemos y pintamos
				status_temp = osMessageQueueGet(mid_MsgQueue_temp,&msg_temp, NULL, 10U);
				
				if (status_temp == osOK) {
					
				//Mensajes al LCD
					sprintf(msg_lcd.frase0,"  %0.2d:%0.2d:%0.2d   T:%0.1lf ",horas, minutos,segundos,msg_temp.temp);
					msg_lcd.line0=0;
					sprintf(msg_lcd.frase1, " FM:%0.1lf VOL:%d N:%d  ",frecuencia_actual,msg_vol.vol,pos_radio);
					msg_lcd.line1=1;
					
					osMessageQueuePut(mid_LCD_Queue,&msg_lcd,0U,0U);		
					osThreadFlagsSet(tid_ThreadLCD_MsgQueue1,GESTION_LCD);				

				}
		
					//Recogemos la pulsacion
				status_joy = osMessageQueueGet(mid_PulsadorQueue,&msg_joystick,NULL,50U);
				
				if(status_joy == osOK) { 
				
					osThreadFlagsSet(tid_Thread_Zumbador,PWM_SIGNAL);
					
					if(msg_joystick.Idx == IDENTIFICADOR_CENTRO_LARGA)	{
			
						MODO = SELECTOR;
						minutos_aux = minutos;
						horas_aux = horas;

					}
									
					if(msg_joystick.Idx == IDENTIFICADOR_ABAJO)	{

						for(j=0;j<16;j++){
							if(array_freq[j] == 0){
								array_freq[j] = frecuencia_actual;
								pos_radio = j;
								j=16;
							}
						}
					}
							
					
					if(msg_joystick.Idx == IDENTIFICADOR_ARRIBA)	{
			
						msg_RDA.Id_RDA = RDA_SEEK_UP;
						osMessageQueuePut(mid_MsgQueue_RDA, &msg_RDA, NULL, 0U);

					}
									
					if(msg_joystick.Idx == IDENTIFICADOR_IZQUIERDA) {
											

						if(pos_radio - 1 == -1 && array_freq[15] != NULL)	{
							
							pos_radio =15;				
							frecuencia_actual = array_freq[pos_radio];
							msg_RDA.freq = frecuencia_actual;
							msg_RDA.Id_RDA = FRECUENCIA_UPDATE;
							osMessageQueuePut(mid_MsgQueue_RDA, &msg_RDA, NULL, 0U);
									
						}else	{
									
							if(array_freq[pos_radio - 1]  != NULL && pos_radio > 0){	
								
								pos_radio--;			
								frecuencia_actual = array_freq[pos_radio];
								msg_RDA.freq = frecuencia_actual;
								msg_RDA.Id_RDA = FRECUENCIA_UPDATE;
								osMessageQueuePut(mid_MsgQueue_RDA, &msg_RDA, NULL, 0U);
								
							}
						}
					}
			
					
					
					if(msg_joystick.Idx == IDENTIFICADOR_DERECHA){
								
						if(pos_radio + 1 == 16 && array_freq[0] != NULL){
							
							pos_radio = 0;
							frecuencia_actual = array_freq[pos_radio];
							msg_RDA.freq = frecuencia_actual;
							msg_RDA.Id_RDA = FRECUENCIA_UPDATE;
							osMessageQueuePut(mid_MsgQueue_RDA, &msg_RDA, NULL, 0U);
								
						} else	{

							if(array_freq[pos_radio + 1]  != NULL && pos_radio < 17){
							
								pos_radio++;	
								frecuencia_actual = array_freq[pos_radio];
								msg_RDA.freq = frecuencia_actual;
								msg_RDA.Id_RDA = FRECUENCIA_UPDATE;
								osMessageQueuePut(mid_MsgQueue_RDA, &msg_RDA, NULL, 0U);
								
							}							
						}
					}
				}	
			}//FIN MODO MEMORIA
			
			if(MODO == PROG_HORA)	{
				
				
				//Recoger el volumen constantemente
				status_vol = osMessageQueueGet(mid_Vol_Queue,&msg_vol,NULL,50U);
						
				if (status_vol == osOK) {
								
					volumen_actual = msg_vol.vol;
					msg_RDA.vol=volumen_actual;
					msg_RDA.Id_RDA=VOLUMEN_UPDATE;
					osMessageQueuePut(mid_MsgQueue_RDA, &msg_RDA, NULL, 0U);
							
				}
				
				//Recogemos la pulsacion
				if(ESTADO_PROG_HORA == MOD_HORAS){
				
					status_joy = osMessageQueueGet(mid_PulsadorQueue,&msg_joystick,NULL,50U);
				
					if(status_joy == osOK) { 
				
						osThreadFlagsSet(tid_Thread_Zumbador,PWM_SIGNAL);
					
						if(msg_joystick.Idx == IDENTIFICADOR_CENTRO_LARGA) {
								
							sprintf(msg_lcd.frase0,"HORA-NEW : %0.2d:%0.2d:%0.2d  ",horas_aux,minutos_aux,segundos_aux);
							msg_lcd.line0=0;
							sprintf(msg_lcd.frase1, "ACTUALIZANDO...         ");
							msg_lcd.line1=1;
							osMessageQueuePut(mid_LCD_Queue,&msg_lcd,0U,0U);		
							osThreadFlagsSet(tid_ThreadLCD_MsgQueue1,GESTION_LCD);	

							osDelay(4000);
							
							minutos = minutos_aux;
							horas = horas_aux;
							segundos = segundos_aux;
								
							boolean_aux = true;

						}
									
						if(msg_joystick.Idx == IDENTIFICADOR_ABAJO) {

							horas_aux--;
								
							if(horas_aux < 0){
								horas_aux = 23;
							}
						}
									
						if(msg_joystick.Idx == IDENTIFICADOR_ARRIBA) {
							horas_aux++;
								
							if(horas_aux > 23){
								horas_aux = 0;
							}

						}
									
						if(msg_joystick.Idx == IDENTIFICADOR_IZQUIERDA) {
											
							ESTADO_PROG_HORA = MOD_ELIM;
							
						}

						if(msg_joystick.Idx == IDENTIFICADOR_DERECHA){
								
							ESTADO_PROG_HORA = MOD_MIN;
						
						}
				}
					
				status_temp = osMessageQueueGet(mid_MsgQueue_temp,&msg_temp, NULL, 10U);
				
				if (status_temp == osOK) {
					
				//Mensajes al LCD
					sprintf(msg_lcd.frase0,"Hora actual: %0.2d:%0.2d:%0.2d  ",horas,minutos,segundos);
					msg_lcd.line0=0;
					sprintf(msg_lcd.frase1, "Modificando horas : %0.2d   ",horas_aux);
					msg_lcd.line1=1;
					osMessageQueuePut(mid_LCD_Queue,&msg_lcd,0U,0U);		
					osThreadFlagsSet(tid_ThreadLCD_MsgQueue1,GESTION_LCD);				

				}
					
			}
				
			if(ESTADO_PROG_HORA == MOD_MIN){
				
				status_joy = osMessageQueueGet(mid_PulsadorQueue,&msg_joystick,NULL,50U);
				
				if(status_joy == osOK) { 
			
					osThreadFlagsSet(tid_Thread_Zumbador,PWM_SIGNAL);
					
					if(msg_joystick.Idx == IDENTIFICADOR_CENTRO_LARGA) {
	
						sprintf(msg_lcd.frase0,"HORA-NEW : %0.2d:%0.2d:%0.2d  ",horas_aux,minutos_aux,segundos_aux);
						msg_lcd.line0=0;
						sprintf(msg_lcd.frase1, "ACTUALIZANDO...         ");
						msg_lcd.line1=1;
						osMessageQueuePut(mid_LCD_Queue,&msg_lcd,0U,0U);		
						osThreadFlagsSet(tid_ThreadLCD_MsgQueue1,GESTION_LCD);	

						osDelay(4000);
								
						minutos = minutos_aux;
						horas = horas_aux;
						segundos = segundos_aux;
							
						boolean_aux = true;
							
					}
									
					if(msg_joystick.Idx == IDENTIFICADOR_ABAJO) {

						minutos_aux--;
								
						if(minutos_aux < 0){
							minutos_aux = 59;
						}
								
					}
									
					if(msg_joystick.Idx == IDENTIFICADOR_ARRIBA) {
			
						minutos_aux++;
								
						if(minutos_aux > 59){
							minutos_aux = 00;
						}
								

					}
									
					if(msg_joystick.Idx == IDENTIFICADOR_IZQUIERDA) {
											
								ESTADO_PROG_HORA = MOD_HORAS;
					}
			
					
					
					if(msg_joystick.Idx == IDENTIFICADOR_DERECHA){
								
								ESTADO_PROG_HORA = MOD_SEG;
								
					}
					
				}
					
				status_temp = osMessageQueueGet(mid_MsgQueue_temp,&msg_temp, NULL, 10U);
				
				if (status_temp == osOK) {
					
				//Mensajes al LCD
					sprintf(msg_lcd.frase0,"Hora actual: %0.2d:%0.2d:%0.2d  ",horas,minutos,segundos);
					msg_lcd.line0=0;
					sprintf(msg_lcd.frase1, "Modificando min : %0.2d    ",minutos_aux);
					msg_lcd.line1=1;
					osMessageQueuePut(mid_LCD_Queue,&msg_lcd,0U,0U);		
					osThreadFlagsSet(tid_ThreadLCD_MsgQueue1,GESTION_LCD);				

				}
					
			}
				
				if(ESTADO_PROG_HORA == MOD_SEG){
				
					status_joy = osMessageQueueGet(mid_PulsadorQueue,&msg_joystick,NULL,50U);
				
					if(status_joy == osOK) { 
				
						osThreadFlagsSet(tid_Thread_Zumbador,PWM_SIGNAL);
					
						if(msg_joystick.Idx == IDENTIFICADOR_CENTRO_LARGA) {
		
							sprintf(msg_lcd.frase0,"HORA-NEW : %0.2d:%0.2d:%0.2d  ",horas_aux,minutos_aux,segundos_aux);
							msg_lcd.line0=0;
							sprintf(msg_lcd.frase1, "ACTUALIZANDO...         ");
							msg_lcd.line1=1;
							osMessageQueuePut(mid_LCD_Queue,&msg_lcd,0U,0U);		
							osThreadFlagsSet(tid_ThreadLCD_MsgQueue1,GESTION_LCD);	

							osDelay(4000);
							
							minutos = minutos_aux;
							horas = horas_aux;
							segundos = segundos_aux;
							
							boolean_aux = true;

						}
									
						if(msg_joystick.Idx == IDENTIFICADOR_ABAJO) {

							segundos_aux--;
								
							if(segundos_aux < 0){
								segundos_aux = 59;
							}
								
						}
									
						if(msg_joystick.Idx == IDENTIFICADOR_ARRIBA) {
			
							segundos_aux++;
								
							if(segundos_aux > 59){
								segundos_aux = 00;
							}

						}
									
						if(msg_joystick.Idx == IDENTIFICADOR_IZQUIERDA) {
											
							ESTADO_PROG_HORA = MOD_MIN;
								
						}
			
					
					
						if(msg_joystick.Idx == IDENTIFICADOR_DERECHA){
							
							ESTADO_PROG_HORA = MOD_ELIM;
								
						}
					}		
					
					status_temp = osMessageQueueGet(mid_MsgQueue_temp,&msg_temp, NULL, 10U);
				
					if (status_temp == osOK) {
					
						//Mensajes al LCD
					sprintf(msg_lcd.frase0,"Hora actual: %0.2d:%0.2d:%0.2d  ",horas,minutos,segundos);
						msg_lcd.line0=0;
						sprintf(msg_lcd.frase1, "Modificando seg : %0.2d    ",segundos_aux);
						msg_lcd.line1=1;
					
						osMessageQueuePut(mid_LCD_Queue,&msg_lcd,0U,0U);		
						osThreadFlagsSet(tid_ThreadLCD_MsgQueue1,GESTION_LCD);				

				}
					
			}
				
				if(ESTADO_PROG_HORA == MOD_ELIM){
				
					status_joy = osMessageQueueGet(mid_PulsadorQueue,&msg_joystick,NULL,50U);
				
					if(status_joy == osOK) { 
				
						osThreadFlagsSet(tid_Thread_Zumbador,PWM_SIGNAL);
					
						if(msg_joystick.Idx == IDENTIFICADOR_CENTRO_LARGA) {
							
							pos_radio=0;
							
							for(int i = 0; i<16 ; i++){
								array_freq[i] = NULL;
							}
							
						//Mensajes al LCD
							sprintf(msg_lcd.frase0,"MEMORIA BORRADA      ");
							msg_lcd.line0=0;
							sprintf(msg_lcd.frase1, "Volviendo al menu         ");
							msg_lcd.line1=1;
							osMessageQueuePut(mid_LCD_Queue,&msg_lcd,0U,0U);		
							osThreadFlagsSet(tid_ThreadLCD_MsgQueue1,GESTION_LCD);	
						

							
							osDelay(4000);
							
							sprintf(msg_lcd.frase0,"HORA-NEW : %0.2d:%0.2d:%0.2d  ",horas_aux,minutos_aux,segundos_aux);
							msg_lcd.line0=0;
							sprintf(msg_lcd.frase1, "ACTUALIZANDO...         ");
							msg_lcd.line1=1;
							osMessageQueuePut(mid_LCD_Queue,&msg_lcd,0U,0U);		
							osThreadFlagsSet(tid_ThreadLCD_MsgQueue1,GESTION_LCD);	

							osDelay(4000);
						
							minutos = minutos_aux;
							horas = horas_aux;
							segundos = segundos_aux;
							
							boolean_aux = true;
							
						}
									
						if(msg_joystick.Idx == IDENTIFICADOR_ABAJO) {


								
						}
									
						if(msg_joystick.Idx == IDENTIFICADOR_ARRIBA) {
			


						}
									
						if(msg_joystick.Idx == IDENTIFICADOR_IZQUIERDA) {
											
								ESTADO_PROG_HORA = MOD_SEG;
								
						}
			
					
					
						if(msg_joystick.Idx == IDENTIFICADOR_DERECHA){
								
								ESTADO_PROG_HORA = MOD_HORAS;
								
						}
					}
					
					status_temp = osMessageQueueGet(mid_MsgQueue_temp,&msg_temp, NULL, 10U);
				
					if (status_temp == osOK) {
					
					//Mensajes al LCD
						sprintf(msg_lcd.frase0,"Borrar Memoria         ");
						msg_lcd.line0=0;
						sprintf(msg_lcd.frase1, "Centro Larga OK          ");
						msg_lcd.line1=1;
						osMessageQueuePut(mid_LCD_Queue,&msg_lcd,0U,0U);		
						osThreadFlagsSet(tid_ThreadLCD_MsgQueue1,GESTION_LCD);				

					}
				
					
			}
				
			if (boolean_aux == true){
				boolean_aux = false;
				MODO = SELECTOR;
				ESTADO_PROG_HORA = MOD_HORAS;

			}
				
		}//FIN PROG_HORA	
			
	 osThreadYield(); 
	}//Fin WHILE(1)		
}//FIN HILO

