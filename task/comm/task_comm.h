/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#ifndef TASK_COMM_H__
#define TASK_COMM_H__

#include "autopilot.h"
#include "mavlink_types.h"

#ifdef __cplusplus
extern "C" {
#endif

rt_bool_t mavlink_msg_heartbeat_pack_func(mavlink_message_t* msg_t);
rt_bool_t mavlink_msg_extended_sys_state_pack_func(mavlink_message_t* msg_t);
rt_bool_t mavlink_msg_sys_status_pack_func(mavlink_message_t* msg_t);
rt_bool_t mavlink_msg_system_time_pack_func(mavlink_message_t* msg_t);
rt_bool_t mavlink_msg_attitude_pack_func(mavlink_message_t* msg_t);
rt_bool_t mavlink_msg_attitude_quaternion_pack_func(mavlink_message_t* msg_t);
rt_bool_t mavlink_msg_local_position_ned_pack_func(mavlink_message_t* msg_t);
rt_bool_t mavlink_msg_global_position_int_pack_func(mavlink_message_t* msg_t);
rt_bool_t mavlink_msg_scaled_imu_pack_func(mavlink_message_t* msg_t);
rt_bool_t mavlink_msg_vfr_hud_pack_func(mavlink_message_t* msg_t);
rt_bool_t mavlink_msg_altitude_pack_func(mavlink_message_t* msg_t);
rt_bool_t mavlink_msg_gps_raw_int_pack_func(mavlink_message_t* msg_t);
rt_bool_t mavlink_msg_rc_channels_pack_func(mavlink_message_t* msg_t);
rt_bool_t mavlink_msg_highres_imu_pack_func(mavlink_message_t* msg_t);
rt_bool_t mavlink_msg_distance_sensor_pack_func(mavlink_message_t* msg_t);
rt_bool_t mavlink_msg_gps_global_origin_pack_func(mavlink_message_t* msg_t);
rt_bool_t mavlink_msg_home_position_pack_func(mavlink_message_t* msg_t);
rt_bool_t mavlink_msg_attitude_target_pack_func(mavlink_message_t* msg_t);
rt_bool_t mavlink_msg_position_target_local_pack_func(mavlink_message_t* msg_t);
rt_bool_t mavlink_msg_hil_state_pack_func(mavlink_message_t* msg_t);

rt_err_t mavlink_command_acknowledge(uint8_t chan, uint16_t command, uint8_t result);

#ifdef __cplusplus
}
#endif

#endif