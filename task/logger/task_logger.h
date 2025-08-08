/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#ifndef TASK_LOGGER_H__
#define TASK_LOGGER_H__

#include "autopilot.h"

#ifdef __cplusplus
extern "C" {
#endif

rt_err_t logger_start_mlog(char* path);
void logger_stop_mlog(void);
void get_working_log_session(char* path);

#ifdef __cplusplus
}
#endif

#endif
