/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#include <Controller.h>
#include "autopilot.h"

#include "module/log/mlog.h"
#include "module/param/param.h"
#include "module/ipc/uMCN.h"
#include "module/console/console.h"
#include "stdlib.h"

#define LOG_TAG "Controller"
#define LOG_LVL DBG_INFO
#include "ulog.h"


/* controller input topic */
MCN_DECLARE(fms_output);
MCN_DECLARE(ins_output);

/* controller output topic */
MCN_DEFINE(control_output, sizeof(Control_Out_Bus));

/* define parameters */
static param_t __param_list[] = {
    // PARAM_UINT8(AIRFRAME, AIRFRAME, true),
    PARAM_FLOAT(VEL_XY_P, 1.4, false),
    PARAM_FLOAT(VEL_XY_I, 0.2, false),
    PARAM_FLOAT(VEL_XY_D, 0.05, false),
    PARAM_FLOAT(VEL_Z_P, 0.5, false),
    PARAM_FLOAT(VEL_Z_I, 0.12, false),
    PARAM_FLOAT(VEL_Z_D, 0.0, false),
    PARAM_FLOAT(VEL_XY_I_MIN, -1, false),
    PARAM_FLOAT(VEL_XY_I_MAX, 1, false),
    PARAM_FLOAT(VEL_XY_D_MIN, -1, false),
    PARAM_FLOAT(VEL_XY_D_MAX, 1, false),
    PARAM_FLOAT(VEL_Z_I_MIN, -0.2, false),
    PARAM_FLOAT(VEL_Z_I_MAX, 0.2, false),
    PARAM_FLOAT(VEL_Z_D_MIN, -0.1, false),
    PARAM_FLOAT(VEL_Z_D_MAX, 0.1, false),
    PARAM_FLOAT(ROLL_P, 7, false),
    PARAM_FLOAT(PITCH_P, 7, false),
    PARAM_FLOAT(ROLL_PITCH_CMD_LIM, PI / 6, false),
    PARAM_FLOAT(ROLL_RATE_P, 0.045, false),
    PARAM_FLOAT(PITCH_RATE_P, 0.045, false),
    PARAM_FLOAT(YAW_RATE_P, 0.15, false),
    PARAM_FLOAT(ROLL_RATE_I, 0.05, false),
    PARAM_FLOAT(PITCH_RATE_I, 0.05, false),
    PARAM_FLOAT(YAW_RATE_I, 0.15, false),
    PARAM_FLOAT(ROLL_RATE_D, 0.0015, false),
    PARAM_FLOAT(PITCH_RATE_D, 0.0015, false),
    PARAM_FLOAT(YAW_RATE_D, 0.001, false),
    PARAM_FLOAT(RATE_I_MIN, -0.1, false),
    PARAM_FLOAT(RATE_I_MAX, 0.1, false),
    PARAM_FLOAT(RATE_D_MIN, -0.1, false),
    PARAM_FLOAT(RATE_D_MAX, 0.1, false),
    PARAM_FLOAT(P_Q_CMD_LIM, PI / 2, false),
    PARAM_FLOAT(R_CMD_LIM, PI, false),
    PARAM_FLOAT(HOVER_THRO, 0.5, false),
};
PARAM_GROUP_DEFINE(CONTROL, __param_list);

/* define log data */
static mlog_elem_t Control_Out_Elems[] = {
    MLOG_ELEMENT(timestamp, MLOG_UINT32),
    MLOG_ELEMENT_VEC(actuator_cmd, MLOG_UINT16, 16),
};
MLOG_BUS_DEFINE(Control_Out, Control_Out_Elems);

static McnNode_t fms_out_nod;
static McnNode_t ins_out_nod;

static int Control_Out_ID;

model_info_t control_model_info;

static int control_out_echo(void* param)
{
    Control_Out_Bus control_out;

    if (mcn_copy_from_hub((McnHub*)param, &control_out) != RT_EOK)
        return -1;

    console_printf("timestamp:%d actuator:", control_out.timestamp);
    for (uint8_t i = 0; i < 16; i++) {
        if (control_out.actuator_cmd[i] > 0) {
            console_printf(" %d", control_out.actuator_cmd[i]);
        } else {
            break;
        }
    }
    console_printf("\n");
    return 0;
}

