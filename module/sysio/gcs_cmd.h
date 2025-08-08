/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

#ifndef GCS_CMD_H__
#define GCS_CMD_H__

#include <FMS.h>
#include "rtdef.h"
#ifdef __cplusplus
extern "C" {
#endif

rt_err_t gcs_cmd_init(void);
GCS_Cmd_Bus gcs_cmd_get(void);
rt_err_t gcs_cmd_collect(void);
void gcs_cmd_heartbeat(void);
rt_err_t gcs_set_cmd(FMS_Cmd cmd, float* param);
rt_err_t gcs_set_mode(PilotMode mode);

#ifdef __cplusplus
}
#endif

#endif
