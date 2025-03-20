#include "play.h"

int main(void) {
    snd_pcm_t *pcm_handle; 
    //int dir;

    FILE *wav_file = fopen("du_hast.wav", "r");
    wav_header *file_header = get_wav_headers(wav_file);
    setup_pcm(file_header, &pcm_handle);
    
    int buffer_size = 4096;
    char* buffer = (char*)malloc(buffer_size);

    while(!feof(wav_file)) {
        size_t bytes_read = fread(buffer, 1, buffer_size, wav_file);
        play_sound(bytes_read, file_header, pcm_handle, buffer);
        // display() // here I should call FE to show;
    }
    
    close_pcm(&pcm_handle);
    free(buffer);
    free(file_header);
    fclose(wav_file);
    return 0;
}
