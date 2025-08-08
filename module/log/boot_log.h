/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

#ifndef BOOT_LOG_H__
#define BOOT_LOG_H__

#include "rtthread.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t boot_log_push(const char* content, uint32_t len);
rt_err_t boot_log_dump(void);
rt_err_t boot_log_init(void);

#ifdef __cplusplus
}
#endif

#endif