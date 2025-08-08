/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#include <string.h>

#include "dev_rc_ppm.h"

/* 16-bit timer */
#define GET_GAP(x, y) (x > y ? (x - y) : (0xFFFF - y + x))

/**
 * @brief update ppm decoder status
 * 
 * @param decoder ppm decoder
 * @param ic_val input capture value
 */
void ppm_update(ppm_decoder_t* decoder, uint32_t ic_val)
{
    static uint16_t temp_val[MAX_PPM_CHANNEL];
    uint16_t gap;

    gap = GET_GAP(ic_val, decoder->last_ic);

    if (gap > (float)decoder->freq_hz / 1000 * 0.9f && gap < (float)decoder->freq_hz / 1000 * 2.1f) {
        /* valid ppm signal should between 1ms to 2ms */
        if (decoder->chan_id < MAX_PPM_CHANNEL) {
            temp_val[decoder->chan_id] = gap;
            decoder->chan_id++;
        }
    } else {
        /* sync signal */
        if (decoder->chan_id >= 1 && decoder->chan_id <= MAX_PPM_CHANNEL) {
            /* get total channel count */
            decoder->total_chan = decoder->chan_id;
        }

        /* if user is reading ppm data while we got a new frame, just drop this frame */
        if (decoder->total_chan && decoder->chan_id == decoder->total_chan && !decoder->ppm_reading) {
            /* reveived all channel data */
            for (uint8_t i = 0; i < decoder->total_chan; i++) {
                decoder->ppm_val[i] = decoder->scale_us * temp_val[i];

                if (decoder->ppm_val[i] < 1000) {
                    decoder->ppm_val[i] = 1000;
                } else if (decoder->ppm_val[i] > 2000) {
                    decoder->ppm_val[i] = 2000;
                }
            }

            decoder->ppm_recvd = 1;
        }

        decoder->chan_id = 0;
    }

    /* update last capture value */
    decoder->last_ic = ic_val;
}

/**
 * @brief initialize ppm decoder
 * 
 * @param decoder ppm decoder
 * @param freq_hz decoder timer frequency in Hz
 * @return rt_err_t RT_EOK for success
 */
rt_err_t ppm_decoder_init(ppm_decoder_t* decoder, uint32_t freq_hz)
{
    decoder->chan_id = 0;
    decoder->total_chan = 0;
    decoder->last_ic = 0;
    decoder->ppm_recvd = 0;
    decoder->ppm_reading = 0;
    decoder->freq_hz = freq_hz;
    decoder->scale_us = 1000000.0f / freq_hz;

    memset(decoder->ppm_val, 0, sizeof(decoder->ppm_val));

    return RT_EOK;
}
