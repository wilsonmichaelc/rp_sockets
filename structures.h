#ifndef STRUCTURES_H_
#define STRUCTURES_H_

struct acq_params_tag {
    int socket;
    rp_channel_t channel;
    rp_acq_decimation_t decimation;
    rp_acq_trig_src_t trigger;
} acq_params;

#endif