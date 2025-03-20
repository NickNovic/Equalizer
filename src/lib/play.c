#include "play.h"

#include <alsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <memory.h>

#define PCM_DEVICE "default"

/*
 * Opens wav file and writes to header information about file
 * IMPORTANT: header description is stored in heap. DONT FORGET TO FREE THIS SPACE!!!
 * */

wav_header* get_wav_headers(FILE *wav_file) {
    wav_header *file_header = malloc(sizeof(wav_header));

    if (wav_file == NULL) {
        perror("error openning .wav file");
        
        free(file_header);
    }
    if (wav_file == NULL) {
        perror("no such file");

        free(file_header);
        return NULL; 
    }

    fread(file_header, sizeof(wav_header), 1, wav_file);
    if (strncmp(file_header->file_type_block_id, "RIFF", 4) != 0 
        || strncmp(file_header->file_format_id, "WAVE", 4) !=0 ) {
        perror("not a valid WAV file!");
        memset(file_header, 0, sizeof(wav_header));
        
        free(file_header);
        return NULL;
    }

    return file_header;
}

/*
 * As input parameter gets pointer to header of file what we are about to play
 * */
int setup_pcm (wav_header *file_header, snd_pcm_t **pcm_handle) {
    int dir;
    snd_pcm_hw_params_t *params;
    // Open PCM device
    if (snd_pcm_open(pcm_handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0) < 0) {
        perror("Error opening PCM device");
        exit(1);
    }

    // Allocate and configure parameters
    snd_pcm_hw_params_malloc(&params);
    snd_pcm_hw_params_any(*pcm_handle, params);
    snd_pcm_hw_params_set_access(*pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    
    // Set format
    snd_pcm_format_t format = (file_header->bits_per_sample == 16) ? SND_PCM_FORMAT_S16_LE : SND_PCM_FORMAT_U8;
    snd_pcm_hw_params_set_format(*pcm_handle, params, format);
    
    // Set channels and sample rate
    snd_pcm_hw_params_set_channels(*pcm_handle, params, file_header->nbr_channels);
    snd_pcm_hw_params_set_rate_near(*pcm_handle, params, &file_header->frequency, &dir);

    // Apply parameters
    if (snd_pcm_hw_params(*pcm_handle, params) < 0) {
        perror("Error setting HW params"); 
        snd_pcm_close(*pcm_handle);
        return 1;
    }

    return 0;
}

void play_sound(size_t bytes_read, wav_header *file_header, snd_pcm_t *pcm_handle, char *buffer) {
    int frames = bytes_read / (file_header->bits_per_sample / 8 * file_header->nbr_channels);
        
    if (snd_pcm_writei(pcm_handle, buffer, frames) == -EPIPE) {
        snd_pcm_prepare(pcm_handle);
    }
}

void close_pcm (snd_pcm_t **pcm_handle) {
    snd_pcm_drain(*pcm_handle);
    snd_pcm_close(*pcm_handle);
}
