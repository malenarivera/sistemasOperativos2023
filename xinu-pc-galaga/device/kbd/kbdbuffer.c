#include <xinu.h>

#define MAX_SIZE 10

char elementos[MAX_SIZE];
int frente, fin, contador;
sid32 mutexkbd, elementosDisponibles, espaciosDisponibles;


void inicializarCola() {
  frente = 0;
  fin = 0;
  contador = 0;
  mutex = semcreate(1); // Semáforo para garantizar exclusión mutua
  elementosDisponibles = semcreate(0); // Semáforo para esperar elementos disponibles
  espaciosDisponibles = semcreate(MAX_SIZE); // Semáforo para esperar espacios disponibles
}

char sacar() {
    wait(elementosDisponibles); // Esperar a que haya elementos disponibles

    wait(mutex); // Entrar en la sección crítica

    char elemento = elementos[frente];
    frente = (frente + 1) % MAX_SIZE;
    contador--;

    signal(mutex); // Salir de la sección crítica

    signal(espaciosDisponibles); // Incrementar el contador de espacios disponibles

return elemento;
}

void poner(char elemento) {
    wait(espaciosDisponibles); // Esperar a que haya espacios disponibles

    wait(mutex); // Entrar en la sección crítica

    elementos[fin] = elemento;
    fin = (fin + 1) % MAX_SIZE;
    contador++;

    signal(mutex); // Salir de la sección crítica

    signal(elementosDisponibles); // Incrementar el contador de elementos disponibles
}


