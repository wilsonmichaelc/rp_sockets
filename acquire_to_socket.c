#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <rp.h>

#include "acquire_to_socket.h"

void *acquire_to_socket(acq_params *parameters){

    /* Get the socket */
    acq_params params = *parameters;
    int sock = *(int*)params.socket;

    /* Create buffer and size to hold data points */
    uint32_t array_size = 16 * 1024; 
    float *buff;
    buff = (float *)malloc(array_size * sizeof(float));

    /* Starts acquisition */
    rp_AcqStart();
    rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);
    rp_AcqSetSamplingRate(RP_SMP_15_625M);
    rp_AcqGetOldestDataV(RP_CH_1, &array_size, buff);

    /* Send buffer to socket */
    int i;
    for (i = 0; i < array_size; ++i)
    {
        send(sock, &buff[i], sizeof(float), 0);
        
        //write(sock, buff[i], strlen(buff[i]));

    }

    /* Releasing resources */
    free(buff);
    return 0;
}