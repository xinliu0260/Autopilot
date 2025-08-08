/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

#ifndef SENSOR_AIRSPEED_H__
#define SENSOR_AIRSPEED_H__

#include "autopilot.h"

#include "module/sensor/sensor_hub.h"

#ifdef __cplusplus
extern "C" {
#endif

sensor_airspeed_t sensor_airspeed_init(const char* airspeed_dev_name);
rt_err_t sensor_airspeed_measure(sensor_airspeed_t airspeed_dev, airspeed_data_t* airspeed_report);

#ifdef __cplusplus
}
#endif

#endif