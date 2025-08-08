/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

#ifndef ACTUATOR_H__
#define ACTUATOR_H__

#include "autopilot.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ACT_CMD_CHANNEL_ENABLE  0x20
#define ACT_CMD_CHANNEL_DISABLE 0x21
#define ACT_CMD_SET_PROTOCOL    0x22

enum {
    ACT_PROTOCOL_PWM = 1,
    ACT_PROTOCOL_DSHOT = 2
};

struct pwm_drv_configure {
    rt_uint16_t pwm_freq;
	rt_uint16_t pulse_time_begin;
	rt_uint16_t pulse_time_end;
	float pulse_factor;
};

struct dshot_drv_configure {
    rt_uint16_t speed;   /* 600, 300, 150 */
    rt_bool_t telem_req; /* telemetry request */
};

struct actuator_configure {
    rt_uint8_t protocol;                     /* actuator protocol: pwm or dshot */
    rt_uint16_t chan_num;                    /* actuator channel number */
    struct pwm_drv_configure pwm_config;     /* pwm configuration */
    struct dshot_drv_configure dshot_config; /* dshot configuration */
};

struct actuator_device {
    struct rt_device parent;
    const struct actuator_ops* ops;
    struct actuator_configure config;
    rt_uint16_t chan_mask; /* channel mask */
    rt_uint16_t range[2];  /* [min, max] value */
    rt_bool_t suspend;     /* suspend device */
};
typedef struct actuator_device* actuator_dev_t;

struct actuator_ops {
    /**
     * @brief actuator config function (optional)
     * @param dev actuator device
     * @param cfg actuator configuration
     */
    rt_err_t (*act_config)(actuator_dev_t dev, const struct actuator_configure* cfg);
    /**
     * @brief actuator control function (optional)
     * @param dev actuator device
     * @param cnd operation command
     * @param arg command arguments
     */
    rt_err_t (*act_control)(actuator_dev_t dev, int cmd, void* arg);
    /**
     * @brief actuator read function (optional)
     * @param dev actuator device
     * @param chan_sel selected channel to read with bit mask
     * @param chan_val selected channel read buffer
     * @param size read size
     */
    rt_size_t (*act_read)(actuator_dev_t dev, rt_uint16_t chan_sel, rt_uint16_t* chan_val, rt_size_t size);
    /**
     * @brief actuator write function
     * @param dev actuator device
     * @param chan_sel selected channel to write with bit mask
     * @param chan_val selected channel write buffer
     * @param size write size
     */
    rt_size_t (*act_write)(actuator_dev_t dev, rt_uint16_t chan_sel, const rt_uint16_t* chan_val, rt_size_t size);
};

rt_err_t dev_actuator_register(actuator_dev_t dev, const char* name, rt_uint32_t flag, void* data);

#ifdef __cplusplus
}
#endif

#endif /* ACTUATOR_H__ */