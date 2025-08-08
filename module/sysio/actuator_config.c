/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

#include "autopilot.h"
#include <string.h>

#include "extdev/dev_actuator.h"
#include "module/sysio/actuator_config.h"
#include "module/toml/toml.h"
#include "module/console//console.h"

#define TOML_DBG_E(...)                 toml_debug("Actuator", "E", __VA_ARGS__)
#define TOML_DBG_W(...)                 toml_debug("Actuator", "W", __VA_ARGS__)

#define ACTUATOR_MAX_DEVICE_NUM         5
#define ACTUATOR_MAX_MAPPING_NUM        10
#define MATCH(a, b)                     (strcmp(a, b) == 0)
#define DEVICE_LIST                     actuator_device_list
#define DEVICE_NUM                      actuator_device_num
#define DEVICE_PROTOCOL_IS(_idx, _name) MATCH(DEVICE_LIST[_idx].protocol, #_name)
#define FIND_DEVICE(_idx)               rt_device_find(DEVICE_LIST[_idx].name)

static actuator_device_info actuator_device_list[ACTUATOR_MAX_DEVICE_NUM];
static uint8_t actuator_device_num;
static actuator_mapping actuator_mappings_list[ACTUATOR_MAX_MAPPING_NUM];
static uint8_t actuator_mapping_num;

static void swap(uint8_t* a, uint8_t* b)
{
    uint8_t temp = *a;
    *a = *b;
    *b = temp;
}

