/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

#include "autopilot.h"
#include <FMS.h>
#include "module/console/console.h"
#include "module/ipc/uMCN.h"
#include "string.h"

#define LOG_TAG "GCS"
#define LOG_LVL DBG_INFO
#include "ulog.h"


MCN_DEFINE(gcs_cmd, sizeof(GCS_Cmd_Bus));
MCN_DECLARE(fms_output);

uint32_t gcs_cmd_buffer[20];
float gcs_cmd_param_buffer[20 * 7];
struct rt_ringbuffer* gcs_cmd_rb;
struct rt_ringbuffer* gcs_cmd_param_rb;

PilotMode gcs_mode_buffer[20];
struct rt_ringbuffer* gcs_mode_rb;
GCS_Cmd_Bus gcs_cmd;

McnNode_t fms_out_nod;
FMS_Out_Bus fms_out;
uint8_t gcs_heartbeat;

static int gcs_cmd_echo(void* parameter)
{
    GCS_Cmd_Bus gcs_cmd;

    if (mcn_copy_from_hub((McnHub*)parameter, &gcs_cmd) != RT_EOK) {
        return -1;
    }

    console_printf("timestamp:%u mode:%u cmd1:%u cmd2:%u\n",
           gcs_cmd.timestamp,
           gcs_cmd.mode,
           gcs_cmd.cmd_1,
           gcs_cmd.cmd_2);

    return 0;
}

void gcs_cmd_heartbeat(void)
{
    gcs_heartbeat = 1;
}

GCS_Cmd_Bus gcs_cmd_get(void)
{
    return gcs_cmd;
}

rt_err_t gcs_set_cmd(FMS_Cmd cmd, float* param)
{
    uint32_t new_cmd = cmd;

    switch (cmd) {
    case FMS_Cmd_PreArm:
        LOG_I("recv PreArm command.");
        break;
    case FMS_Cmd_Arm:
        LOG_I("recv Arm command.");
        break;
    case FMS_Cmd_Disarm:
        LOG_I("recv Disarm command.");
        break;
    case FMS_Cmd_Takeoff:
        LOG_I("recv Takeoff command.");
        break;
    case FMS_Cmd_Land:
        LOG_I("recv Land command.");
        break;
    case FMS_Cmd_Return:
        LOG_I("recv Return command.");
        break;
    case FMS_Cmd_Pause:
        LOG_I("recv Pause command.");
        break;
    case FMS_Cmd_Continue:
        LOG_I("recv Continue command.");
        break;
    case FMS_Cmd_SetHome:
        LOG_I("recv SetHome command. [x y z yaw] = [%.2f %.2f %.2f %.2f]",
              param[0],
              param[1],
              param[2],
              param[3]);
        break;
    default:
        LOG_W("recv Unknown command %d.", cmd);
        return RT_EINVAL;
    }

    rt_ringbuffer_put(gcs_cmd_rb, (uint8_t*)&new_cmd, sizeof(new_cmd));
    rt_ringbuffer_put(gcs_cmd_param_rb, (uint8_t*)param, sizeof(gcs_cmd.param));

    return RT_EOK;
}

rt_err_t gcs_set_mode(PilotMode mode)
{
    uint32_t new_mode = mode;

    switch (mode) {
    case PilotMode_Manual:
        LOG_I("set Manual mode.");
        break;
    case PilotMode_Acro:
        LOG_I("set Acro mode.");
        break;
    case PilotMode_Stabilize:
        LOG_I("set Stabilize mode.");
        break;
    case PilotMode_Altitude:
        LOG_I("set Altitude mode.");
        break;
    case PilotMode_Position:
        LOG_I("set Position mode.");
        break;
    case PilotMode_Mission:
        LOG_I("set Mission mode.");
        break;
    case PilotMode_Offboard:
        LOG_I("set Offboard mode.");
        break;
    default:
        LOG_W("set Unknown mode %d.", mode);
        return RT_EINVAL;
    }

    if (mcn_poll(fms_out_nod)) {
        mcn_copy(MCN_HUB(fms_output), fms_out_nod, &fms_out);
    }

    if ((fms_out.mode == PilotMode_Mission || fms_out.mode == PilotMode_Offboard)
        && fms_out.mode == mode && fms_out.state == VehicleState_Hold) {
        /* When vehicle is in auto mode (mission,offboard), reset the mode would trigger FMS_Cmd_Continue.
           e.g, When mission is paused, the vehicle would enter hold mode (state = VehicleState_Hold), 
           however, the mode is still PilotMode_Mission, so the mode would not change when user try
           to set mode to Mission and continue. Therefore, we send FMS_Cmd_Continue instead to continue
           the mission mode. */
        gcs_set_cmd(FMS_Cmd_Continue, (float[7]) { 0 });
    } else {
        /* For normal case, we just set the new mode. */
        rt_ringbuffer_put(gcs_mode_rb, (uint8_t*)&new_mode, sizeof(new_mode));
    }

    return RT_EOK;
}

