/* source: https://forum.osdev.org/viewtopic.php?t=10247 */

/* kbdhandler.c - kbdhandler */

#include <xinu.h>
#include <keyboard.h>

//unsigned char tecla_actual;

unsigned char get_scancode()
{
	unsigned char inputdata;
	inputdata = inportb(KEYBOARD_DATAREG);
	return inputdata;
}

/*------------------------------------------------------------------------
 *  kbdhandler  -  Handle an interrupt for the keyboard device
 *------------------------------------------------------------------------
 */
void kbdhandler(void)
{
	char t[80];
	unsigned char scancode;

	scancode = get_scancode();
	poner(scancode);
}
