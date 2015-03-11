#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <rp.h>

#include "acquire_to_socket.h"
int stop_continuous_acquisition = 0;

void *acquire_to_socket(acq_params *parameters){

    /* Get the socket */
    acq_params params = *parameters;
    int sock = *(int*)params.socket;
    int num_points = (int)params.num_points;
    rp_channel_t channel = (rp_channel_t)params.channel;
    rp_acq_decimation_t sample_rate = (rp_acq_decimation_t)params.sample_rate;
    rp_acq_trig_src_t trigger = (rp_acq_trig_src_t)params.trigger;

    /* Create buffer and size to hold data points */
    uint32_t array_size = 16*1024; 
    float *buff;
    buff = (float *)malloc(array_size * sizeof(float));

    if(num_points == 0){

        /* Continuously acquire data */
        while(stop_continuous_acquisition == 0){
            /* Starts acquisition */
            rp_AcqStart();
            rp_AcqSetTriggerSrc(trigger);
            rp_AcqSetSamplingRate(sample_rate);
            rp_AcqGetOldestDataV(channel, &array_size, buff);
            
            int i;
            for (i = 0; i < array_size; ++i)
            {
                //send(sock, &buff[i], sizeof(float), 0);
                char tmp[1024];
                sprintf(tmp, "%f", buff[i]);
                write(sock, tmp, strlen(tmp));
                write(sock, "\n", strlen("\n"));
            }
        }

    }else{

        while(num_points > 0){
            /* Starts acquisition */
            rp_AcqStart();
            rp_AcqSetTriggerSrc(trigger);
            rp_AcqSetSamplingRate(sample_rate);
            rp_AcqGetOldestDataV(channel, &array_size, buff);
            
            int i;
            for (i = 0; i < array_size; ++i)
            {
                //send(sock, &buff[i], sizeof(float), 0);
                char tmp[1024];
                sprintf(tmp, "%f", buff[i]);
                write(sock, tmp, strlen(tmp));
                write(sock, "\n", strlen("\n"));
            }
        }

    }

    /* Releasing resources */
    free(buff);
    return 0;
}