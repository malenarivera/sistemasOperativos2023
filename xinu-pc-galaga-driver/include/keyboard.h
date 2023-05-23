
#define inportb(p)      inb(p)
#define outportb(p,v)   outb(p,v)

#define KEYBOARD_DATAREG 0x60  // :Data Register(Read\Write)
#define KEYBOARD_CMDREG 0x64   // :Command Register(Read\Write) 

#ifndef	MAX_BUFFER
#define	MAX_BUFFER	10		/* Num. chars in input queue	*/
#endif


extern unsigned char kblayout [128];  // { ... } Fill your layout yourself 

struct	tecladito	{			/* Tty line control block	*/
	char	*tyihead;		/* Next input char to read	*/
	char	*tyitail;		/* Next slot for arriving char	*/
	char	kbd[MAX_BUFFER];	/* Input buffer (holds one line)*/
	sid32   semkbd;
};
extern	struct	tecladito	kbdtab[];