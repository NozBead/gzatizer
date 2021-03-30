#ifndef __WAVE_H
#define __WAVE_H

#include <unistd.h>
#include <stdio.h>

#define W_HEADSIZE 44

typedef unsigned short uint16;
typedef unsigned int uint32;

typedef struct{
	uint32 file_type_bloc_id;
	uint32 file_size;
	uint32 file_format_id;
	uint32 format_bloc_id;
	uint32 bloc_size;
	uint16 audio_format;
	uint16 number_channels;
	uint32 sample_frequence;
	uint32 byte_rate;
	uint16 byte_per_bloc;
	uint16 resolution;
	uint32 data_bloc_id;
	uint32 data_size;
} waveheader;

int read_header(int fd, waveheader *wav);
void print_header(waveheader *wav);
int read_audio_data(int fd, waveheader *wav, unsigned char *data);
#endif
