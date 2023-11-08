#ifndef __PRINCIPAL_H
#define __PRINCIPAL_H

void Thread_Principal (void *argument);
int Init_Thread_Principal	(void);

#define REPOSO 0
#define MANUAL 1
#define MEMORIA 2
#define PROG_HORA 3
#define SELECTOR 4

#define MOD_HORAS 5
#define MOD_MIN 6 
#define MOD_SEG 7
#define MOD_ELIM 8

#define SELECTOR_REPOSO 9
#define SELECTOR_MANUAL 10
#define SELECTOR_MEMORIA 11
#define SELECTOR_HORA 12

#define GESTION_PULSADOR 0x203040
#define SIGNAL_ZUMBADOR 0x007

#endif
