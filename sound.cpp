#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <alsa/asoundlib.h>
#include "sound.h"

      
int play() {

	/*
	int i;
	int err;
	unsigned int bitRate = 44100;
	short buf[128];
	snd_pcm_t *playback_handle;
	snd_pcm_hw_params_t *hw_params;
	
	if ((err = snd_pcm_open(&playback_handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0) {

		fprintf(stderr, "cannot open audio device \"default\" (%s)\n", snd_strerror (err));
		exit(1);

	}
		   
	if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {

		fprintf(stderr, "cannot allocate hardware parameter structure (%s)\n", snd_strerror(err));
		exit(1);

	}
				 
	if ((err = snd_pcm_hw_params_any(playback_handle, hw_params)) < 0) {

		fprintf(stderr, "cannot initialize hardware parameter structure (%s)\n", snd_strerror(err));
		exit(1);

	}
	
	if ((err = snd_pcm_hw_params_set_access(playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {

		fprintf (stderr, "cannot set access type (%s)\n", snd_strerror(err));
		exit(1);

	}
	
	if ((err = snd_pcm_hw_params_set_format(playback_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {

		fprintf (stderr, "cannot set sample format (%s)\n", snd_strerror (err));
		exit (1);

	}

	if ((err = snd_pcm_hw_params_set_rate_near(playback_handle, hw_params, &bitRate, 0)) < 0) {

		fprintf (stderr, "cannot set sample rate (%s)\n", snd_strerror (err));
		exit(1);

	}
	
	if ((err = snd_pcm_hw_params_set_channels(playback_handle, hw_params, 2)) < 0) {

		fprintf(stderr, "cannot set channel count (%s)\n", snd_strerror (err));
		exit(1);

	}
	
	if ((err = snd_pcm_hw_params(playback_handle, hw_params)) < 0) {

		fprintf (stderr, "cannot set parameters (%s)\n", snd_strerror (err));
		exit (1);

	}
	
	snd_pcm_hw_params_free(hw_params);
	
	if ((err = snd_pcm_prepare(playback_handle)) < 0) {

		fprintf(stderr, "cannot prepare audio interface for use (%s)\n", snd_strerror(err));
		exit(1);

	}
	
	for (i = 0; i < 10; ++i) {

		if ((err = snd_pcm_writei(playback_handle, buf, 128)) != 128) {

			fprintf(stderr, "write to audio interface failed (%s)\n", snd_strerror(err));
			exit (1);

		}

	}
	
	snd_pcm_close(playback_handle);
	exit(0);
	*/

	unsigned int pcm, tmp, dir, rate;
	short volume = 16384;
	int i, channels, seconds;
	snd_pcm_t *pcm_handle;
	snd_pcm_hw_params_t *params;
	snd_pcm_uframes_t frames;
	char* buff;
	short* buff2;
	int buff_size, loops;

	rate 	 = 44100;
	channels = 2;
	seconds  = 10;

	/* Open the PCM device in playback mode */
	if (pcm = snd_pcm_open(&pcm_handle, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0) {

		printf("ERROR: Can't open \"%s\" PCM device. %s\n", "default", snd_strerror(pcm));

	}

	/* Allocate parameters object and fill it with default values*/
	snd_pcm_hw_params_alloca(&params);

	snd_pcm_hw_params_any(pcm_handle, params);

	/* Set parameters */
	if (pcm = snd_pcm_hw_params_set_access(pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {

		printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(pcm));

	}

	if (pcm = snd_pcm_hw_params_set_format(pcm_handle, params, SND_PCM_FORMAT_S16_LE) < 0) {

		printf("ERROR: Can't set format. %s\n", snd_strerror(pcm));

	}

	if (pcm = snd_pcm_hw_params_set_channels(pcm_handle, params, channels) < 0) {

		printf("ERROR: Can't set channels number. %s\n", snd_strerror(pcm));

	}

	if (pcm = snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rate, 0) < 0) {

		printf("ERROR: Can't set rate. %s\n", snd_strerror(pcm));

	}

	/* Write parameters */
	if (pcm = snd_pcm_hw_params(pcm_handle, params) < 0) {

		printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(pcm));

	}

	/* Resume information */
	printf("PCM name: '%s'\n", snd_pcm_name(pcm_handle));

	printf("PCM state: %s\n", snd_pcm_state_name(snd_pcm_state(pcm_handle)));

	snd_pcm_hw_params_get_channels(params, &tmp);
	printf("channels: %i ", tmp);

	if (tmp == 1) {

		printf("(mono)\n");

	} else if (tmp == 2) {

		printf("(stereo)\n");

	}

	snd_pcm_hw_params_get_rate(params, &tmp, 0);
	printf("rate: %d bps\n", tmp);

	printf("seconds: %d\n", seconds);	

	/* Allocate buffer to hold single period */
	snd_pcm_hw_params_get_period_size(params, &frames, 0);

	buff_size = frames * channels * 2 /* 2 -> sample size */;
	buff = (char*)malloc(buff_size);
	buff2 = (short*)buff;

	snd_pcm_hw_params_get_period_time(params, &tmp, NULL);

	for (loops = (seconds * 1000000) / tmp; loops > 0; --loops) {

		/*
		if (pcm = read(0, buff, buff_size) == 0) {

			printf("Early end of file.\n");
			return 0;

		}
		*/
		for (i = 0; i < (buff_size >> 1); ++i) {

			buff2[i] = volume * sin((M_PI * 2.0 * 440.0 / 44100.0) * i);

		}

		if (pcm = snd_pcm_writei(pcm_handle, buff, frames) == -EPIPE) {
			
			printf(".");
			snd_pcm_prepare(pcm_handle);

		} else if (pcm < 0) {

			printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(pcm));

		}

	}

	snd_pcm_drain(pcm_handle);
	snd_pcm_close(pcm_handle);
	free(buff);

	exit(0);

}


