#include "sound.h"

int get_default_pcm(snd_pcm_t **dest) {
	return snd_pcm_open(dest, "default", SND_PCM_STREAM_PLAYBACK, SND_PCM_ASYNC);
}

snd_pcm_t *create_pcm() {
	int err;
	snd_pcm_t *pcm;

	if((err = get_default_pcm(&pcm)) < 0) {
		fprintf(stderr, "Default pcm error : %s", snd_strerror(err));
	}
	
	if((err = snd_pcm_set_params(	pcm,
					SND_PCM_FORMAT_S16_BE,
					SND_PCM_ACCESS_RW_INTERLEAVED,
					1,
					44100,
					1,
					500000)) < 0) {
		fprintf(stderr, "Params error : %s", snd_strerror(err));
	}

	return pcm;	
}
