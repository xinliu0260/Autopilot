/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#include "module/sensor/sensor_airspeed.h"
#include "module/param/param.h"
#include "extdev/dev_airspeed.h"


static float diff_press_offset;

/**
 * @brief Measure airspeed data
 * 
 * @param airspeed_dev Airspeed sensor device
 * @param airspeed_report Airspeed data buffer
 * @return rt_err_t RT_EOK for success
 */
rt_err_t sensor_airspeed_measure(sensor_airspeed_t airspeed_dev, airspeed_data_t* airspeed_report)
{
    rt_size_t r_size;
    float report[2];

    RT_ASSERT(airspeed_dev != NULL);

    r_size = rt_device_read(airspeed_dev->dev, 0, report, 8);

    airspeed_report->timestamp_ms = rt_tick_get_millisecond();
    airspeed_report->diff_pressure_pa = report[0] - diff_press_offset;
    airspeed_report->temperature_deg = report[1];

    return r_size == 8 ? RT_EOK : RT_ERROR;
}

/**
 * @brief Initialize airspeed sensor device
 * 
 * @param baro_dev_name Airspeed device name
 * @return sensor_airspeed_t Airspeed sensor device
 */
sensor_airspeed_t sensor_airspeed_init(const char* airspeed_dev_name)
{
    rt_device_t dev = rt_device_find(airspeed_dev_name);

    if (dev == NULL) {
        /* do not find airspeed device */
        return NULL;
    }

    sensor_airspeed_t airspeed_dev = (sensor_airspeed_t)rt_malloc(sizeof(struct sensor_airspeed));
    RT_ASSERT(airspeed_dev != NULL);

    /* set airspeed device */
    airspeed_dev->dev = dev;

    /* open device */
    RT_CHECK(rt_device_open(airspeed_dev->dev, RT_DEVICE_OFLAG_RDWR));

    RT_CHECK(param_link_variable(PARAM_GET(CALIB, DIFF_PRESS_OFFSET), &diff_press_offset));

    return airspeed_dev;
}