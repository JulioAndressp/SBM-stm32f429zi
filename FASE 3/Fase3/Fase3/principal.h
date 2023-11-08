#ifndef __PRINCIPAL_H
#define __PRINCIPAL_H

void Thread_Principal (void *argument);
int Init_Thread_Principal	(void);

#define REPOSO 0
#define MANUAL 1
#define MEMORIA 2

#define GESTION_PULSADOR 0x203040
#define SIGNAL_ZUMBADOR 0x007

#endif
