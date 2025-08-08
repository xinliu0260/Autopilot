/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#ifndef CONTROL_INTERFACE_H__
#define CONTROL_INTERFACE_H__

#include <Controller.h>
#include "autopilot_def.h"
#ifdef __cplusplus
extern "C" {
#endif

extern model_info_t control_model_info;

void control_interface_init(void);
void control_interface_step(uint32_t timestamp);

#ifdef __cplusplus
}
#endif

#endif