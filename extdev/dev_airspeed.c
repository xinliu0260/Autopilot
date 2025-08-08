/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#include "dev_airspeed.h"

static rt_ssize_t dev_airspeed_read(struct rt_device* dev,
                                   rt_off_t pos,
                                   void* buffer,
                                   rt_size_t size)
{
    rt_ssize_t rb = 0;
    airspeed_dev_t airspeed;

    RT_ASSERT(dev != RT_NULL);

    airspeed = (airspeed_dev_t)dev;

    if (airspeed->ops->airspeed_read && size) {
        rb = airspeed->ops->airspeed_read(airspeed, pos, buffer, size);
    }

    return rb;
}

static rt_err_t dev_airspeed_control(struct rt_device* dev,
                                     int cmd,
                                     void* args)
{
    rt_err_t ret = RT_EOK;
    airspeed_dev_t airspeed;

    RT_ASSERT(dev != RT_NULL);

    airspeed = (airspeed_dev_t)dev;

    if (airspeed->ops->airspeed_control) {
        ret = airspeed->ops->airspeed_control(airspeed, cmd, args);
    }

    return ret;
}

/**
 * @brief register an airspeed device
 * 
 * @param dev airspeed device
 * @param name device name
 * @param flag device flag
 * @param data device data
 * @return rt_err_t RT_EOK for success
 */
rt_err_t dev_airspeed_register(airspeed_dev_t dev, const char* name, rt_uint32_t flag, void* data)
{
    rt_err_t ret;
    struct rt_device* device;

    RT_ASSERT(dev != RT_NULL);

    device = &(dev->parent);

    device->type = (dev->bus_type == AIRSPEED_SPI_BUS_TYPE) ? RT_Device_Class_SPIDevice : RT_Device_Class_I2CBUS;
    device->ref_count = 0;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;

    device->init = RT_NULL;
    device->open = RT_NULL;
    device->close = RT_NULL;
    device->read = dev_airspeed_read;
    device->write = RT_NULL;
    device->control = dev_airspeed_control;
    device->user_data = data;

    /* register device to system */
    ret = rt_device_register(device, name, flag);

    return ret;
}