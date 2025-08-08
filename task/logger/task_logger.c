
/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#include "autopilot.h"
#include <string.h>

#include "module/file_manager/file_manager.h"
#include "module/log/mlog.h"
#include "module/console/console.h"

#define TAG               "Logger"
#define ULOG_FILE_NAME    "ulog.txt"
#define EVENT_MLOG_UPDATE (1 << 0)
#define EVENT_ULOG_UPDATE (1 << 1)

static struct rt_event _log_event;

static void mlog_update_cb(void)
{
    rt_event_send(&_log_event, EVENT_MLOG_UPDATE);
}

static void ulog_update_cb(void)
{
    rt_event_send(&_log_event, EVENT_ULOG_UPDATE);
}

rt_err_t logger_start_mlog(char* path)
{
    char log_name[100];
    char file_name[50];
    static uint8_t mlog_id = 0;

    if (path) {
        /* if a valid path is provided, use it for mlog */
        return mlog_start(path);
    }

    if (current_log_session(log_name) != RT_EOK) {
        console_printf("no available log session\n");
        return RT_ERROR;
    }
    sprintf(file_name, "/mlog%d.bin", ++mlog_id);
    strcat(log_name, file_name);

    return mlog_start(log_name);
}

void logger_stop_mlog(void)
{
    mlog_stop();
}



void task_logger_entry(void* parameter)
{
    rt_err_t rt_err;
    rt_uint32_t recv_set = 0;
    rt_uint32_t wait_set = EVENT_MLOG_UPDATE | EVENT_ULOG_UPDATE;

    mlog_register_callback(MLOG_CB_UPDATE, mlog_update_cb);

    while (1) {
        /* wait event happen */
        rt_err = rt_event_recv(&_log_event, wait_set, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, 20, &recv_set);

        if (rt_err == RT_EOK) {
            if (recv_set & EVENT_MLOG_UPDATE) {
                mlog_async_output();
            }
        } else if (rt_err == -RT_ETIMEOUT) {
            /* if timeout, check if there are log data need to send */
            mlog_async_output();

        } else {
            /* some other error happen */
        }
    }
}

rt_err_t task_logger_init(void)
{
    /* create log event */
    if (rt_event_init(&_log_event, "logger", RT_IPC_FLAG_FIFO) != RT_EOK) {
        console_printf("log event create fail\n");
        return RT_ERROR;
    }

    /* init binary log */
    mlog_init();

    if (PARAM_GET_INT32(SYSTEM, MLOG_MODE) == 2 || PARAM_GET_INT32(SYSTEM, MLOG_MODE) == 3) {
        logger_start_mlog(NULL);
    }
    return RT_EOK;
}

int task_logger_create(void){
    rt_thread_t logger = rt_thread_create("logger", 
        task_logger_entry, 
        RT_NULL, 
        2048, 
        LOGGER_THREAD_PRIORITY, 
        20);
    if(logger!=RT_NULL){
        rt_thread_startup(logger);
        return 0;
    }
    return -1;
}