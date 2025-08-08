/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


 
#ifndef AIRSPEED_H__
#define AIRSPEED_H__

#include "autopilot.h"

#ifdef __cplusplus
extern "C" {
#endif

/* mag device bus type */
#define AIRSPEED_SPI_BUS_TYPE 1
#define AIRSPEED_I2C_BUS_TYPE 2

struct airspeed_device {
    struct rt_device parent;
    const struct airspeed_ops* ops;
    rt_uint8_t bus_type;
};
typedef struct airspeed_device* airspeed_dev_t;

/* airspeed driver opeations */
struct airspeed_ops {
    /**
     * @brief airspeed control function (optional)
     * @param dev airspeed device
     * @param cmd operation command
     * @param arg command arguments
     */
    rt_err_t (*airspeed_control)(airspeed_dev_t dev, int cmd, void* arg);
    /**
     * @brief airspeed read function
     * @param dev airspeed device
     * @param pos read position
     * @param data read data buffer
     * @param size read data size
     */
    rt_size_t (*airspeed_read)(airspeed_dev_t dev, rt_off_t pos, void* data, rt_size_t size);
};

rt_err_t dev_airspeed_register(airspeed_dev_t dev, const char* name, rt_uint32_t flag, void* data);

#ifdef __cplusplus
}
#endif

#endif
