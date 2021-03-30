#ifndef __SYNTHESIS_H
#define __SYNTHESIS_H

#include <math.h>

typedef void (* samplelistener)(int);

typedef struct {
	int *values;
	int note;
	int size;
} wavetable;

void synth_note(int note, wavetable *table, samplelistener listener);

#endif
