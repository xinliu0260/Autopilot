/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

#ifndef SENSOR_BARO_H__
#define SENSOR_BARO_H__

#include "autopilot.h"

#include "module/sensor/sensor_hub.h"

#ifdef __cplusplus
extern "C" {
#endif

sensor_baro_t sensor_baro_init(const char* baro_dev_name);
uint8_t sensor_baro_check_ready(sensor_baro_t baro_dev);
rt_err_t sensor_baro_read(sensor_baro_t baro_dev, baro_data_t* baro_data);

#ifdef __cplusplus
}
#endif

#endif