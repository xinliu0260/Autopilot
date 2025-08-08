/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

#ifndef ACCEL_H__
#define ACCEL_H__

#include "autopilot.h"

#ifdef __cplusplus
extern "C" {
#endif

/* accel range type */
#define ACCEL_RANGE_2G  2
#define ACCEL_RANGE_4G  4
#define ACCEL_RANGE_8G  8
#define ACCEL_RANGE_16G 16

/* accel device bus type */
#define ACCEL_SPI_BUS_TYPE 1
#define ACCEL_I2C_BUS_TYPE 2

/* default config for accel sensor */
#define ACCEL_CONFIG_DEFAULT                                    \
    {                                                           \
            1000,            /* 1K sample rate */               \
            250,             /* 250Hz internal lpf for accel */ \
            ACCEL_RANGE_16G, /* +-16g */                        \
    }

struct accel_configure {
    rt_uint32_t sample_rate_hz; //采样率
    rt_uint16_t dlpf_freq_hz;   //低通滤波截止频率
    rt_uint32_t acc_range_g;    //量程
};

struct accel_device {
    struct rt_device parent;
    const struct accel_ops* ops;
    struct accel_configure config;
    rt_uint8_t bus_type;
};
typedef struct accel_device* accel_dev_t;

/* accel driver opeations */
struct accel_ops {
    /**
     * @brief accel configuration function (optional)
     * @param dev accel device
     * @param cfg accel configuration
    */
    rt_err_t (*accel_config)(accel_dev_t dev, const struct accel_configure* cfg);
    /**
     * @brief accel control function (optional)
     * @param dev accel device
     * @param cmd operation command
     * @param arg command argument (optional)
    */
    rt_err_t (*accel_control)(accel_dev_t dev, int cmd, void* arg);
    /**
     * @brief accel read data function
     * @param dev accel device
     * @param pos read pos, sent by upper layer. can be used to identify the data type to read, e.g, raw data or scaled data
     * @param data read data buffer. normally it's a pointer to float[3]
     * @param size read data size
    */
    rt_size_t (*accel_read)(accel_dev_t dev, rt_off_t pos, void* data, rt_size_t size);
};

rt_err_t dev_accel_register(accel_dev_t accel, const char* name, rt_uint32_t flag, void* data);

#ifdef __cplusplus
}
#endif

#endif
