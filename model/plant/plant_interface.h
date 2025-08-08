/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#ifndef PLANT_INTERFACE_H__
#define PLANT_INTERFACE_H__

#include <Plant.h>
#include "autopilot_def.h"
#ifdef __cplusplus
extern "C" {
#endif

extern model_info_t plant_model_info;

void plant_interface_init(void);
void plant_interface_step(uint32_t timestamp);

#ifdef __cplusplus
}
#endif

#endif