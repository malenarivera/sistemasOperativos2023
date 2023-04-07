/* mut.c - mut, operar, incrementar */
#include <xinu.h>

void operar(void), incrementar(void);
unsigned char x = 0;
sid32 mutex;
int32 pidUltiLock;

void mutex_init(void){
 mutex= semcreate(1);
}

void mutex_lock(void){
	wait(mutex);
}

void mutex_unlock(void){
    /*verificando que el que hace unlock sea el mismo que tenia el lock*/
    int pidcito=getpid();
    if(pidUltiLock==pidcito){
	    signal(mutex);
    }
}


/*------------------------------------------------------------------------
* mut -- programa con regiones criticas
*------------------------------------------------------------------------
*/
void probandoMutex(void){
	int i; /*???? para que usa este i, wtf*/
	mutex_init();
	resume( create(operar, 1024, 20, "process 1", 0) );
	resume( create(incrementar, 1024, 20, "process 2", 0) );
	sleep(10);
}
/*------------------------------------------------------------------------
* operar x e y
*------------------------------------------------------------------------
*/
void operar(void){
int y = 0;
printf("Si no existen mensajes de ERROR entonces todo va OK! \n");
while (1) {
	mutex_lock();
	/* si x es multiplo de 10 */
	if ((x % 10) == 0) {
		y = x * 2; /* como y es el doble de x entonces
		* y es multiplo de 10 tambien*/
		/* si y no es multiplo de 10 entonces hubo un error */
		if ((y % 10) != 0)
			printf("\r ERROR!! y=%d, x=%d \r", y, x);
	}
	mutex_unlock();
}
}
/*------------------------------------------------------------------------
* incrementar x
*------------------------------------------------------------------------
*/
void incrementar(void){
	while (1) {
		mutex_lock();
		x = x + 1;
		mutex_unlock();
	}
}
