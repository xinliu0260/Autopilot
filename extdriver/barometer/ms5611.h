/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

#ifndef MS5611_H__
#define MS5611_H__

#include "autopilot.h"

#ifdef __cplusplus
extern "C" {
#endif

rt_err_t drv_ms5611_init(const char* spi_device_name, const char* baro_device_name);

#ifdef __cplusplus
}
#endif

#endif
