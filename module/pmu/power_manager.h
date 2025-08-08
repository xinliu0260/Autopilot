/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */
 
#ifndef POWER_MANAGER_H__
#define POWER_MANAGER_H__

#include "autopilot.h"

#ifdef __cplusplus
extern "C" {
#endif

struct battery_status {
    uint16_t battery_voltage;
    int16_t battery_current;
    int8_t battery_remaining;
};

rt_err_t pmu_init(void);
rt_err_t pmu_poll_battery_status(void);

#ifdef __cplusplus
}
#endif

#endif
