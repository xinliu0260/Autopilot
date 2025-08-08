/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

#include "dev_barometer.h"

static rt_ssize_t dev_baro_read(struct rt_device* dev,
                               rt_off_t pos,
                               void* buffer,
                               rt_size_t size)
{
    rt_ssize_t rb = 0;
    baro_dev_t baro;

    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(buffer != RT_NULL);

    baro = (baro_dev_t)dev;

    if (pos == BARO_RD_REPORT && baro->ops->baro_read) {
        rb = (rt_ssize_t)baro->ops->baro_read(baro, buffer);
    }

    return rb;
}

static rt_err_t dev_baro_control(struct rt_device* dev,
                                 int cmd,
                                 void* args)
{
    rt_err_t ret = RT_EOK;
    baro_dev_t baro;

    RT_ASSERT(dev != RT_NULL);

    baro = (baro_dev_t)dev;

    if (baro->ops->baro_control) {
        ret = baro->ops->baro_control(baro, cmd, args);
    }

    return ret;
}

/**
 * @brief register a barometer device
 * 
 * @param baro barometer device
 * @param name device name
 * @param flag device flag
 * @param data device data
 * @return rt_err_t RT_EOK for success
 */
rt_err_t dev_baro_register(baro_dev_t baro, const char* name, rt_uint32_t flag, void* data)
{
    rt_err_t ret;
    struct rt_device* device;

    RT_ASSERT(baro != RT_NULL);

    device = &(baro->parent);

    device->type = RT_Device_Class_SPIDevice;
    device->ref_count = 0;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;

    device->init = RT_NULL;
    device->open = RT_NULL;
    device->close = RT_NULL;
    device->read = dev_baro_read;
    device->write = RT_NULL;
    device->control = dev_baro_control;
    device->user_data = data;

    /* register device to system */
    ret = rt_device_register(device, name, flag);

    return ret;
}