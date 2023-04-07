#include <xinu.h>
sid32 productor_sem, consumidor_sem;

void productor(void), consumidor(void);

int32 n=0; /*variable globar que es compartida por todos los procesos*/


/*main*/
/*Ejemplo de un productor consumidor sincronizado*/

void productorConsumidor(void){
	productor_sem= semcreate(1);
	consumidor_sem= semcreate(0);
	resume(create(consumidor,1024,20,"consumidor",0));
	resume(create(productor,1024,20,"productor",0));
	
}


/*ahora el productor*/
/*El productor incrementa n 2000 veces y se va*/

void productor(void){
	int32 i;
	for(i=1; i<=2000; i++){
		/*espera que el consumidor le diga cuando volver a producir */
		wait(productor_sem);
		n++;
		/*le avisa al consumidor que puede volver a consumir*/
		signal(consumidor_sem);
	}
}

/* ahora el consumidor*/
/*El consumidor impirme n 2000 veces y se va*/
void consumidor(void){
	int32 i;
	for(i=1; i<=2000; i++){
		/*espera que el productor le indique cuando consumir ja ja y si*/
		wait(consumidor_sem);
		printf("El valor de n es: %d \n",n);
		/*consume y le avisa al productor que ya puede volver a producir*/
		signal(productor_sem);
	}
}
