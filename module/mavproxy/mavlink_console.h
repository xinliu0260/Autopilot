/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#ifndef MAVLINK_CONSOLE_H__
#define MAVLINK_CONSOLE_H__

#include "autopilot.h"
#include <mavlink.h>

#ifdef __cplusplus
extern "C" {
#endif

rt_err_t mavlink_console_init(void);
void mavlink_console_process_rx_msg(const mavlink_serial_control_t* serial_control);
void mavlink_console_handle_timeout(void);

#ifdef __cplusplus
}
#endif

#endif
