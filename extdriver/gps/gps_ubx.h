/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#ifndef GPS_MBX_H__
#define GPS_MBX_H__

#include "autopilot.h"

#ifdef __cplusplus
extern "C" {
#endif

rt_err_t gps_ubx_init(const char* serial_device_name, const char* gps_device_name);

#ifdef __cplusplus
}
#endif

#endif /* GPS_UBX_H__ */
