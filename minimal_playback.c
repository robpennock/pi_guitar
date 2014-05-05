#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
	      
main (int argc, char *argv[])
{
	int i;
	int err;
	short buf[128];		//buff size
	unsigned sample_rate = 44100;	//modified
	snd_pcm_t *playback_handle;	//pcm handle struct
	snd_pcm_hw_params_t *hw_params;	//contains a set of possible PCM hardware configurations
	const char *device_name = "default";
	
	//pcm means pulse control modulation
	//opens a pcm
	//change argv[1] to device_name
	if ((err = snd_pcm_open (&playback_handle, device_name, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
			fprintf (stderr, "cannot open audio device %s (%s)\n", 
			 default_device,
			 snd_strerror (err));
		exit (1);
	}
	//allocate memory   
	//allocate an invalid snd_pcm_hw_params_t using standard malloc
	if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
		fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
			 snd_strerror (err));
		exit (1);
	}
	//Fill params with a full configuration space for a PCM			 
	if ((err = snd_pcm_hw_params_any (playback_handle, hw_params)) < 0) {
		fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
			 snd_strerror (err));
		exit (1);
	}
	//restrict a configuration space to contain only one access type
	//this is a mandatory parameter
	if ((err = snd_pcm_hw_params_set_access (playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		fprintf (stderr, "cannot set access type (%s)\n",
			 snd_strerror (err));
		exit (1);
	}
	//Restrict a configuration space to contain only one format.
	//basically select a specific format for playback	
	if ((err = snd_pcm_hw_params_set_format (playback_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
		fprintf (stderr, "cannot set sample format (%s)\n",
			 snd_strerror (err));
		exit (1);
	}
	//changed from snd_hw_params_set_rate_near() as it seems to be antiquated
	//this fucntion sets the sample rate to be used
	if ((err = snd_pcm_hw_params_set_rate(playback_handle, hw_params, sample_rate, 0)) < 0) {
		fprintf (stderr, "cannot set sample rate (%s)\n",
			 snd_strerror (err));
		exit (1);
	}
	//Restrict a configuration space to contain only one channels count
	//simply put: set the number of channels
	//	1 for mono 2 for stereo
	if ((err = snd_pcm_hw_params_set_channels (playback_handle, hw_params, 2)) < 0) {
		fprintf (stderr, "cannot set channel count (%s)\n",
			 snd_strerror (err));
		exit (1);
	}
	//nstall one PCM hardware configuration chosen from a configuration space and snd_pcm_prepare it.
	//basically...
	//	final preparation of hardware (based on hw_params)
	//	now call snd_pcm_prepare to prepare playback device to recieve buffer of info for write
	if ((err = snd_pcm_hw_params (playback_handle, hw_params)) < 0) {
		fprintf (stderr, "cannot set parameters (%s)\n",
			 snd_strerror (err));
		exit (1);
	}
	//now that we are done instantiating the HW params, need to free allocated memory
	//	of said hw_paramsv to prevent a memory leak
	snd_pcm_hw_params_free (hw_params);

//set software stuff now
	//prepare pcm for use
	if ((err = snd_pcm_prepare (playback_handle)) < 0) {
		fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
			 snd_strerror (err));
		exit (1);
	}
//=============================================================================//
//pcm is now write ready
	
	//FINALLY YOU CAN WRITE TO YOUR PCM	
	for (i = 0; i < 10; ++i) {
		if ((err = snd_pcm_writei (playback_handle, buf, 128)) != 128) {
			fprintf (stderr, "write to audio interface failed (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
	}

	snd_pcm_close (playback_handle);
	exit (0);
	}
