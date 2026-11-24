#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

#define SAMPLE_RATE 44100
#define DURATION_SECONDS 5
#define TOTAL_SAMPLES (SAMPLE_RATE * DURATION_SECONDS)

struct wav_header {
    char riff[4];                   // Resource Interchange File Format - the WAV file is a subset of this format = "RIFF"               
    int32_t file_size;              // file size in Bytes

    char wave[4];                   // file type header = "WAVE"

    char fmt[4];                    // format chunk = "fmt " with a trailing space (so its 4 bytes)

    int32_t fmt_size;               // size of the fmt chunk, usually 16 bytes
    int16_t format_type;            // 1=PCM, 257=Mu-Law, 258=A-Law, 259=ADPCM
    int16_t num_channels;           // 1 - mono, 2 - stereo
    int32_t sample_rate;            // number of samples per second, usually 44100
    int32_t bytes_per_second;       // how many bytes of data are 1 second of playback      
    int16_t bytes_per_sample;       // bytes per sample, just the bits per sample divided by 8 multiplied by the number of channels. 2=16-bit mono, 4=16-bit stereo
    int16_t bits_per_sample;        // more bits = higher vertical resolution for the wave - higher freq are more clear and lower ones as well, 8 bits would be like crunched

    char data[4];                   // data chunk = "data"
    int32_t data_size;              // the size of the data chunk in bytes = file size - 44, because the wav header is 44
};

int main(void) {

    struct wav_header wavh;

    const float MIDDLE_C = 256.00;
    const double AMPLITUDE = 10000.0; // (higher means louder)
    
    short int buffer[TOTAL_SAMPLES] = {0}; // create a buffer where we will store the actual sound data

    const int header_size = sizeof(struct wav_header);

    strncpy(wavh.riff, "RIFF", 4);
    strncpy(wavh.wave, "WAVE", 4);
    strncpy(wavh.fmt, "fmt ", 4);
    strncpy(wavh.data, "data", 4);

    wavh.fmt_size = 16; // 16 bytes - constant
    wavh.format_type = 1; // 1 is PCM
    wavh.num_channels = 1;
    wavh.sample_rate = SAMPLE_RATE;
    
    wavh.bits_per_sample = 16; // higher number = cleaner/crispier audio. That is why 8-bit audio sounds crunchy
    wavh.bytes_per_sample = wavh.bits_per_sample / 8 * wavh.num_channels;
    wavh.bytes_per_second = wavh.sample_rate * wavh.bytes_per_sample;

    // populate the buffer with fancy math
    for (int i = 0; i < TOTAL_SAMPLES; i++) {
        buffer[i] = (short int)((cos((2 * M_PI * MIDDLE_C * i) / SAMPLE_RATE) * AMPLITUDE));
    }

    wavh.data_size = TOTAL_SAMPLES * wavh.bytes_per_sample;
    wavh.file_size = wavh.data_size + header_size - 8;

    FILE* fptr = fopen("output.wav", "wb");

    if (!fptr) {
        return 1;
    }

    fwrite(&wavh, 1, header_size, fptr); // write the header
    fwrite(buffer, wavh.bytes_per_sample, TOTAL_SAMPLES, fptr); // write the buffer (the actual data)

    fclose(fptr);

    return 0;
}