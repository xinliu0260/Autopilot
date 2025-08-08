/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

#include "autopilot.h"

#include "rtdevice.h"
#include "module/mavproxy/mavproxy.h"
#include "module/mavproxy/mavproxy_config.h"
#include "module/toml/toml.h"


#define TOML_DBG_E(...)                    toml_debug("Mavproxy", "E", __VA_ARGS__)
#define TOML_DBG_W(...)                    toml_debug("Mavproxy", "W", __VA_ARGS__)

#define MAVPROXY_MAX_DEVICE_NUM            3
#define MATCH(a, b)                        (strcmp(a, b) == 0)
#define DEVICE_LIST(_chan)                 mavproxy_device_list[_chan]
#define DEVICE_NUM(_chan)                  mavproxy_device_num[_chan]
#define DEVICE_TYPE_IS(_chan, _idx, _name) MATCH(DEVICE_LIST(_chan)[_idx].type, #_name)
#define FIND_DEVICE(_chan, _idx)           rt_device_find(DEVICE_LIST(_chan)[_idx].name)
#define MAVPROXY_SERIAL_BAUDRATE           230400

mavproxy_device_info mavproxy_device_list[MAVPROXY_CHAN_NUM][MAVPROXY_MAX_DEVICE_NUM];
uint8_t mavproxy_device_num[MAVPROXY_CHAN_NUM];

mavproxy_device_info mavproxy_rtcm_device_list[MAVPROXY_MAX_DEVICE_NUM];
uint8_t mavproxy_rtcm_device_num;

static void __handle_device_msg(rt_device_t dev, void* msg)
{
    // device_status status = *((device_status*)msg);

    // for (uint8_t chan = 0; chan < MAVPROXY_CHAN_NUM; chan++) {
    //     for (uint8_t idx = 0; idx < DEVICE_NUM(chan); idx++) {
    //         if (rt_device_find(DEVICE_LIST(chan)[idx].name) == dev) {
    //             if (status == DEVICE_STATUS_CONNECT) {
    //                 /* if usb conncted, switch to usb channel */
    //                 mavproxy_set_device(chan, idx);
    //             } else if (status == DEVICE_STAUTS_DISCONNECT) {
    //                 /* if usb disconncted, switch to default channel */
    //                 mavproxy_set_device(chan, 0);
    //             }
    //             break;
    //         }
    //     }
    // }
}

/**
 * mavproxy device toml definition example:
 * 	chan = 0\n
 *	type = "serial"\n
 *	name = "serial2"\n
 *	baudrate = 230400\n
 */
static rt_err_t mavproxy_parse_device(const toml_table_t* curtab)
{
    rt_err_t err = RT_EOK;
    int i;
    const char* key;
    uint8_t chan;
    uint8_t idx;

    /* get channel */
    int64_t ival;
    if (toml_int_in(curtab, "chan", &ival) == 0) {
        chan = (uint8_t)ival;

        if (chan >= MAVPROXY_CHAN_NUM) {
            TOML_DBG_E("invalid channel:%d\n", chan);
            return RT_EINVAL;
        }

        if (DEVICE_NUM(chan) >= MAVPROXY_MAX_DEVICE_NUM) {
            TOML_DBG_W("too many devices for chan:%d\n", chan);
            return RT_EINVAL;
        }

        idx = DEVICE_NUM(chan);
    } else {
        TOML_DBG_E("fail to parse chan value\n");
        return RT_ERROR;
    }

    /* get device type */
    if (toml_string_in(curtab, "type", &DEVICE_LIST(chan)[idx].type) == 0) {
        if (DEVICE_TYPE_IS(chan, idx, serial)) {
            mavproxy_serial_dev_config serial_default_config = {
                .baudrate = MAVPROXY_SERIAL_BAUDRATE
            };
            (DEVICE_LIST(chan)[idx].config) = rt_malloc(sizeof(mavproxy_serial_dev_config));

            /* set default value */
            if (DEVICE_LIST(chan)[idx].config) {
                *(mavproxy_serial_dev_config*)DEVICE_LIST(chan)[idx].config = serial_default_config;
            } else {
                TOML_DBG_E("malloc fail\n");
                err = RT_ERROR;
            }
        } else if (DEVICE_TYPE_IS(chan, idx, usb)) {
            /* do nothing */
        } else if (DEVICE_TYPE_IS(chan, idx, bb_com)) {
            /* do nothing */
        } else {
            TOML_DBG_E("unknown device type: %s\n", DEVICE_LIST(chan)[idx].type);
            err = RT_ERROR;
        }
    } else {
        TOML_DBG_E("fail to parse type value\n");
        return RT_ERROR;
    }

    if (toml_string_in(curtab, "name", &DEVICE_LIST(chan)[idx].name) != 0) {
        TOML_DBG_E("fail to parse name value\n");
        return RT_ERROR;
    }

    /* traverse keys in table 遍历表中的所有键*/
    //key = toml_key_in(curtab, i)把表中的键名赋值给Key,如果取到空则该表达式值为0，因此该循环会一直持续到取到空值为止
    for (i = 0; 0 != (key = toml_key_in(curtab, i)); i++) {
        if (MATCH(key, "chan") || MATCH(key, "type") || MATCH(key, "name")) {
            /* already handled */
            continue;
        }

        if (DEVICE_TYPE_IS(chan, idx, serial)) {
            mavproxy_serial_dev_config* config = (mavproxy_serial_dev_config*)DEVICE_LIST(chan)[idx].config;
            if (MATCH(key, "baudrate")) {
                int64_t ival;
                if (toml_int_in(curtab, key, &ival) == 0) {
                    config->baudrate = (uint32_t)ival;
                } else {
                    TOML_DBG_W("fail to parse baudrate value\n");
                    continue;
                }
            } else {
                TOML_DBG_W("unknown config key: %s\n", key);
                continue;
            }
        } else if (DEVICE_TYPE_IS(chan, idx, usb)) {
            if (MATCH(key, "auto-switch")) {
                int bval;
                if (toml_bool_in(curtab, key, &bval) == 0) {
                    if (bval) {
                        /* if auto-switch is true, register devmq to monitor device status */
                        // if (devmq_register(FIND_DEVICE(chan, idx), __handle_device_msg) != RT_EOK) {
                        //     TOML_DBG_W("fail to register %s message queue\n", DEVICE_LIST(chan)[idx].name);
                        // }
                    }
                } else {
                    TOML_DBG_E("fail to parse auto-switch value\n");
                    continue;
                }
            } else {
                TOML_DBG_E("unknown config key: %s\n", key);
                continue;
            }
        } else {
            TOML_DBG_W("unknown device type:%s \n", DEVICE_LIST(chan)[idx].type);
            continue;
        }
    }

    if (err == RT_EOK) {
        DEVICE_NUM(chan) += 1;
    }

    return err;
}

