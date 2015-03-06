#ifndef STR_TO_ENUM_H_
#define STR_TO_ENUM_H_

rp_channel_t getChannel(char channel[]);
rp_acq_trig_src_t getTrigger(char trigger[]);
rp_acq_sampling_rate_t getSampleRate(char rate[]);

#endif