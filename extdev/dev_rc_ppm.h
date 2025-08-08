/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#ifndef PPM_H__
#define PPM_H__

#include "autopilot.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_PPM_CHANNEL 10

typedef struct {
    uint8_t chan_id;
    uint8_t total_chan; /* total ppm channel number */
    uint16_t last_ic;
    uint16_t ppm_recvd;
    uint16_t ppm_reading;
    uint32_t freq_hz;
    float scale_us;
    uint16_t ppm_val[MAX_PPM_CHANNEL]; /* ppm raw value in microseconds */
} ppm_decoder_t;

rt_inline void ppm_lock(ppm_decoder_t* decoder)
{
    decoder->ppm_reading = 1;
}

rt_inline void ppm_unlock(ppm_decoder_t* decoder)
{
    decoder->ppm_reading = 0;
}

rt_inline uint16_t ppm_data_ready(ppm_decoder_t* decoder)
{
    return decoder->ppm_recvd;
}

rt_inline void ppm_data_clear(ppm_decoder_t* decoder)
{
    decoder->ppm_recvd = 0;
}

void ppm_update(ppm_decoder_t* decoder, uint32_t ic_val);
rt_err_t ppm_decoder_init(ppm_decoder_t* decoder, uint32_t freq_hz);

#ifdef __cplusplus
}
#endif

#endif