rt_err_t gcs_cmd_collect(void)
{
    static uint32_t last_cmd_timestamp = 0;
    uint8_t updated = 0;
    uint32_t time_now = rt_tick_get_millisecond();

    if (rt_ringbuffer_data_len(gcs_mode_rb) >= sizeof(gcs_cmd.mode)) {
        rt_ringbuffer_get(gcs_mode_rb, (uint8_t*)&gcs_cmd.mode, sizeof(gcs_cmd.mode));
        updated = 1;
    }

    /* the command need to last at least FMS_CONST.dt */
    if (time_now - last_cmd_timestamp > 100) {
        /* check if there is new command in buffer */
        if (rt_ringbuffer_data_len(gcs_cmd_rb) >= sizeof(gcs_cmd.cmd_1)) {
            last_cmd_timestamp = time_now;

            rt_ringbuffer_get(gcs_cmd_rb, (uint8_t*)&gcs_cmd.cmd_1, sizeof(gcs_cmd.cmd_1));
            rt_ringbuffer_get(gcs_cmd_param_rb, (uint8_t*)&gcs_cmd.param, sizeof(gcs_cmd.param));
            updated = 1;
        } else {
            if (gcs_cmd.cmd_1 > 0) {
                gcs_cmd.cmd_1 = 0;
                updated = 1;
            }
        }
    }

    if (gcs_heartbeat) {
        gcs_heartbeat = 0;
        /* if received heartbeat, publish the gcs_cmd, so system knows gcs is online */
        updated = 1;
    }

    if (updated) {
        gcs_cmd.timestamp = rt_tick_get_millisecond();
        RT_TRY(mcn_publish(MCN_HUB(gcs_cmd), &gcs_cmd));
    }

    return RT_EOK;
}

rt_err_t gcs_cmd_init(void)
{
	gcs_cmd_rb = (struct rt_ringbuffer* )rt_malloc(sizeof(struct rt_ringbuffer));
	RT_ASSERT(gcs_cmd_rb != NULL);
	gcs_cmd_param_rb = (struct rt_ringbuffer* )rt_malloc(sizeof(struct rt_ringbuffer));
	RT_ASSERT(gcs_cmd_param_rb != NULL);
	gcs_mode_rb = (struct rt_ringbuffer* )rt_malloc(sizeof(struct rt_ringbuffer));
	RT_ASSERT(gcs_mode_rb != NULL);

	rt_ringbuffer_init(gcs_cmd_rb, (uint8_t*)gcs_cmd_buffer, sizeof(gcs_cmd_buffer));
	rt_ringbuffer_init(gcs_cmd_param_rb, (uint8_t*)gcs_cmd_param_buffer, sizeof(gcs_cmd_param_buffer));
	rt_ringbuffer_init(gcs_mode_rb, (uint8_t*)gcs_mode_buffer,sizeof(gcs_mode_buffer));
	
    RT_TRY(mcn_advertise(MCN_HUB(gcs_cmd), gcs_cmd_echo));

    fms_out_nod = mcn_subscribe(MCN_HUB(fms_output), NULL,NULL);
    RT_ASSERT(fms_out_nod != NULL);

    return RT_EOK;
}