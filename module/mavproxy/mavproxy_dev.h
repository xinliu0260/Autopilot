/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#ifndef MAVPROXY_DEV_H__
#define MAVPROXY_DEV_H__

#include "autopilot.h"

#ifdef __cplusplus
extern "C" {
#endif

rt_err_t mavproxy_dev_init(void);
rt_size_t mavproxy_dev_write(uint8_t chan, const void* buffer, uint32_t len, int32_t timeout);
rt_size_t mavproxy_dev_read(uint8_t chan, void* buffer, uint32_t len, int32_t timeout);
void mavproxy_dev_set_rx_indicate(uint8_t chan, rt_err_t (*rx_ind)(uint32_t size));
rt_err_t mavproxy_dev_set_device(uint8_t chan, const char* dev_name);
rt_device_t mavproxy_dev_get_device(uint8_t chan);

#ifdef __cplusplus
}
#endif

#endif
