#include <xinu.h>

void cambiandoPrioridades ( 

/* argumentos: */
int pid_a, int ms_a, /* PID de a y rafaga de a en ms */
int pid_b, int ms_b, /* PID de b y rafaga de b en ms */
int pid_c, int ms_c /* PID de c y rafaga de c en ms */
){

	/* obtener el PID del planificador (proceso actual) */
	int pidMain= getpid();
	/*obtener la prioridad del planificador (proceso con la mas alta prioridad) */
	int prioridadMain= getprio(pidMain);

	while (1) { 
	    /* obtener la prioridad del proceso a */
			int prioA= getprio(pid_a);

		/* cambiar la prioridad del proceso a, a un valor igual a la prioridad del planificador menos 1 */
			chprio(pid_a, prioridadMain-1);

			

		/* liberar la CPU por ms_a ms (ponerse a dormir), por lo que Xinu asignará la CPU al proceso a */
			sleepms(ms_a);

		/* devolverle al proceso a su prioridad original */
			chprio(pid_a, prioA);

			


/* ---------------------------- */
/* planificar aquí el proceso b */
/* ---------------------------- */

        /* obtener la prioridad del proceso b */
			int prioB= getprio(pid_b);

		/* cambiar la prioridad del proceso b, a un valor igual a la prioridad del planificador menos 1 */
			chprio(pid_b, prioridadMain-1);

			

		/* liberar la CPU por ms_a ms (ponerse a dormir), por lo que Xinu asignará la CPU al proceso b */
			sleepms(ms_b);

		/* devolverle al proceso b su prioridad original */
			chprio(pid_b, prioB);

			

/* ---------------------------- */
/* planificar aquí el proceso c */
/* ---------------------------- */

        /* obtener la prioridad del proceso c */
			int prioC= getprio(pid_c);

		/* cambiar la prioridad del proceso c, a un valor igual a la prioridad del planificador menos 1 */
			chprio(pid_c, prioridadMain-1);

		

		/* liberar la CPU por ms_a ms (ponerse a dormir), por lo que Xinu asignará la CPU al proceso c*/
			sleepms(ms_c);

		/* devolverle al proceso a su prioridad original */
			chprio(pid_c, prioC);

			
	  }   
}

process mensajito(char c){
int x=0;
	while(1){
		printf("Proceso %c: %d \n",c,x);
		x++;
	}
}



void inicializarProcesitos(void){
int pidA, msA, pidB, msB, pidC, msC;
pidA=create(mensajito, 1024, 20, "process 2", 1,'A');
pidB=create(mensajito,1024, 20, "Process B", 1,'B');
pidC=create(mensajito,1024, 20, "Process C", 1,'C');

msA= ((60*200)/100);
msB= ((30*200)/100);
msC= ((10*200)/100);

resume(pidA);
resume(pidB);
resume(pidC);

cambiandoPrioridades(pidA,msA,pidB,msB,pidC,msC);
}

void planificadorDeProcesos(){
int pidProcesito;
pidProcesito=create (inicializarProcesitos,1024,30,"procesito",0);
resume(pidProcesito);
}

