/* kbdgetc.c - kbdgetc */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  kbdgetc  -  Read one character from a el buffer
 *------------------------------------------------------------------------
 */
devcall	kbdgetc(
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{	
	char	ch;			/* Character to return		*/

	/* Wait for a character in the buffer and extract one character	*/

	ch =  sacar();
	return ch;
}
