#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <rp.h>

const int buff_size = 16384;

void generate_arbitrary_waveform(char channel, double amplitude, double frequency, float *waveform[]){

    float ch_arb[buff_size];

    if(channel == 1){

        rp_GenWaveform(RP_CH_1, RP_WAVEFORM_ARBITRARY);
        rp_GenArbWaveform(RP_CH_1, ch_arb, buff_size);
        rp_GenAmp(RP_CH_1, amplitude);
        rp_GenFreq(RP_CH_1, frequency);
        rp_GenOutEnable(RP_CH_1);

    }else if(channel == 2){

        rp_GenWaveform(RP_CH_2, RP_WAVEFORM_ARBITRARY);
        rp_GenArbWaveform(RP_CH_2, ch_arb, buff_size);
        rp_GenAmp(RP_CH_2, amplitude);
        rp_GenFreq(RP_CH_2, frequency);
        rp_GenOutEnable(RP_CH_2);

    }else{

        rp_GenWaveform(RP_CH_1, RP_WAVEFORM_ARBITRARY);
        rp_GenArbWaveform(RP_CH_1, ch_arb, buff_size);
        rp_GenAmp(RP_CH_1, amplitude);
        rp_GenFreq(RP_CH_1, frequency);
        rp_GenOutEnable(RP_CH_1);

        rp_GenWaveform(RP_CH_2, RP_WAVEFORM_ARBITRARY);
        rp_GenArbWaveform(RP_CH_2, ch_arb, buff_size);
        rp_GenAmp(RP_CH_2, amplitude);
        rp_GenFreq(RP_CH_2, frequency);
        rp_GenOutEnable(RP_CH_2);

    }

}