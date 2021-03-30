#include "synthesis.h"
int lookup_table(double idx, wavetable *table){
	int n = (int) idx;
	int s_n = table->values[n];

	/*Get fractionnal part*/
	double k = idx - n;

	if(k == 0) {
		return s_n;
	}
	else {
		int n1 = n + 1;
		if (n1 >= table->size) n1 = 0;	
		int s_n1 = table->values[n1];
		/*Linear interpolation*/
		return ((1 - k) * s_n) + (k * s_n1);
	}
}

double get_increment(int new_note, wavetable *table) {
	return 1 + ((new_note - table->note) * 0.05946309);
}

void synth_note(int note, wavetable *table, samplelistener listener) {
	double incr = get_increment(note, table);
	double idx = 0;
	while (1) {
		int value = lookup_table(idx, table);
		listener(value);
		idx = fmod((idx + incr), table->size);
	}
}