static rt_err_t mavproxy_parse_devices(const toml_array_t* array)
{
    int i;
    toml_table_t* curtab;
    rt_err_t err = RT_EOK;

    for (i = 0; 0 != (curtab = toml_table_at(array, i)); i++) {
        err = mavproxy_parse_device(curtab);

        if (err != RT_EOK) {
            TOML_DBG_E("device parse fail: %d\n", err);
            return err;
        }
    }

    return err;
}

static rt_err_t mavproxy_parse_rtcm_device(const toml_table_t* curtab)
{
    rt_err_t err = RT_EOK;
    int i;
    const char* key;
    uint8_t idx = mavproxy_rtcm_device_num;

    if (idx >= MAVPROXY_MAX_DEVICE_NUM) {
        TOML_DBG_W("too many rtcm devices\n");
        return RT_EINVAL;
    }

    /* get device type */
    if (toml_string_in(curtab, "type", &mavproxy_rtcm_device_list[idx].type) == 0) {
        if (MATCH(mavproxy_rtcm_device_list[idx].type, "serial")) {
            mavproxy_serial_dev_config serial_default_config = {
                .baudrate = 230400
            };
            mavproxy_rtcm_device_list[idx].config = rt_malloc(sizeof(mavproxy_serial_dev_config));

            /* set default value */
            if (mavproxy_rtcm_device_list[idx].config != NULL) {
                *(mavproxy_serial_dev_config*)mavproxy_rtcm_device_list[idx].config = serial_default_config;
            } else {
                TOML_DBG_E("malloc fail\n");
                err = RT_ERROR;
            }
        } else {
            TOML_DBG_E("unknown device type: %s\n", mavproxy_rtcm_device_list[idx].type);
            err = RT_ERROR;
        }
    } else {
        TOML_DBG_E("fail to parse type value\n");
        return RT_ERROR;
    }

    if (toml_string_in(curtab, "name", &mavproxy_rtcm_device_list[idx].name) != 0) {
        TOML_DBG_E("fail to parse name value\n");
        return RT_ERROR;
    }

    /* traverse keys in table */
    for (i = 0; 0 != (key = toml_key_in(curtab, i)); i++) {
        if (MATCH(key, "type") || MATCH(key, "name")) {
            /* already handled */
            continue;
        }

        if (MATCH(mavproxy_rtcm_device_list[idx].type, "serial")) {
            mavproxy_serial_dev_config* config = (mavproxy_serial_dev_config*)mavproxy_rtcm_device_list[idx].config;
            if (MATCH(key, "baudrate")) {
                int64_t ival;
                if (toml_int_in(curtab, key, &ival) == 0) {
                    config->baudrate = (uint32_t)ival;
                } else {
                    TOML_DBG_W("fail to parse baudrate value\n");
                    continue;
                }
            } else {
                TOML_DBG_W("unknown config key: %s\n", key);
                continue;
            }
        } else {
            TOML_DBG_W("unknown device type:%s \n", mavproxy_rtcm_device_list[idx].type);
            continue;
        }
    }

    if (err == RT_EOK) {
        mavproxy_rtcm_device_num += 1;
    }

    return err;
}

