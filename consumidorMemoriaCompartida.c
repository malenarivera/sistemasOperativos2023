#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>


void main(){
    const char *name= "malena";
	const int SIZE=4096;
	void *ptr;
	int i;

	/*Abriendo el segmento de memoria compartida*/
    int shm_fd= shm_open(name, O_RDONLY, 0666);
	if(shm_fd==-1){
		printf("memoria compartida f\n");
	}


	/*Ahora mapeo el segmento de memoria compartida en la direccion de memoria del proceso*/
	ptr=mmap(0,SIZE,PROT_READ, MAP_SHARED, shm_fd ,0);
	if(ptr==MAP_FAILED){
		printf("MAP failes \n");
	}


	/*leo desde la mem compartida*/
	printf("%s", (char *)ptr);

	/*elimino de la memoria compartida*/
	if(shm_unlink(name) ==-1){
		printf("Error");
	}

  

}
