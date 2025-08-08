/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

 #include "dev_accel.h"

static rt_err_t dev_accel_init(struct rt_device* dev)
{
    rt_err_t ret = RT_EOK;
    accel_dev_t accel;

    RT_ASSERT(dev != RT_NULL);

    accel = (accel_dev_t)dev;

    /* invoke driver configu function */
    if (accel->ops->accel_config) {
        ret = accel->ops->accel_config(accel, &accel->config);
    }

    return ret;
}

static rt_ssize_t dev_accel_read(struct rt_device* dev,
                                rt_off_t pos,
                                void* buffer,
                                rt_size_t size)
{
    rt_ssize_t rb = 0;
    accel_dev_t accel;

    RT_ASSERT(dev != RT_NULL);

    accel = (accel_dev_t)dev;

    /* invoke driver read function to read data */
    if (accel->ops->accel_read && size) {
        rb = (rt_ssize_t)accel->ops->accel_read(accel, pos, buffer, size);
    }

    return rb;
}

static rt_err_t dev_accel_control(struct rt_device* dev,
                                  int cmd,
                                  void* args)
{
    rt_err_t ret = RT_EOK;
    accel_dev_t accel;

    RT_ASSERT(dev != RT_NULL);

    accel = (accel_dev_t)dev;

    /* invoke driver control function to handle command */
    if (accel->ops->accel_control) {
        ret = accel->ops->accel_control(accel, cmd, args);
    }

    return ret;
}

/**
 * @brief register an accel device
 * 
 * @param accel accel device
 * @param name device name
 * @param flag device flag
 * @param data device data
 * @return rt_err_t RT_EOK for success
 */
rt_err_t dev_accel_register(accel_dev_t accel, const char* name, rt_uint32_t flag, void* data)
{
    rt_err_t ret;
    struct rt_device* device;

    RT_ASSERT(accel != RT_NULL);

    device = &(accel->parent);

    device->type = (accel->bus_type == ACCEL_SPI_BUS_TYPE) ? RT_Device_Class_SPIDevice : RT_Device_Class_I2CBUS;
    device->ref_count = 0;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;

    device->init = dev_accel_init;
    device->open = RT_NULL;
    device->close = RT_NULL;
    device->read = dev_accel_read;
    device->write = RT_NULL;
    device->control = dev_accel_control;
    device->user_data = data;

    /* register device to system */
    ret = rt_device_register(device, name, flag);

    return ret;
}