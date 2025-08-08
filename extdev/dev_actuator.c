/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

 
#include "dev_actuator.h"
#include "module/math/ap_math.h"
static rt_err_t dev_actuator_init(struct rt_device* dev)
{
    actuator_dev_t act = (actuator_dev_t)dev;

    RT_ASSERT(dev != RT_NULL);

    act->suspend = RT_FALSE;

    /* apply default configuration */
    if (act->ops->act_config) {
        return act->ops->act_config(act, &act->config);
    }

    return RT_EOK;
}

static rt_err_t dev_actuator_control(struct rt_device* dev, int cmd, void* args)
{
    actuator_dev_t act = (actuator_dev_t)dev;

    RT_ASSERT(dev != RT_NULL);

    switch (cmd) {
    case RT_DEVICE_CTRL_SUSPEND:
        act->suspend = RT_TRUE;
        break;

    case RT_DEVICE_CTRL_RESUME:
        act->suspend = RT_FALSE;
        break;

    default:
        if (act->ops->act_control) {
            return act->ops->act_control(act, cmd, args);
        }
    }

    return RT_EOK;
}

static rt_err_t dev_actuator_open(rt_device_t dev, rt_uint16_t oflag)
{
    return dev_actuator_control(dev, ACT_CMD_CHANNEL_ENABLE, NULL);
}

static rt_err_t dev_actuator_close(rt_device_t dev)
{
    return dev_actuator_control(dev, ACT_CMD_CHANNEL_DISABLE, NULL);
}

static rt_ssize_t dev_actuator_read(struct rt_device* dev, rt_off_t pos, void* buffer, rt_size_t size)
{
    actuator_dev_t act = (actuator_dev_t)dev;
    rt_ssize_t rb = 0;

    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(buffer != RT_NULL);

    if (pos == 0) {
        return 0;
    }

    /* apply channel mask */
    pos = pos & act->chan_mask;

    if (act->ops->act_read) {
        rb = (rt_ssize_t)act->ops->act_read(act, pos, buffer, size);
    }

    return rb;
}

static rt_ssize_t dev_actuator_write(rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size)
{
    actuator_dev_t act = (actuator_dev_t)dev;
    rt_ssize_t wb = 0;
    uint16_t chan_val[16];
    uint8_t index = 0;
    uint16_t* val_ptr = (uint16_t*)buffer;

    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(buffer != RT_NULL);

    if (act->suspend == RT_TRUE || pos == 0) {
        return 0;
    }

    /* apply channel mask */
    pos = pos & act->chan_mask;
    /* saturate channel value */
    for (uint8_t i = 0; i < 16; i++) {
        if (pos & (1 << i)) {
            chan_val[index] = constrain_uint16(val_ptr[index], act->range[0], act->range[1]);
            index++;
        }
    }

    if (act->ops->act_write) {
        wb = (rt_ssize_t)act->ops->act_write(act, pos, chan_val, size);
    }

    return wb;
}

/**
 * @brief register an actuator device
 * 
 * @param dev actuator device
 * @param name device name
 * @param flag device flag
 * @param data device data
 * @return rt_err_t RT_EOK for success
 */
rt_err_t dev_actuator_register(actuator_dev_t dev, const char* name, rt_uint32_t flag, void* data)
{
    RT_ASSERT(dev != RT_NULL);

    struct rt_device* device;

    device = &(dev->parent);

    device->type = RT_Device_Class_Miscellaneous;
    device->ref_count = 0;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;

    device->init = dev_actuator_init;
    device->open = dev_actuator_open;
    device->close = dev_actuator_close;
    device->read = dev_actuator_read;
    device->write = dev_actuator_write;
    device->control = dev_actuator_control;
    device->user_data = data;

    /* register device to system */
    return rt_device_register(device, name, flag);
}
