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
	/*Pongo los pids en un arreglo*/
	int pids[] = {pid_a,pid_b,pid_c};
	int tiempoDispo[] = {ms_a,ms_b,ms_c};
	int i = 0;
	while (1) { 
	    int pidCambiar = pids[i%3]; 
	    /* obtener la prioridad del proceso a cambiar */
			int prioCambiar= getprio(pidCambiar);

		/* cambiar la prioridad del proceso, a un valor igual a la prioridad del planificador menos 1 */
			chprio(pidCambiar, prioridadMain-1);


		/* liberar la CPU por ms_a ms (ponerse a dormir), por lo que Xinu asignará la CPU al proceso*/
			sleepms(tiempoDispo[i%3]);

		/* devolverle al proceso a su prioridad original */
			chprio(pidCambiar, prioCambiar);
        
        i++;
	  }   
}

process mensajito(char c){
int x=0;
	while(1){
		printf("Proceso %c: %d \n",c,x);
		x++;
	}
}

process organizador(){
int pidA, msA, pidB, msB, pidC, msC;

pidA=create(mensajito, 1024, 20, "Process A", 1,'A');
pidB=create(mensajito,1024, 20, "Process B", 1,'B');
pidC=create(mensajito,1024, 20, "Process C", 1,'C');

msA= ((60*200)/100);/*Calcula el 60%*/
msB= ((30*200)/100);/*Calcula el 30%*/
msC= ((10*200)/100);/*Calcula el 10%*/

resume(pidA);
resume(pidB);
resume(pidC);

cambiandoPrioridades(pidA,msA,pidB,msB,pidC,msC);
}


process planificadorDeProcesos(){
    int pidsh = create(organizador,1024,30,"Proceso Organizador",0);
    resume(pidsh);
}


