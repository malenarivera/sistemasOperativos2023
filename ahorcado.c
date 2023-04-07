#include <xinu.h>
#include <stdio.h>

void mostrar(char letrasEncon[], int longPala){
for( int p=0; p<longPala-1;p++){
	    if(letrasEncon[p]=='\0'){
	        printf("_ ",p);
	    }else{
	        printf("%c ",letrasEncon[p]);
        }
    }
    printf("\n");
 
}

char pedirLetra(void){
	char c;
printf("\r                                                          ");
		printf("\r c = %c  ingrese una letra:  ",c);
		c = getchar();
		printf("\n");
return c;
}

void ahorcado() {
int erroresMax,errores = 1,encontrada = 0, longPala, letraAcertada;
	erroresMax = 6;
	char palabra[] = "sotfware";
	longPala = sizeof(palabra);
	char letrasEncon[longPala];
	char c;
	
	/* Decirle al sistema que el modo input es RAW */
control(0, TC_MODER, 0, 0);
	while(encontrada!=1 && errores < erroresMax) {
		c = pedirLetra();
		int cantLetrasEnco = 0;
		for(int i=0;i<longPala;i++){
			if(palabra[i] == c){
				letrasEncon[i] = c;
				letraAcertada = 1;
			}
			if(palabra[i] == letrasEncon[i]){
				cantLetrasEnco++;
			}
		}
		if(letraAcertada == 0){
			printf("Error %d: la letra no forma parte de la palabra. \n Cantidad de errores disponibles: %d\n", errores,(erroresMax-errores));
			errores++;
		}else{
			printf("Acierto: la letra forma parte de la palabra\n");
			letraAcertada = 0;	
		}
		if(cantLetrasEnco == longPala){
			encontrada = 1;
			printf("\nFELICIDADES, HAS GANADO!, \n La palabra era: ");	
		}
			
		mostrar(letrasEncon, longPala);
		
	}
		
	if(errores==erroresMax){
		printf("\n LO SENTIMOS!, PERDISTE :(.\n No te preocupes, vuelve a intentarlo!\n");
	}	
	control(0, TC_MODEC, 0, 0);
} 
