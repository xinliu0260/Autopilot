/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

#ifndef SENSOR_IMU_H__
#define SENSOR_IMU_H__

#include "autopilot.h"

#include "module/sensor/sensor_hub.h"

#ifdef __cplusplus
extern "C" {
#endif

sensor_imu_t sensor_imu_init(const char* gyr_dev_name, const char* acc_dev_name);
/* gyro api */
rt_err_t sensor_gyr_measure(sensor_imu_t imu_dev, float buffer[3]);
void sensor_gyr_set_rotation(sensor_imu_t imu_dev, const float rotation[9]);
void sensor_gyr_set_offset(sensor_imu_t imu_dev, const float offset[3]);
void sensor_gyr_correct(sensor_imu_t imu_dev, const float src[3], float dst[3]);
/* accel api */
rt_err_t sensor_acc_measure(sensor_imu_t imu_dev, float buffer[3]);
void sensor_acc_set_rotation(sensor_imu_t imu_dev, const float rotation[9]);
void sensor_acc_set_offset(sensor_imu_t imu_dev, const float offset[3]);
void sensor_acc_correct(sensor_imu_t imu_dev, const float src[3], float dst[3]);

#ifdef __cplusplus
}
#endif

#endif
