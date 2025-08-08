/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

#ifndef MAVPROXY_H__
#define MAVPROXY_H__

#include "autopilot.h"
#include <mavlink.h>

#include "module/mavproxy/mavlink_console.h"
#include "module/mavproxy/mavlink_mission.h"
#include "module/mavproxy/mavlink_param.h"
#include "module/mavproxy/mavlink_rtcm.h"
#include "module/mavproxy/mavlink_status.h"
#include "module/mavproxy/mavproxy_cmd.h"
#include "module/mavproxy/mavproxy_dev.h"
#include "module/mavproxy/mavproxy_monitor.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EVENT_MAVPROXY_UPDATE    (1 << 0)
#define EVENT_MAVCONSOLE_TIMEOUT (1 << 1)
#define EVENT_SEND_ALL_PARAM     (1 << 2)

#define MAVPROXY_CHAN_NUM        2
#define MAVPROXY_GCS_CHAN        0
#define MAVPROXY_OBC_CHAN        1

typedef rt_bool_t (*msg_pack_cb_t)(mavlink_message_t* msg_t);

rt_err_t mavproxy_init(void);
void mavproxy_channel_loop(uint8_t chan);
mavlink_system_t mavproxy_get_system(void);
rt_err_t mavproxy_set_device(uint8_t chan, uint8_t devid);
rt_err_t mavproxy_send_event(uint8_t chan, uint32_t event_set);
rt_err_t mavproxy_send_immediate_msg(uint8_t chan, const mavlink_message_t* msg, rt_bool_t sync);
rt_err_t mavproxy_register_period_msg(uint8_t chan, uint8_t msgid, uint16_t msg_rate_hz, msg_pack_cb_t msg_pack_cb, rt_bool_t start);

#ifdef __cplusplus
}
#endif

#endif
