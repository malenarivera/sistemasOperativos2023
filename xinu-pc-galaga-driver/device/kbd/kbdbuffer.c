#include <xinu.h>

extern int contador, frente, fin;
extern sid32 espaciosDisponibles, elementosDisponibles;
extern char elementos[];

char sacar() {
    wait(elementosDisponibles); // Esperar a que haya elementos disponibles
    char elemento = elementos[frente];
    frente = (frente + 1) % 10;
    contador--;
    signal(espaciosDisponibles); // Incrementar el contador de espacios disponibles

return elemento;
}

void poner(char elemento) {
    wait(espaciosDisponibles); // Esperar a que haya espacios disponibles
    elementos[fin] = elemento;
    fin = (fin + 1) % 10;
    contador++;
    signal(elementosDisponibles); // Incrementar el contador de elementos disponibles
}


