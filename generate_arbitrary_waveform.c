#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <rp.h>

#include "generate_arbitrary_waveform.h"

const int buff_size = 16*1024;

int read_in_file(float *ch_data);

void generate_arbitrary_waveform(gen_params *parameters){

    gen_params params = *parameters;
    double frequency = (double)params.frequency;
    double amplitude = (double)params.amplitude;
    float waveform[buff_size];

    read_in_file(waveform);

    rp_GenWaveform(RP_CH_1, RP_WAVEFORM_ARBITRARY);
    rp_GenArbWaveform(RP_CH_1, waveform, buff_size);
    rp_GenAmp(RP_CH_1, amplitude);
    rp_GenFreq(RP_CH_1, frequency);
    rp_GenOutEnable(RP_CH_1);

}

int read_in_file(float *ch_data)
{
    FILE *fi = NULL;
    int i, read_size, samples_read = 0;
    char file_path[] = "/tmp/batman.csv";

    /* open file */
    fi = fopen(file_path, "r+");
    if (fi == NULL) {
        fprintf(stderr, "read_in_file(): Can not open input file (%s): %s\n",
                file_path, strerror(errno));
        return -1;
    }


    /* parse at most AWG_SIG_LEN lines and save data to the specified buffers */
    for (i = 0; i < buff_size; i++) {

        read_size = fscanf(fi, "%f \n", &ch_data[i]);
        if((read_size == EOF) || (read_size != 1)) {
            i--;
            break;
        }
    }
    samples_read = i + 1;

    if (samples_read >= buff_size)
        samples_read = buff_size - 1;

    /* check for errors */
    if (i == 0) {
        fprintf(stderr, "read_in_file() cannot read in signal, wrong format?\n");
        fclose(fi);
        return -1;
    }

    /* close a file */
    fclose(fi);

    /* and return the number of parsed lines */
    return samples_read;
}