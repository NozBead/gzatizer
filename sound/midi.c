#include "midi.h"

int isstatus(unsigned char byte) {
	return byte & STSMSK;
}

int msgid(unsigned char status) {
	return (status & MIDMSK) >> 4;
}

int readdata(int fd, unsigned char *dest, midimsg *msg) {
	int bytesread = 0;
	bytesread = read(fd, dest, msg->datasize);
	if (bytesread < 0) {
		perror("Error reading payload");
	}
	return bytesread;
}

midimsg *getmsg(unsigned char status, midimsg *msgs, int msgssize) {
	int id = msgid(status);
	midimsg *msg = NULL;
	if (id < msgssize) {
		msg = msgs+id;
	}
	return msg;
}

void midiloop(int fd, midimsg *msgs) {
	unsigned char byte;
	while (1) {
		if (read(fd, &byte, 1) == -1) {
			perror("Error reading midi");
			return;
		}
		if (isstatus(byte)) {
			unsigned char data[2];
			midimsg *msg = getmsg(byte, msgs, 2);
			readdata(fd, data, msg);
			msg->handler(data);
		}
	}	
}
