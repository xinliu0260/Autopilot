/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

#include "autopilot.h"

#define LOG_TAG "mav_dev"
#define LOG_LVL DBG_INFO
#include "ulog.h"


#define MAVPROXY_DEV_CHAN_NUM 2

struct mavdev {
    rt_device_t dev;
    rt_err_t (*tx_done)(rt_device_t dev, void* buffer);
    rt_err_t (*rx_ind)(rt_device_t dev, rt_size_t size);
    rt_err_t (*mav_rx_ind)(uint32_t size);
    struct rt_completion tx_cplt, rx_cplt;
};

static rt_err_t mavdev_chan0_tx_done(rt_device_t dev, void* buffer);
static rt_err_t mavdev_chan0_rx_ind(rt_device_t dev, rt_size_t size);
static rt_err_t mavdev_chan1_tx_done(rt_device_t dev, void* buffer);
static rt_err_t mavdev_chan1_rx_ind(rt_device_t dev, rt_size_t size);

struct mavdev mavdev_list[MAVPROXY_DEV_CHAN_NUM] = {
    { .tx_done = mavdev_chan0_tx_done, .rx_ind = mavdev_chan0_rx_ind },
    { .tx_done = mavdev_chan1_tx_done, .rx_ind = mavdev_chan1_rx_ind },
};

static rt_err_t mavdev_chan0_tx_done(rt_device_t dev, void* buffer)
{
    rt_completion_done(&mavdev_list[0].tx_cplt);
    return RT_EOK;
}

static rt_err_t mavdev_chan0_rx_ind(rt_device_t dev, rt_size_t size)
{
    rt_completion_done(&mavdev_list[0].rx_cplt);

    if (mavdev_list[0].mav_rx_ind) {
        mavdev_list[0].mav_rx_ind(size);
    }

    return RT_EOK;
}

static rt_err_t mavdev_chan1_tx_done(rt_device_t dev, void* buffer)
{
    rt_completion_done(&mavdev_list[1].tx_cplt);

    return RT_EOK;
}

static rt_err_t mavdev_chan1_rx_ind(rt_device_t dev, rt_size_t size)
{
    rt_completion_done(&mavdev_list[1].rx_cplt);

    if (mavdev_list[1].mav_rx_ind) {
        mavdev_list[1].mav_rx_ind(size);
    }

    return RT_EOK;
}

rt_size_t mavproxy_dev_write(uint8_t chan, const void* buffer, uint32_t len, int32_t timeout)
{
    rt_size_t size;

    if (mavdev_list[chan].dev == NULL) {
        /* mavproxy device not initialized */
        return 0;
    }
    /* write data to device */
    size = rt_device_write(mavdev_list[chan].dev, 0, buffer, len);
	if(size<len){
		/* wait write complete (synchronized write) */
        if (rt_completion_wait(&mavdev_list[chan].tx_cplt, timeout) != RT_EOK) {
			size = rt_device_write(mavdev_list[chan].dev, 0, buffer+size, len-size);
			if(size == len-size){
				return len;
			}
			else{
				LOG_W("mavproxy tx data length is over\r\n");
				return 0;
			}
        }
	}
	else{
		return size;
	}
//    if (size > 0) {
//        /* wait write complete (synchronized write) */
//        if (rt_completion_wait(&mavdev_list[chan].tx_cplt, timeout) != RT_EOK) {
//            return 0;
//        }
//    }

    return size;
}

rt_size_t mavproxy_dev_read(uint8_t chan, void* buffer, uint32_t len, int32_t timeout)
{
    rt_size_t cnt = 0;

    if (mavdev_list[chan].dev == NULL) {
        /* mavproxy device not initialized */
        return 0;
    }

    /* try to read data */
    cnt = rt_device_read(mavdev_list[chan].dev, 0, buffer, len);

    /* sync mode */
    if (timeout != 0) {
        uint32_t time_start, elapse_time;
        /* if not enough data reveived, wait it */
        while (cnt < len) {
            time_start = rt_tick_get_millisecond();
            /* wait until something reveived (synchronized read) */
            if (rt_completion_wait(&mavdev_list[chan].rx_cplt, timeout) != RT_EOK) {
                break;
            }
            if (timeout != RT_WAITING_FOREVER) {
                elapse_time = rt_tick_get_millisecond() - time_start;
                timeout -= elapse_time;
                if (timeout <= 0) {
                    /* timeout */
                    break;
                }
            }
            /* read rest data */
            cnt += rt_device_read(mavdev_list[chan].dev, 0, (void*)((uint32_t)buffer + cnt), len - cnt);
        }
    }

    return cnt;
}

rt_err_t mavproxy_dev_set_rx_indicate(uint8_t chan, rt_err_t (*rx_ind)(uint32_t size))
{
    mavdev_list[chan].mav_rx_ind = rx_ind;

    return RT_EOK;
}

rt_err_t mavproxy_dev_set_device(uint8_t chan, const char* dev_name)
{
    rt_device_t new_dev;

    new_dev = rt_device_find(dev_name);

    if (new_dev == RT_NULL) {
        return RT_EEMPTY;
    }

    if (new_dev != mavdev_list[chan].dev) {
        rt_uint16_t flag = RT_DEVICE_OFLAG_RDWR | RT_SERIAL_RX_NON_BLOCKING | RT_SERIAL_TX_NON_BLOCKING;

        rt_err_t err = rt_device_open(new_dev, flag);
        if (err != RT_EOK) {
            return RT_ERROR;
        }
        mavdev_list[chan].dev = new_dev;
    }

    /* set callback functions */
    rt_device_set_tx_complete(new_dev, mavdev_list[chan].tx_done);
    rt_device_set_rx_indicate(new_dev, mavdev_list[chan].rx_ind);

    return RT_EOK;
}

rt_device_t mavproxy_dev_get_device(uint8_t chan)
{
    return mavdev_list[chan].dev;
}

rt_err_t mavproxy_dev_init(void)
{
    for (uint8_t chan = 0; chan < MAVPROXY_DEV_CHAN_NUM; chan++) {
        rt_completion_init(&mavdev_list[chan].tx_cplt);
        rt_completion_init(&mavdev_list[chan].rx_cplt);
    }

    return RT_EOK;
}
