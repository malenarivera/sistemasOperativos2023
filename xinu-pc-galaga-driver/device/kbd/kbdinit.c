/* kbdinit.c  -  kbdinit */

#include <xinu.h>
#include <keyboard.h>

int pidActual;
sid32 mutexkbd;
int frente, fin, contador;
char elementos[10];
sid32  elementosDisponibles, espaciosDisponibles;


unsigned char kblayout [128];  // { ... } Fill your layout yourself

void keyboard_wait(byte a_type) //unsigned char
{
  int _time_out=100000; //unsigned int
  if(a_type==0)
  {
    while(_time_out--) //Data
    {
      if((inportb(0x64) & 1)==1)
      {
        return;
      }
    }
    return;
  }
  else
  {
    while(_time_out--) //Signal
    {
      if((inportb(0x64) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}




/*------------------------------------------------------------------------
 *  kbdinit  -  Initialize the ps/2 keyboard
 *------------------------------------------------------------------------
 */
devcall	kbdinit (
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
	frente = 0;
    fin = 0;
    contador = 0;
	mutexkbd = semcreate(1); // Semáforo para garantizar exclusión mutua
    elementosDisponibles = semcreate(0); // Semáforo para esperar elementos disponibles
    espaciosDisponibles = semcreate(10); // Semáforo para esperar espacios disponibles
	
	int i;
	for (i=0; i<128; i++)
		kblayout[i] = i;
	//keyboard_restart();


	byte _status;  //unsigned char

	//Enable the keyboard device   old: auxiliary mouse device
	keyboard_wait(1);
	outportb(0x64, 0xAE);
 
	//Enable the interrupts
	keyboard_wait(1);
	outportb(0x64, 0x20);
	keyboard_wait(0);
	_status=(inportb(0x60) | 1);
	keyboard_wait(1);
	outportb(0x64, 0x60);
	keyboard_wait(1);
	outportb(0x60, _status);
 
	//Tell the keyboard to use default settings
	/* while((inportb(0x64)&2)!=0){};*/
	// outportb(0x64, 0xF6);
	//keyboard_read();  //Acknowledge
 
	//Enable the keyboard
	while ((inportb(0x64) & 2) != 0) {
		inportb(0x60);
	}; 


	set_evec(1 + IRQBASE, (uint32)kbdhandlerirq);
	//outportb(0x64, 0xF4);
	//keyboard_read();  //Acknowledge

	return OK;
}


