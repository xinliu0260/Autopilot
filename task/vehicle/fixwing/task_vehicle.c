/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#include "autopilot.h"

#include "model/control/control_interface.h"
#include "model/fms/fms_interface.h"
#include "model/ins/ins_interface.h"
#include "model/plant/plant_interface.h"
#include "module/file_manager/file_manager.h"
#include "module/sensor/sensor_hub.h"
#include "module/sysio/actuator_cmd.h"
#include "module/sysio/gcs_cmd.h"
#include "module/sysio/mission_data.h"
#include "module/sysio/pilot_cmd.h"
#include "rtthread.h"
#include "task/logger/task_logger.h"
#include <stdint.h>

#define EVENT_VEHICLE_UPDATE (1 << 0)

extern rt_device_t main_out_dev;
extern rt_device_t aux_out_dev;

static struct rt_timer timer_vehicle;
static struct rt_event event_vehicle;

static void timer_vehicle_update(void* parameter)
{
    rt_event_send(&event_vehicle, EVENT_VEHICLE_UPDATE);
}

void task_vehicle_entry(void* parameter)
{
    static uint32_t time_start = 0;
    uint32_t time_now;
    uint32_t timestamp;
    rt_err_t res;
    rt_uint32_t recv_set = 0;
    uint32_t wait_set = EVENT_VEHICLE_UPDATE;

    while (1) {
        res = rt_event_recv(&event_vehicle, wait_set, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &recv_set);

        if (res == RT_EOK) {
            if (recv_set & EVENT_VEHICLE_UPDATE) {
                time_now = rt_tick_get_millisecond();
                /* record loop start time */
                if (time_start == 0) {
                    time_start = time_now;
                }
                /* the model simulation start from 0, so we calcualtet the timestamp relative to start time */
                timestamp = time_now - time_start;

#if !defined(HIL_SIM_MODE) && !defined(SIH_SIM_MODE)
                sensor_collect();
#endif
                pilot_cmd_collect();
                gcs_cmd_collect();
                mission_data_collect();

#ifdef SIH_SIM_MODE
                /* run Plant model */
                static uint32_t last_time_plant_step = 0;
                if(time_now - last_time_plant_step >=plant_model_info.period){
                    last_time_plant_step = time_now;
                    plant_interface_step(timestamp);
                }
#endif
                /* run INS model */
                static uint32_t last_time_ins_step = 0;
                if(time_now - last_time_ins_step >=ins_model_info.period){
                    last_time_ins_step = time_now;
                    ins_interface_step(timestamp);
                }
                /* run FMS model */
                static uint32_t last_time_fms_step = 0;
                if(time_now - last_time_fms_step >=fms_model_info.period){
                    last_time_fms_step = time_now;
                    fms_interface_step(timestamp);
                }
                /* run Controller model */
                static uint32_t last_time_control_step = 0;
                if(time_now - last_time_control_step >=control_model_info.period){
                    last_time_control_step = time_now;
                    control_interface_step(timestamp);
                }
                /* send actuator command */
                send_actuator_cmd();
            }
        }
    }
}


rt_err_t task_vehicle_init(void)
{
	
#if defined(SIH_SIM_MODE)
    /* init plant model */
    plant_interface_init();
#endif
	
    /* init ins model */
    ins_interface_init();

    /* init fms model */
    fms_interface_init();

    /* init controller model */
    control_interface_init();

    /* create event */
    if (rt_event_init(&event_vehicle, "vehicle", RT_IPC_FLAG_FIFO) != RT_EOK) {
        return RT_ERROR;
    }

    /* register timer event */
    rt_timer_init(&timer_vehicle, "vehicle", timer_vehicle_update, RT_NULL, 1, RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
    if (rt_timer_start(&timer_vehicle) != RT_EOK) {
        return RT_ERROR;
    }

    return RT_EOK;
}

int task_vehicle_create(void){
    RT_CHECK(task_vehicle_init());
    rt_thread_t tid = rt_thread_create("vehicle", 
        task_vehicle_entry, 
        RT_NULL, 
        5*1024, 
        VEHICLE_THREAD_PRIORITY, 
        10);
    if(tid!=RT_NULL){
        rt_thread_startup(tid);
        return 0;
    }
    return -1;
}
