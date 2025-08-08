/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#ifndef MAVLINK_RTCM_H__
#define MAVLINK_RTCM_H__

#include "autopilot.h"
#include <mavlink.h>

#ifdef __cplusplus
extern "C" {
#endif

void handle_gps_rtcm_data(const mavlink_message_t* msg);
rt_err_t mavlink_rtcm_device_init(void);

#ifdef __cplusplus
}
#endif

#endif
