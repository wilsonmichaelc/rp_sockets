#ifndef GENERATE_ARBITRARY_WAVEFORM_H_
#define GENERATE_ARBITRARY_WAVEFORM_H_

typedef struct gen_params_tag
{
    //rp_channel_t channel;
    double frequency;
    double amplitude;
    float waveform[16384];
} gen_params;

void generate_arbitrary_waveform(gen_params *parameters);
int read_in_file(float *ch_data, char file_path[]);

#endif