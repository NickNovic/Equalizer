#include <alsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <memory.h>
#ifndef PLAY_H
#define PLAY_H

#define PCM_DEVICE "default"
/*
 * According to the wikipedia (https://en.wikipedia.org/wiki/WAV),
 * header of an WAV file looks like this:
 * [Master RIFF chunk]
 * FileTypeBlocID  (4 bytes) : Identifier « RIFF »  (0x52, 0x49, 0x46, 0x46)
 * FileSize        (4 bytes) : Overall file size minus 8 bytes
 * FileFormatID    (4 bytes) : Format = « WAVE »  (0x57, 0x41, 0x56, 0x45)
 * 
 * [Chunk describing the data format]
 * FormatBlocID    (4 bytes) : Identifier « fmt␣ »  (0x66, 0x6D, 0x74, 0x20)
 * BlocSize        (4 bytes) : Chunk size minus 8 bytes, which is 16 bytes here  (0x10)
 * AudioFormat     (2 bytes) : Audio format (1: PCM integer, 3: IEEE 754 float)
 * NbrChannels     (2 bytes) : Number of channels
 * Frequency       (4 bytes) : Sample rate (in hertz)
 * BytePerSec      (4 bytes) : Number of bytes to read per second (Frequency * BytePerBloc).
 * BytePerBloc     (2 bytes) : Number of bytes per block (NbrChannels * BitsPerSample / 8).
 * BitsPerSample   (2 bytes) : Number of bits per sample
 * 
 * [Chunk containing the sampled data]
 * DataBlocID      (4 bytes) : Identifier « data »  (0x64, 0x61, 0x74, 0x61)
 * DataSize        (4 bytes) : SampledData size
 * 
 * SampledData
 * */

#pragma pack(push, 1)
typedef struct {
    char file_type_block_id[4];     /* "RIFF" */
    uint32_t file_size;
    char file_format_id[4];         /* "WAVE" */
    char format_block_id[4];        /* "fmt " */
    uint32_t block_size;
    uint16_t audio_format;
    uint16_t nbr_channels;
    uint32_t frequency;
    uint32_t byte_per_sec;
    uint16_t byte_per_block;
    uint16_t bits_per_sample;
    char data_block_id[4];          /* "data" */
    uint32_t data_size;

} wav_header;
#pragma pack(pop)
/*
 * Opens wav file and writes to header information about file
 * IMPORTANT: header description is stored in heap. DONT FORGET TO FREE THIS SPACE!!!
 * */

wav_header* get_wav_headers(FILE *wav_file);

/*
 * As input parameter gets pointer to header of file what we are about to play
 * */
int setup_pcm (wav_header *file_header, snd_pcm_t **pcm_handle);

void play_sound(size_t bytes_read, wav_header *file_header, snd_pcm_t *pcm_handle, char *buffer);

void close_pcm (snd_pcm_t **pcm_handle);

#endif
