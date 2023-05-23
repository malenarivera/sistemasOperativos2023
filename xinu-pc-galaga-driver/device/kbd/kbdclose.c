/* kbdclose.c  -  kbdclose */

#include <xinu.h>
extern sid32 mutexkbd;
extern int pidActual;

/*------------------------------------------------------------------------
 * kbdclose  -  Close the keyboard device
 *------------------------------------------------------------------------
 */
devcall	kbdclose (
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
	if(pidActual == getpid())
		signal(mutexkbd);
}
