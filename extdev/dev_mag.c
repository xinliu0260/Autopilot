/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#include "dev_mag.h"

static rt_err_t dev_mag_init(struct rt_device* dev)
{
    rt_err_t ret = RT_EOK;
    mag_dev_t mag;

    RT_ASSERT(dev != RT_NULL);

    mag = (mag_dev_t)dev;

    /* apply configuration */
    if (mag->ops->mag_config) {
        ret = mag->ops->mag_config(mag, &mag->config);
    }

    return ret;
}

static rt_ssize_t dev_mag_read(struct rt_device* dev,
                              rt_off_t pos,
                              void* buffer,
                              rt_size_t size)
{
    rt_ssize_t rb = 0;
    mag_dev_t mag;

    RT_ASSERT(dev != RT_NULL);

    mag = (mag_dev_t)dev;

    if (mag->ops->mag_read && size) {
        rb = (rt_ssize_t)mag->ops->mag_read(mag, pos, buffer, size);
    }

    return rb;
}

static rt_err_t dev_mag_control(struct rt_device* dev,
                                int cmd,
                                void* args)
{
    rt_err_t ret = RT_EOK;
    mag_dev_t mag;

    RT_ASSERT(dev != RT_NULL);

    mag = (mag_dev_t)dev;

    if (mag->ops->mag_control) {
        ret = mag->ops->mag_control(mag, cmd, args);
    }

    return ret;
}

/**
 * @brief register a mag device
 * 
 * @param mag mag device
 * @param name device name
 * @param flag device flag
 * @param data device data
 * @return rt_err_t RT_EOK for success
 */
rt_err_t dev_mag_register(mag_dev_t mag, const char* name, rt_uint32_t flag, void* data)
{
    rt_err_t ret;
    struct rt_device* device;

    RT_ASSERT(mag != RT_NULL);

    device = &(mag->parent);

    device->type = (mag->bus_type == MAG_SPI_BUS_TYPE) ? RT_Device_Class_SPIDevice : RT_Device_Class_I2CBUS;
    device->ref_count = 0;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;

    device->init = dev_mag_init;
    device->open = RT_NULL;
    device->close = RT_NULL;
    device->read = dev_mag_read;
    device->write = RT_NULL;
    device->control = dev_mag_control;
    device->user_data = data;

    /* register device to system */
    ret = rt_device_register(device, name, flag);

    return ret;
}