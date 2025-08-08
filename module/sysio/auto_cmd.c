/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

#include "autopilot.h"

#include <FMS.h>

#include "module/sysio/auto_cmd.h"
#include "module/console/console.h"
#include "module/ipc/uMCN.h"

#define LOG_TAG "autocmd"
#define LOG_LVL DBG_INFO
#include "ulog.h"


MCN_DEFINE(auto_cmd, sizeof(Auto_Cmd_Bus));

static int auto_cmd_echo(void* param)
{
    Auto_Cmd_Bus auto_cmd;

    mcn_copy_from_hub((McnHub*)param, &auto_cmd);

    console_printf("timestamp:%u frame:%d\n", auto_cmd.timestamp, auto_cmd.frame);

    if (auto_cmd.cmd_mask & P_CMD_VALID) {
        console_printf("p: %.2f\n", auto_cmd.p_cmd);
    }

    if (auto_cmd.cmd_mask & Q_CMD_VALID) {
        console_printf("q: %.2f\n", auto_cmd.q_cmd);
    }

    if (auto_cmd.cmd_mask & R_CMD_VALID) {
        console_printf("r: %.2f\n", auto_cmd.r_cmd);
    }

    if (auto_cmd.cmd_mask & PHI_CMD_VALID) {
        console_printf("phi: %.2f\n", auto_cmd.phi_cmd);
    }

    if (auto_cmd.cmd_mask & THETA_CMD_VALID) {
        console_printf("theta: %.2f\n", auto_cmd.theta_cmd);
    }

    if (auto_cmd.cmd_mask & PSI_CMD_VALID) {
        console_printf("psi: %.2f\n", auto_cmd.psi_cmd);
    }

    if (auto_cmd.cmd_mask & PSI_RATE_CMD_VALID) {
        console_printf("psi rate: %.2f\n", auto_cmd.psi_rate_cmd);
    }

    if (auto_cmd.cmd_mask & X_CMD_VALID) {
        console_printf("x: %.2f\n", auto_cmd.x_cmd);
    }

    if (auto_cmd.cmd_mask & Y_CMD_VALID) {
        console_printf("y: %.2f\n", auto_cmd.y_cmd);
    }

    if (auto_cmd.cmd_mask & Z_CMD_VALID) {
        console_printf("z: %.2f\n", auto_cmd.z_cmd);
    }

    if (auto_cmd.cmd_mask & LAT_CMD_VALID) {
        console_printf("lat: %d\n", auto_cmd.lat_cmd);
    }

    if (auto_cmd.cmd_mask & LON_CMD_VALID) {
        console_printf("lon: %.d\n", auto_cmd.lon_cmd);
    }

    if (auto_cmd.cmd_mask & ALT_CMD_VALID) {
        console_printf("alt: %.2f\n", auto_cmd.alt_cmd);
    }

    if (auto_cmd.cmd_mask & U_CMD_VALID) {
        console_printf("u: %.2f\n", auto_cmd.u_cmd);
    }

    if (auto_cmd.cmd_mask & V_CMD_VALID) {
        console_printf("v: %.2f\n", auto_cmd.v_cmd);
    }

    if (auto_cmd.cmd_mask & W_CMD_VALID) {
        console_printf("w: %.2f\n", auto_cmd.w_cmd);
    }

    if (auto_cmd.cmd_mask & AX_CMD_VALID) {
        console_printf("ax: %.2f\n", auto_cmd.ax_cmd);
    }

    if (auto_cmd.cmd_mask & AY_CMD_VALID) {
        console_printf("ay: %.2f\n", auto_cmd.ay_cmd);
    }

    if (auto_cmd.cmd_mask & AZ_CMD_VALID) {
        console_printf("az: %.2f\n", auto_cmd.az_cmd);
    }

    if (auto_cmd.cmd_mask & THROTTLE_CMD_VALID) {
        console_printf("throttle: %d\n", auto_cmd.throttle_cmd);
    }

    console_printf("------------------------------------------\n");

    return 0;
}

rt_err_t auto_cmd_init(void)
{
    RT_TRY(mcn_advertise(MCN_HUB(auto_cmd), auto_cmd_echo));

    return RT_EOK;
}
