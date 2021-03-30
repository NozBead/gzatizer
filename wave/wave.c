#include "wave.h"

uint16 read_two_bytes(unsigned char *data, int *offset) {
	uint16 val = (data[*offset + 1] << 8) | data[*offset + 0];
	*offset += 2;
	return val;
}

uint32 read_four_bytes(unsigned char * data, int * offset) {
	uint32 val = (data[*offset + 3] << 24) | (data[*offset + 2]<<16) | (data[*offset + 1]<<8) | data[*offset + 0];
	*offset += 4;
	return val;
}

int read_header(int fd, waveheader *wav){
	int offset = 0;
	unsigned char raw[W_HEADSIZE];

	if (read(fd, raw, W_HEADSIZE) == -1) {
		perror("Error reading WAVE header");
		return -1;
	}

	wav->file_type_bloc_id 	= read_four_bytes(raw, &offset);
	wav->file_size 		= read_four_bytes(raw, &offset);
	wav->file_format_id 	= read_four_bytes(raw, &offset);
	wav->format_bloc_id 	= read_four_bytes(raw, &offset);
	wav->bloc_size 		= read_four_bytes(raw, &offset);
	wav->audio_format 	= read_two_bytes(raw, &offset);
	wav->number_channels 	= read_two_bytes(raw, &offset);
	wav->sample_frequence 	= read_four_bytes(raw, &offset);
	wav->byte_rate 		= read_four_bytes(raw, &offset);
	wav->byte_per_bloc 	= read_two_bytes(raw, &offset);
	wav->resolution 	= read_two_bytes(raw, &offset);
	wav->data_bloc_id 	= read_four_bytes(raw, &offset);
	wav->data_size 		= read_four_bytes(raw, &offset);

	return 0;
}

int read_audio_data(int fd, waveheader *wav, unsigned char *data){
	if (lseek(fd, W_HEADSIZE, SEEK_SET) == -1) {
		perror("Error repositioning cursor");
		return -1;
	}

	int size;
	if ((size = read(fd, data, wav->data_size)) == -1){
		perror("Error reading audio data");
		return -2;
	}

	return size;
}

void print_header(waveheader *wav) {
	printf(	"==WAVE DESCRIPTION==\n"
		"BlocID : %x\n"
		"FileSize : %d\n"
		"FormatID : %x\n"
		"==DATA==\n"
		"BlocID : %x\n"
		"DataSize : %d\n"
		"==AUDIO FORMAT==\n"
		"BlocID : %x\n"
		"BlocSize : %d\n"
		"AudioFormat : %d\n"
		"NumberChannels : %d\n"
		"SampleFreq : %d\n"
		"ByteRate : %d\n"
		"BytePerBloc : %d\n"
		"Resolution : %d\n",
		wav->file_type_bloc_id,
		wav->file_size,
		wav->file_format_id,
		wav->format_bloc_id,
		wav->bloc_size,
		wav->audio_format,
		wav->number_channels,
		wav->sample_frequence,
		wav->byte_rate,
		wav->byte_per_bloc,
		wav->resolution,
		wav->data_bloc_id,
		wav->data_size);
}
