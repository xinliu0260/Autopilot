/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

#include "module/pmu/power_manager.h"
#include "module/ipc/uMCN.h"
#include "module/param/param.h"

#define LOG_TAG "pm"
#define LOG_LVL DBG_INFO
#include "ulog.h"

MCN_DEFINE(bat_status, sizeof(struct battery_status));

static rt_device_t adc_dev;

static int echo_battery_status(void* parameter)
{
    struct battery_status bat_status;

    if (mcn_copy_from_hub((McnHub*)parameter, &bat_status) != RT_EOK) {
        return -1;
    }

    LOG_I("voltage:%5d mV, current:%5d mA, remaining:%3u%%\n",
                   bat_status.battery_voltage,
                   bat_status.battery_current,
                   bat_status.battery_remaining);

    return 0;
}

rt_err_t pmu_poll_battery_status(void)
{
    struct battery_status bat_status;
    uint32_t value;

    if (adc_dev == NULL) {
        return RT_EEMPTY;
    }

    if (rt_device_read(adc_dev, 0, &value, sizeof(value)) != sizeof(value)) {
        return RT_ERROR;
    }
    bat_status.battery_voltage = value * PARAM_GET_FLOAT(CALIB, BAT_V_DIV); /* millivolt */

    if (rt_device_read(adc_dev, 1, &value, sizeof(value)) != sizeof(value)) {
        return RT_ERROR;
    }
    bat_status.battery_current = value; /* millicurrent */
    bat_status.battery_remaining = 0;

    /* publish battery 0 status */
    if (mcn_publish(MCN_HUB(bat_status), &bat_status) != RT_EOK) {
        return RT_ERROR;
    }

    return RT_EOK;
}

rt_err_t pmu_init(void)
{
    RT_CHECK(mcn_advertise(MCN_HUB(bat_status), echo_battery_status));

    adc_dev = rt_device_find("adc0");
    RT_ASSERT(adc_dev != NULL);

    RT_CHECK(rt_device_open(adc_dev, RT_DEVICE_FLAG_RDONLY));

    return RT_EOK;
}
