#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>


void main(){
	const int SIZE=4096;
    const char *dire= "/usr/share/doc/util-linux/source-code-management.txt";
    const char *name= "malena";
	char area[1024];
	void *ptr;

	
	/*creando el segmento de memoria compartida*/
    int shm_fd= shm_open(name, O_CREAT | O_RDWR , 0666);

	 
	/*configurando el tamaño del segmento de memoria compartida*/	
	frtuncate(shm_fd, SIZE); 	

	/*Ahora mapeo el segmento de memoria compartida en la direccion de memoria del proceso*/
	ptr=mmap(0,SIZE,PROT_READ | PROT_WRITE, MAP_SHARED, shm,fd ,0);
	if(ptr==MAP_FAILED){
		printf("MAP failes \n");
	}


	/*leo el archivo de rafita*/
    int blabla=open(dire,O_RDONLY);
	while(int dasd=read(blabla,area, sizeof(area))>0){
		memcpy(ptr,area,dasd);
	}
	close(blabla);


	/*pongo lo que lei en la memoria compartida*/		
	sprintf(ptr, "%s", blabla);
	

}
