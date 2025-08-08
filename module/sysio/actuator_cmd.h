/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#ifndef ACTUATOR_CMD_H__
#define ACTUATOR_CMD_H__

#include "autopilot.h"

#ifdef __cplusplus
extern "C" {
#endif

rt_err_t send_actuator_cmd(void);
rt_err_t send_hil_actuator_cmd(void);
rt_err_t actuator_init(void);

#ifdef __cplusplus
}
#endif

#endif