static rt_err_t mavproxy_parse_rtcm_devices(const toml_array_t* array)
{
    int i;
    toml_table_t* curtab;
    rt_err_t err = RT_EOK;

    for (i = 0; 0 != (curtab = toml_table_at(array, i)); i++) {
        err = mavproxy_parse_rtcm_device(curtab);

        if (err != RT_EOK) {
            TOML_DBG_E("device parse fail: %d\n", err);
            return err;
        }
    }

    return err;
}

uint8_t mavproxy_get_dev_num(uint8_t chan)
{
    return DEVICE_NUM(chan);
}

mavproxy_device_info* mavproxy_get_dev_list(uint8_t chan)
{
    return DEVICE_LIST(chan);
}

uint8_t mavproxy_get_rtcm_dev_num(void)
{
    return mavproxy_rtcm_device_num;
}

mavproxy_device_info* mavproxy_get_rtcm_dev_list(void)
{
    return mavproxy_rtcm_device_list;
}

rt_err_t mavproxy_get_devinfo(uint8_t chan, rt_device_t dev, mavproxy_device_info* info)
{
    for (int idx = 0; idx < DEVICE_NUM(chan); idx++) {
        if (rt_device_find(DEVICE_LIST(chan)[idx].name) == dev) {
            *info = DEVICE_LIST(chan)[idx];
            return RT_EOK;
        }
    }

    return RT_EINVAL;
}

rt_err_t mavproxy_switch_dev(uint8_t chan, uint8_t idx)
{
    rt_device_t old_device, new_device;

    if (idx >= DEVICE_NUM(chan)) {
        return RT_EINVAL;
    }

    old_device = mavproxy_dev_get_device(chan);
	new_device = rt_device_find(DEVICE_LIST(chan)[idx].name);
	if(new_device == old_device){
		return RT_EOK;
	}
    if (mavproxy_dev_set_device(chan, DEVICE_LIST(chan)[idx].name) == RT_EOK) {
		mavproxy_set_device(chan,idx);
        /* configure serial device if needed */
        if (DEVICE_TYPE_IS(chan, idx, serial)) {
            struct rt_serial_device* serial_dev;
            mavproxy_serial_dev_config* config = (mavproxy_serial_dev_config*)DEVICE_LIST(chan)[idx].config;
			
            serial_dev = (struct rt_serial_device*)new_device;

            if (serial_dev->config.baud_rate != config->baudrate) {
                struct serial_configure pconfig = serial_dev->config;
                pconfig.baud_rate = config->baudrate;
                if (rt_device_control(new_device, RT_DEVICE_CTRL_CONFIG, &pconfig) != RT_EOK) {
                    return RT_ERROR;
                }
            }
        }
        /* now we can safely close the old device */
        if (old_device != NULL && new_device != old_device) {
            rt_device_close(old_device);
        }
    } else {
        TOML_DBG_E("mavproxy_dev_set_device failed \n");
        return RT_ERROR;
    }

    return RT_EOK;
}

/* config mavproxy via toml system configuration file */
rt_err_t mavproxy_toml_config(toml_table_t* table)
{
    int i;
    const char* key;
    toml_array_t* arr;
    rt_err_t err = RT_EOK;

    /* traverse keys in table */
    for (i = 0; 0 != (key = toml_key_in(table, i)); i++) {
        if (MATCH(key, "devices")) {
            if (toml_array_table_in(table, key, &arr) == 0) {
                err = mavproxy_parse_devices(arr);
                if (err != RT_EOK) {
                    TOML_DBG_E("fail to parse devices\n");
                    return err;
                }

            } else {
                TOML_DBG_E("fail to get devices table\n");
                return RT_ERROR;
            }
        } else if (MATCH(key, "rtcm_devices")) {
            if (toml_array_table_in(table, key, &arr) == 0) {
                err = mavproxy_parse_rtcm_devices(arr);
                if (err != RT_EOK) {
                    TOML_DBG_E("fail to parse rtcm devices\n");
                    return err;
                }
            } else {
                TOML_DBG_E("fail to get rtcm devices table\n");
                return RT_ERROR;
            }
        } else {
            TOML_DBG_E("unknown config key: %s\n", key);
            err = RT_ERROR;
        }
    }
	mavproxy_switch_dev(0,0);
	mavproxy_switch_dev(1,0);
    return err;
}
