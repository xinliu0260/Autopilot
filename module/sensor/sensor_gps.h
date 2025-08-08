/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

#ifndef SENSOR_GPS_H__
#define SENSOR_GPS_H__

#include "autopilot.h"

#include "module/sensor/sensor_hub.h"

#ifdef __cplusplus
extern "C" {
#endif

sensor_gps_t sensor_gps_init(const char* gps_dev_name);
rt_err_t sensor_gps_read(sensor_gps_t gps_dev, gps_data_t* gps_data);
uint8_t sensor_gps_check_ready(sensor_gps_t gps_dev);

#ifdef __cplusplus
}
#endif

#endif