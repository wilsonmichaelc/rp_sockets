#ifndef ACQUIRE_TO_SOCKET_H_
#define ACQUIRE_TO_SOCKET_H_

typedef struct acq_params_tag
{
    int *socket;
    rp_channel_t channel;
    rp_acq_decimation_t sample_rate;
    rp_acq_trig_src_t trigger;
} acq_params;

void *acquire_to_socket(acq_params *parameters);

#endif