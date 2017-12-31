#include <conio.h>
#include <ioport.h>
#include <dos2.h>
#include <stdlib.h>

#include "imfplay.h"
#include "delay.h"

#define MAX_IMF_SIZE 32768
#define FILE_CHUNK_SIZE 1024

uint8_t opl2port = 0xC0;

uint8_t buffer[MAX_IMF_SIZE];

void opl2_send_data(unsigned char reg, unsigned char data) {
    out(opl2port, reg);
    out(opl2port + 1, data);
}

void opl2_reset(void) {
    int i;

    for (i = 0x01; i <= 0xF5; i++) {
        opl2_send_data(i, 0x00);
    }
}

void opl2_test_sample(void) {
    opl2_send_data(0x20, 0x01); /* Set the modulator's multiple to 1 */
    opl2_send_data(0x40, 0x10); /* Set the modulator's level to about 40 dB */
    opl2_send_data(0x60, 0xF0); /* Modulator attack: quick; decay: long */
    opl2_send_data(0x80, 0x77); /* Modulator sustain: medium; release: medium */
    opl2_send_data(0xA0, 0x98); /* Set voice frequency's LSB (it'll be a D#) */
    opl2_send_data(0x23, 0x01); /* Set the carrier's multiple to 1 */
    opl2_send_data(0x43, 0x00); /* Set the carrier to maximum volume (about 47 dB) */
    opl2_send_data(0x63, 0xF0); /* Carrier attack: quick; decay: long */
    opl2_send_data(0x83, 0x77); /* Carrier sustain: medium; release: medium */
    opl2_send_data(0xB0, 0x31); /* Turn the voice on; set the octave and freq MSB */
}

void opl2_play_imf_file(char *fname, uint16_t freq) {

    typedef struct {
        uint8_t reg;
        uint8_t data;
        uint16_t delay;
    } t_sound_data;

    int8_t file;
    uint32_t file_size, song_size;
    int16_t bytes_read;
    float wait, percentage;
    t_sound_data *sound_data;
    int record;

    file = open(fname, O_RDONLY);

    if (file < 0) {
        puts("Error opening file!\r\n");
        exit(1);
    }

    file_size = lseek(file, 0, SEEK_END);
    lseek(file, 0, SEEK_SET);

    if (file_size > MAX_IMF_SIZE) {
        puts("IMF file is too big.\r\n");
        close(file);
        exit(1);
    }

    puts("Loading IMF file...\r\n");

    record = 0;

    bytes_read = read(file, buffer + record, FILE_CHUNK_SIZE);

    while(bytes_read > 0) {
        percentage = ((float) record / (float) file_size) * 100.0;

        putdec8(percentage);
        puts("%\r");

        record += FILE_CHUNK_SIZE;

        bytes_read = read(file, buffer + record, FILE_CHUNK_SIZE);
    }

    puts("100%\r\n");

    close(file);

    song_size = *((unsigned short *)buffer);

    record = 2;

    if (song_size == 0) {
        puts("Type-0 IMF file detected!\r\n");
        song_size = file_size;
        record = 0;
    }

    puts("Playing IMF file...\r\n");

    opl2_reset();

    for (; record < song_size; record += sizeof(t_sound_data)) {

        sound_data = (t_sound_data *)(buffer + record);

        if (sound_data->reg < 0x01 || sound_data->reg > 0xF5) {
            continue;
        }

        opl2_send_data(sound_data->reg, sound_data->data);

        if (sound_data->delay) {
            wait = ((float) sound_data->delay / (float) freq) * 1000.0;
            delay((uint16_t)wait);
        }
    }

    opl2_send_data(0xB0, 0x11); /* Turn the voice off */
}

int main(char **argv, int argc) {

    int frequency = 560;

    puts("IMFPLAY v0.2.1 - (c)2017/2018 bootsector - http://www.brunofreitas.com/\r\n\r\n");

    if (argc != 3) {
        puts("Usage: IMFPLAY <IMF_FILE.IMF> <IO PORT> <IMF FREQUENCY> (280, 560 or 700)\r\n\r\n");
        puts("Where:\r\n");
        puts("  <IMF_FILE.IMF> -> IMF file name\r\n");
        puts("  <IO PORT> -> OPL2 board IO port, in decimal. Ex: 192 (0xC0) or 196 (0xC4) \r\n");
        puts("  <IMF FREQUENCY> -> IMF frequency. Usually 560. Increase if music is too slow\r\n");

        return 1;
    }

    opl2port = atoi(argv[1]);
    frequency = atoi(argv[2]);

    opl2_play_imf_file(argv[0], frequency);

    return 0;
}