static int partition(uint8_t arr[], uint8_t arr2[], int low, int high)
{
    uint8_t pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
            swap(&arr2[i], &arr2[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    swap(&arr2[i + 1], &arr2[high]);
    return (i + 1);
}

static void quickSort(uint8_t arr[], uint8_t arr2[], int low, int high)
{
    if (low < high) {
        int pi = partition(arr, arr2, low, high);

        quickSort(arr, arr2, low, pi - 1);
        quickSort(arr, arr2, pi + 1, high);
    }
}

static rt_err_t actuator_parse_device(const toml_table_t* curtab, int idx)
{
    rt_err_t err = RT_EOK;
    int i;
    const char* key;

    /* get device name */
    if (toml_string_in(curtab, "name", &DEVICE_LIST[idx].name) != 0) {
        TOML_DBG_E("fail to parse name value\n");
        return RT_ERROR;
    }

    /* get device protocol */
    if (toml_string_in(curtab, "protocol", &DEVICE_LIST[idx].protocol) == 0) {
        actuator_dev_t act_dev = (actuator_dev_t)rt_device_find(DEVICE_LIST[idx].name);
        if (act_dev == NULL) {
            TOML_DBG_E("can't find device %s\n", DEVICE_LIST[idx].name);
            return RT_ERROR;
        }

        if (DEVICE_PROTOCOL_IS(idx, pwm)) {
            /* set protocol if it's not the case */
            if (act_dev->config.protocol != ACT_PROTOCOL_PWM) {
                int protocol = ACT_PROTOCOL_PWM;
                if (rt_device_control(&act_dev->parent, ACT_CMD_SET_PROTOCOL, &protocol) != RT_EOK) {
                    TOML_DBG_E("fail to set protocol to pwm\n");
                    return RT_ERROR;
                }
            }

            act_pwm_drv_config pwm_drv_default_config = {
                .freq = act_dev->config.pwm_config.pwm_freq
            };

            DEVICE_LIST[idx].config = rt_malloc(sizeof(act_pwm_drv_config));

            /* set default value */
            if (DEVICE_LIST[idx].config) {
                *(act_pwm_drv_config*)DEVICE_LIST[idx].config = pwm_drv_default_config;
            } else {
                TOML_DBG_E("fail to malloc memory\n");
                err = RT_ERROR;
            }
        } else if (DEVICE_PROTOCOL_IS(idx, dshot)) {
            /* set protocol if it's not the case */
            if (act_dev->config.protocol != ACT_PROTOCOL_DSHOT) {
                int protocol = ACT_PROTOCOL_DSHOT;
                if (rt_device_control(&act_dev->parent, ACT_CMD_SET_PROTOCOL, &protocol) != RT_EOK) {
                    TOML_DBG_E("fail to set protocol to dshot\n");
                    return RT_ERROR;
                }
            }

            act_dshot_drv_config dshot_drv_default_config = {
                .speed = act_dev->config.dshot_config.speed,
                .telem_req = act_dev->config.dshot_config.telem_req
            };

            DEVICE_LIST[idx].config = rt_malloc(sizeof(act_dshot_drv_config));

            /* set default value */
            if (DEVICE_LIST[idx].config) {
                *(act_dshot_drv_config*)DEVICE_LIST[idx].config = dshot_drv_default_config;
            } else {
                TOML_DBG_E("fail to malloc memory\n");
                err = RT_ERROR;
            }
        } else {
            TOML_DBG_E("unknown device protocol: %s\n", DEVICE_LIST[idx].protocol);
            err = RT_ERROR;
        }
    } else {
        TOML_DBG_E("fail to parse protocol value\n");
        return RT_ERROR;
    }

    /* traverse keys in table */
    for (i = 0; 0 != (key = toml_key_in(curtab, i)); i++) {
        if (MATCH(key, "protocol") || MATCH(key, "name")) {
            /* already handled */
            continue;
        }

        if (DEVICE_PROTOCOL_IS(idx, pwm)) {
            act_pwm_drv_config* config = (act_pwm_drv_config*)DEVICE_LIST[idx].config;

            if (MATCH(key, "freq")) {
                int64_t ival;
                if (toml_int_in(curtab, key, &ival) == 0) {
                    config->freq = ival;
                } else {
                    TOML_DBG_W("fail to parse freq value\n");
                    continue;
                }
            } else {
                TOML_DBG_W("unknown config key: %s\n", key);
                continue;
            }
        } else if (DEVICE_PROTOCOL_IS(idx, dshot)) {
            act_dshot_drv_config* config = (act_dshot_drv_config*)DEVICE_LIST[idx].config;

            if (MATCH(key, "speed")) {
                int64_t ival;
                if (toml_int_in(curtab, key, &ival) == 0) {
                    config->speed = ival;
                } else {
                    TOML_DBG_W("fail to parse speed value\n");
                    continue;
                }
            } else if (MATCH(key, "telem-req")) {
                int bval;
                if (toml_bool_in(curtab, key, &bval) == 0) {
                    config->telem_req = (rt_bool_t)bval;
                } else {
                    TOML_DBG_W("fail to parse telem-req value\n");
                    continue;
                }
            } else {
                TOML_DBG_W("unknown config key: %s\n", key);
                continue;
            }
        } else {
            // unknown type
            TOML_DBG_W("unknown key: %s\n", key);
            continue;
        }
    }

    return err;
}

static rt_err_t actuator_parse_devices(const toml_array_t* array)
{
    int i;
    toml_table_t* curtab;
    rt_err_t err = RT_EOK;
    uint32_t idx = 0;

    for (i = 0; 0 != (curtab = toml_table_at(array, i)); i++) {
        err = actuator_parse_device(curtab, idx);

        if (err != RT_EOK) {
            TOML_DBG_E("device parse fail: %d\n", err);
            continue;
        }

        if (++idx >= ACTUATOR_MAX_DEVICE_NUM) {
            TOML_DBG_W("too many devices\n");
            break;
        }
    }

    DEVICE_NUM = idx;

    return err;
}

static rt_err_t actuator_parse_mapping(const toml_table_t* curtab, int idx)
{
    rt_err_t err = RT_EOK;
    toml_array_t* arr;
    int i;
    int64_t ival;

    if (toml_string_in(curtab, "from", &actuator_mappings_list[idx].from) != 0) {
        TOML_DBG_E("fail to parse from device\n");
        return RT_ERROR;
    }

    if (toml_string_in(curtab, "to", &actuator_mappings_list[idx].to) != 0) {
        TOML_DBG_E("fail to parse to device\n");
        return RT_ERROR;
    }

    toml_array_t* mapping_array = toml_array_in(curtab, "chan-map");
    if (mapping_array == NULL) {
        TOML_DBG_E("fail to parse chan-map\n");
        return RT_ERROR;
    }

    if (toml_array_nelem(mapping_array) != 2) {
        TOML_DBG_E("illegal chan-map array length: %d\n", toml_array_nelem(mapping_array));
        return RT_ERROR;
    }

    arr = toml_array_at(mapping_array, 0);
    actuator_mappings_list[idx].map_size = toml_array_nelem(arr);
    for (i = 0; i < actuator_mappings_list[idx].map_size; i++) {
        if (toml_int_at(arr, i, &ival) == 0) {
            actuator_mappings_list[idx].from_map[i] = (uint16_t)ival;
        } else {
            TOML_DBG_E("fail to parse mapping value\n");
            return RT_ERROR;
        }
    }

    arr = toml_array_at(mapping_array, 1);
    if (toml_array_nelem(arr) != actuator_mappings_list[idx].map_size) {
        TOML_DBG_E("illegal array length:%d %d\n", toml_array_nelem(arr), actuator_mappings_list[idx].map_size);
        return RT_ERROR;
    }
    for (i = 0; i < actuator_mappings_list[idx].map_size; i++) {
        if (toml_int_at(arr, i, &ival) == 0) {
            actuator_mappings_list[idx].to_map[i] = (uint16_t)ival;
        } else {
            TOML_DBG_E("fail to parse mapping value\n");
            return RT_ERROR;
        }
    }

    /* sort to_mapping from small to big */
    if (actuator_mappings_list[idx].map_size > 0)
        quickSort(actuator_mappings_list[idx].to_map, actuator_mappings_list[idx].from_map, 0, actuator_mappings_list[idx].map_size - 1);

    return err;
}

static rt_err_t actuator_parse_mappings(const toml_array_t* array)
{
    int i;
    toml_table_t* curtab;
    rt_err_t err = RT_EOK;
    uint32_t idx = 0;

    for (i = 0; 0 != (curtab = toml_table_at(array, i)); i++) {
        err = actuator_parse_mapping(curtab, idx);

        if (err != RT_EOK) {
            TOML_DBG_E("mapping parse fail: %d\n", err);
            continue;
        }

        if (++idx >= ACTUATOR_MAX_MAPPING_NUM) {
            TOML_DBG_E("too many mappings\n");
            break;
        }
    }

    actuator_mapping_num = idx;

    return err;
}

void show_actuator_config(void)
{
    for (int i = 0; i < DEVICE_NUM; i++) {
        console_printf("protocol:%s name:%s\n", DEVICE_LIST[i].protocol, DEVICE_LIST[i].name);
        if (DEVICE_PROTOCOL_IS(i, pwm)) {
            act_pwm_drv_config* config = (act_pwm_drv_config*)DEVICE_LIST[i].config;
            console_printf("freq:%d\n", config->freq);
        } else if (DEVICE_PROTOCOL_IS(i, dshot)) {
            act_dshot_drv_config* config = (act_dshot_drv_config*)DEVICE_LIST[i].config;
            console_printf("speed:%d telem_req:%d\n", config->speed, config->telem_req);
        }
    }

    for (int i = 0; i < actuator_mapping_num; i++) {
        console_printf("from:%s to:%s\n", actuator_mappings_list[i].from, actuator_mappings_list[i].to);
        console_printf("[");
        console_printf("[");
        for (int j = 0; j < actuator_mappings_list[i].map_size; j++) {
            console_printf("%d,", actuator_mappings_list[i].from_map[j]);
        }
        console_printf("]");
        console_printf("[");
        for (int j = 0; j < actuator_mappings_list[i].map_size; j++) {
            console_printf("%d,", actuator_mappings_list[i].to_map[j]);
        }
        console_printf("]");
        console_printf("]\n");
    }
}

uint8_t actuator_toml_get_device_num(void)
{
    return actuator_device_num;
}

actuator_device_info* actuator_toml_get_device_list(void)
{
    return actuator_device_list;
}

uint8_t actuator_toml_get_mapping_num(void)
{
    return actuator_mapping_num;
}

actuator_mapping* actuator_toml_get_mapping_list(void)
{
    return actuator_mappings_list;
}

rt_err_t actuator_toml_config(toml_table_t* table)
{
    int i;
    const char* key;
    toml_array_t* arr;
    rt_err_t err = RT_EOK;

    /* first parse devices and mappings */
    for (i = 0; 0 != (key = toml_key_in(table, i)); i++) {
        if (MATCH(key, "devices")) {
            if (toml_array_table_in(table, key, &arr) == 0) {
                err = actuator_parse_devices(arr);
                if (err != RT_EOK) {
                    return err;
                }
            } else {
                TOML_DBG_E("fail to parse devices\n");
                return RT_ERROR;
            }
        } else if (MATCH(key, "mappings")) {
            if (toml_array_table_in(table, key, &arr) == 0) {
                err = actuator_parse_mappings(arr);
                if (err != RT_EOK) {
                    return err;
                }
            } else {
                TOML_DBG_E("fail to parse mappings\n");
                return RT_ERROR;
            }
        } else {
            TOML_DBG_W("unknown config key: %s\n", key);
        }
    }

    /* open actuator devices */
    for (i = 0; i < DEVICE_NUM; i++) {
        actuator_dev_t act_dev = (actuator_dev_t)rt_device_find(DEVICE_LIST[i].name);

        if (DEVICE_PROTOCOL_IS(i, pwm)) {
            act_pwm_drv_config* config = (act_pwm_drv_config*)DEVICE_LIST[i].config;

            act_dev->config.pwm_config.pwm_freq = config->freq;
        } else if (DEVICE_PROTOCOL_IS(i, dshot)) {
            act_dshot_drv_config* config = (act_dshot_drv_config*)DEVICE_LIST[i].config;

            act_dev->config.dshot_config.speed = config->speed;
            act_dev->config.dshot_config.telem_req = config->telem_req;
        } else {
            continue;
        }

        if (rt_device_open(&act_dev->parent, RT_DEVICE_OFLAG_RDWR) != RT_EOK) {
            return RT_ERROR;
        }
    }

    return RT_EOK;
}