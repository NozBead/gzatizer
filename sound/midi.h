#ifndef __MIDI_H
#define __MIDI_H

#include <stdio.h>
#include <unistd.h>

#define STSMSK (1 << 7) /* 1000 0000 */
#define MIDMSK 0x70	/* 0111 0000 */

typedef void (*msghandler) (unsigned char *);
typedef struct {
	msghandler handler;
	int datasize;
} midimsg;

int isstatus(unsigned char byte);

int msgid(unsigned char status);

int readdata(int fd, unsigned char *dest, midimsg *msg);

midimsg *getmsg(unsigned char status, midimsg *msgs, int msgssize);

void midiloop(int fd, midimsg *msgs);

#endif