static void init_parameter(void)
{
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, VEL_XY_P), &CONTROL_PARAM.VEL_XY_P));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, VEL_XY_I), &CONTROL_PARAM.VEL_XY_I));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, VEL_XY_D), &CONTROL_PARAM.VEL_XY_D));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, VEL_Z_P), &CONTROL_PARAM.VEL_Z_P));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, VEL_Z_I), &CONTROL_PARAM.VEL_Z_I));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, VEL_Z_D), &CONTROL_PARAM.VEL_Z_D));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, VEL_XY_I_MIN), &CONTROL_PARAM.VEL_XY_I_MIN));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, VEL_XY_I_MAX), &CONTROL_PARAM.VEL_XY_I_MAX));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, VEL_XY_D_MIN), &CONTROL_PARAM.VEL_XY_D_MIN));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, VEL_XY_D_MAX), &CONTROL_PARAM.VEL_XY_D_MAX));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, VEL_Z_I_MIN), &CONTROL_PARAM.VEL_Z_I_MIN));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, VEL_Z_I_MAX), &CONTROL_PARAM.VEL_Z_I_MAX));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, VEL_Z_D_MIN), &CONTROL_PARAM.VEL_Z_D_MIN));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, VEL_Z_D_MAX), &CONTROL_PARAM.VEL_Z_D_MAX));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, ROLL_P), &CONTROL_PARAM.ROLL_P));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, PITCH_P), &CONTROL_PARAM.PITCH_P));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, ROLL_PITCH_CMD_LIM), &CONTROL_PARAM.ROLL_PITCH_CMD_LIM));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, ROLL_RATE_P), &CONTROL_PARAM.ROLL_RATE_P));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, PITCH_RATE_P), &CONTROL_PARAM.PITCH_RATE_P));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, YAW_RATE_P), &CONTROL_PARAM.YAW_RATE_P));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, ROLL_RATE_I), &CONTROL_PARAM.ROLL_RATE_I));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, PITCH_RATE_I), &CONTROL_PARAM.PITCH_RATE_I));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, YAW_RATE_I), &CONTROL_PARAM.YAW_RATE_I));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, ROLL_RATE_D), &CONTROL_PARAM.ROLL_RATE_D));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, PITCH_RATE_D), &CONTROL_PARAM.PITCH_RATE_D));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, YAW_RATE_D), &CONTROL_PARAM.YAW_RATE_D));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, RATE_I_MIN), &CONTROL_PARAM.RATE_I_MIN));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, RATE_I_MAX), &CONTROL_PARAM.RATE_I_MAX));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, RATE_D_MIN), &CONTROL_PARAM.RATE_D_MIN));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, RATE_D_MAX), &CONTROL_PARAM.RATE_D_MAX));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, P_Q_CMD_LIM), &CONTROL_PARAM.P_Q_CMD_LIM));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, R_CMD_LIM), &CONTROL_PARAM.R_CMD_LIM));
    RT_CHECK(param_link_variable(PARAM_GET(CONTROL, HOVER_THRO), &CONTROL_PARAM.HOVER_THRO));
}

void control_interface_step(uint32_t timestamp)
{
    if (mcn_poll(fms_out_nod)) {
        mcn_copy(MCN_HUB(fms_output), fms_out_nod, &Controller_U.FMS_Out);
    }

    if (mcn_poll(ins_out_nod)) {
        mcn_copy(MCN_HUB(ins_output), ins_out_nod, &Controller_U.INS_Out);
    }

    Controller_step();

    mcn_publish(MCN_HUB(control_output), &Controller_Y.Control_Out);

    PERIOD_EXECUTE(control_output, 100, 
        {
            mlog_push_msg((uint8_t*)&Controller_Y.Control_Out, Control_Out_ID, sizeof(Control_Out_Bus));
        }
    );
}

void control_interface_init(void)
{
    control_model_info.period = CONTROL_EXPORT.period;
    control_model_info.info = (char*)CONTROL_EXPORT.model_info;

    mcn_advertise(MCN_HUB(control_output), control_out_echo);

    fms_out_nod = mcn_subscribe(MCN_HUB(fms_output), NULL);
    ins_out_nod = mcn_subscribe(MCN_HUB(ins_output), NULL);

    Control_Out_ID = mlog_get_bus_id("Control_Out");
    RT_ASSERT(Control_Out_ID >= 0);

    Controller_init();

    init_parameter();
}