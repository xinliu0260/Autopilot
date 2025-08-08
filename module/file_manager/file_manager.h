/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#ifndef FILE_MANAGER_H__
#define FILE_MANAGER_H__

#include <dfs_posix.h>
#include "autopilot.h"

#ifdef __cplusplus
extern "C" {
#endif

rt_err_t file_manager_init(void);
rt_err_t current_log_session(char* path);

/* file extended operation */
int fm_fprintf(int fd, const char* fmt, ...);
int fm_deldir(const char* path);

#ifdef __cplusplus
}
#endif

#endif
