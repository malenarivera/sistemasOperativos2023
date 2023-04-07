#include <xinu.h>

void sndA(void),sndB(void),sndch(char);

/*------------------------------------------------------------------------
 * main  --  example of creating processes in Xinu
 *------------------------------------------------------------------------
 */
void miPrograma(void){
int pidProcessA, pidProcessB;

pidProcessA=create(sndA, 1024, 20, "process 1", 0);
pidProcessB=create(sndB, 1024, 20, "process 2", 0);
resume(pidProcessA);
resume(pidProcessB);
sleep(5);
kill(pidProcessA);
kill(pidProcessB);

resume(create(sndch, 1024, 20, "send A", 1, 'p') );
resume(create(sndch, 1024, 20, "send B", 1, 'x') );
}

/*------------------------------------------------------------------------
 * sndA  --  repeatedly emit 'A' on the console without terminating
 *------------------------------------------------------------------------
 */
void sndA(void){
while( 1 )
putc(CONSOLE, 'A');
}

/*------------------------------------------------------------------------
 * sndB  --  repeatedly emit 'B' on the console without terminating
 *------------------------------------------------------------------------
 */
void sndB(void){
while( 1 )
putc(CONSOLE, 'B');
}


/* probando probando*/
/*------------------------------------------------------------------------
 * sndch  --  output a character on a serial device indefinitely
 *------------------------------------------------------------------------
 */
void sndch(char ch){
while ( 1 )
	putc(CONSOLE, ch);
}
