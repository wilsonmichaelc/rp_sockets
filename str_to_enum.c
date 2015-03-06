#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <rp.h>

rp_channel_t getChannel(char channel[]){

    if( strcmp("CH1",channel) ){
        return RP_CH_1;
    }
    else if( strcmp("CH2",channel) ){
        return RP_CH_2;
    }else{
        return -1;
    }
    
}

rp_acq_trig_src_t getTrigger(char trigger[]){

    if( strcmp("NOW",trigger) ){
        return RP_TRIG_SRC_NOW;
    }
    else if( strcmp("CHA_PE",trigger) ){
        return RP_TRIG_SRC_CHA_PE;
    }
    else if( strcmp("CHA_NE",trigger) ){
        return RP_TRIG_SRC_CHA_NE;
    }
    else if( strcmp("CHB_PE",trigger) ){
        return RP_TRIG_SRC_CHB_PE;
    }
    else if( strcmp("CHB_NE",trigger) ){
        return RP_TRIG_SRC_CHB_NE;
    }else{
        return -1;
    }

}

rp_acq_sampling_rate_t getSampleRate(char rate[]){

    if( strcmp("125M",rate) ){
        return RP_SMP_125M;
    }
    else if( strcmp("15625M",rate) ){
        return RP_SMP_15_625M;
    }
    else if( strcmp("1953M",rate) ){
        return RP_SMP_1_953M;
    }
    else if( strcmp("122070K",rate) ){
        return RP_SMP_122_070K;
    }
    else if( strcmp("15258K",rate) ){
        return RP_SMP_15_258K;
    }
    else if( strcmp("1907K",rate) ){
        return RP_SMP_1_907K;
    }else{
        return -1;
    }
}