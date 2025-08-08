/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#ifndef MPU6000_H__
#define MPU6000_H__

#include "autopilot.h"

#ifdef __cplusplus
extern "C" {
#endif

rt_err_t mpu6000_drv_init(const char* spi_device_name, const char* gyro_device_name, const char* accel_device_name);

#ifdef __cplusplus
}
#endif

#endif
