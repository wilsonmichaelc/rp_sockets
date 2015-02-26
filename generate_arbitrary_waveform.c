#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <rp.h>

#include "generate_arbitrary_waveform.h"

const int buff_size = 16384;

void generate_arbitrary_waveform(gen_params *parameters){

    gen_params params = *parameters;

    rp_GenWaveform(params.channel, RP_WAVEFORM_ARBITRARY);
    rp_GenArbWaveform(params.channel, params.waveform, buff_size);
    rp_GenAmp(params.channel, params.amplitude);
    rp_GenFreq(params.channel, params.frequency);
    rp_GenOutEnable(params.channel);

}