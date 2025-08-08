/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#ifndef FMS_INTERFACE_H__
#define FMS_INTERFACE_H__

#include <FMS.h>
#include "autopilot_def.h"

#ifdef __cplusplus
extern "C" {
#endif

extern model_info_t fms_model_info;

void fms_interface_init(void);
void fms_interface_step(uint32_t timestamp);

#ifdef __cplusplus
}
#endif

#endif