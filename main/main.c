#include <sys/mman.h>
#include <signal.h>
#include "../wave/wave.h"
#include "../wave/conversion.h"
#include "../synthesis/synthesis.h"
#include "../sound/sound.h"
#include "../sound/midi.h"

waveheader wav;
snd_pcm_t *pcm;

int *pids;
wavetable *table;

int sharemem(int nvalues) {
	table = mmap(NULL, sizeof(wavetable), PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	pids = mmap(NULL, 128 * sizeof(int), PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	table->values = mmap(NULL, nvalues * sizeof(int), PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	table->size = nvalues;

	if (table == MAP_FAILED || table->values == MAP_FAILED) {
		perror("Error sharing mem");
		return -1;
	}
	return 0;
}

int wavfd(char *path) {
	int fd = open(path, O_RDONLY);
	if (fd == -1) perror(path);
	return fd;
}

int midifd(void) {
	int fd = open("/dev/midi1", O_RDONLY);
	if (fd == -1) perror("Error opening midi");
	return fd;
}

void write_pcm(int value) {
	int frames = 0;
	unsigned char raw[wav.byte_per_bloc];
	tobendian(raw, value, wav.byte_per_bloc);

	if ((frames = snd_pcm_writei(pcm, (void **) &raw, 1)) < 0 ) {
		fprintf(stderr, "pcm write error: %s\n", snd_strerror(frames));
	}
}

void note_on(unsigned char *data) {
	int note = data[0];
	int pid = fork();
	if (pid == -1) {
		perror("Error forking synth");
	}
	else if (pid == 0) {
		pcm = create_pcm();
		printf("%d\n", note);
		synth_note(note, table, write_pcm);
	}
	else {
		pids[note] = pid;
	}
}

void note_off(unsigned char *data) {
	int note = data[0];
	int pid = pids[note];
	if (pid != 0) kill(pid, SIGKILL); 
}

int getaudio(int *dest, waveheader *wav, int wavfile) {
	unsigned char raw[wav->data_size];
	if (read_audio_data(wavfile, wav, raw) == -1) {
		return -1;
	}
	letoint(dest, raw, wav->byte_per_bloc, wav->data_size / wav->byte_per_bloc);
	return 0;
}

int main(int argc, char **argv) {
	if (argc != 3) return 1;

	int fdwav = wavfd(argv[1]);
	if (fdwav == -1) {
		return 2;
	}
	int fdmidi = midifd();
	if (fdmidi == -1) {
		return 3;
	}
	if (read_header(fdwav, &wav) == -1) {
		return 4;
	}
	if (sharemem(wav.data_size / wav.byte_per_bloc) == -1) {
		return 5;
	}
	if (getaudio(table->values, &wav, fdwav) == -1) {
		return 7;
	}

	table->note = atoi(argv[2]);

	midimsg msg1 = {note_off, 2}, msg2 = {note_on, 2};
	midimsg msgs[] = {msg1, msg2};
	midiloop(fdmidi, msgs);
	return 0;
}
