/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */
 
#ifndef CONSOLE_H__
#define CONSOLE_H__

#include "rtthread.h"

#ifdef __cplusplus
extern "C" {
#endif

#define console_printf console_printf

/* console API */
rt_err_t console_set_device(const char* device_name);
rt_device_t console_get_device(void);
int console_printf(const char* fmt, ...);
int console_println(const char* fmt, ...);
int console_print_args(const char* fmt, va_list args);
int console_write(const char* content, uint32_t len);
void console_format(char* buffer, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
