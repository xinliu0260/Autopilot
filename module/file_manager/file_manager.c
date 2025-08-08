/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

#include "autopilot.h"

#include "module/file_manager/file_manager.h"
#include <string.h>

#define DBG_TAG "file_manager"
#define DBG_LVL DBG_INFO
#include "rtdbg.h"

#define MAX_LOG_SESSION_NUM 20
#define LOG_SESSION_FILE    "/log/session_id"

static int cws_id; /* current work session id */

static const char* rfs_folder[] = {
    "/sys",
    "/usr",
    "/log",
    "/mnt",
    NULL /* NULL indicate the end */
};

/**
 * Read log session id from /log/session_id.
 * 
 * @return FMT Errors.
 */
static int read_log_session_id(void)
{
    char id_buffer[5] = { 0 };
    struct stat sta;
    int id = -1;
    int fd = -1;

    /* if file existed, read it */
    if (stat(LOG_SESSION_FILE, &sta) == 0) {
        int i;
        fd = open(LOG_SESSION_FILE, O_WRONLY);
        if (fd < 0) {
            return -1;
        }
        /* read session id from file */
        int rb = read(fd, id_buffer, sizeof(id_buffer) - 1);
        /* check if it's a valid number */
        for (i = 0; i < rb; i++) {
            /* session id starts from 1 */
            if (id_buffer[i] < '0' || id_buffer[i] > '9' || id_buffer[0] == '0') {
                /* illegal session id */
                close(fd);
                return -1;
            }
        }

        id = atoi(id_buffer);
        close(fd);
    }

    return id;
}

/**
 * Update log session id into /log/session_id.
 * 
 * @return FMT Errors.
 */
static rt_err_t update_log_session(void)
{
    int fd;

    /* read current log session id */
    cws_id = read_log_session_id();
    if (cws_id < 0) {
        cws_id = 0;
    }

    /* increment session id */
    cws_id = (cws_id % MAX_LOG_SESSION_NUM) + 1;
    /* open or create the log session id file */
    fd = open(LOG_SESSION_FILE, O_TRUNC | O_CREAT | O_RDWR);
    if (fd < 0) {
        LOG_E("fail to create log session id file! err:%ld\n", rt_get_errno());
        return RT_ERROR;
    }
    fm_fprintf(fd, "%d", cws_id);
    close(fd);

    return RT_EOK;
}

/**
 * Create working log session folder.
 * 
 * @return FMT Errors.
 */
static rt_err_t create_log_session(void)
{
    char path[50];
    struct stat sta;
    /* get log session full path */
    sprintf(path, "/log/session_%d", cws_id);

    if (stat(path, &sta) == 0) {
        /* delete existed folder */
        if (fm_deldir(path) < 0) {
            LOG_E("fail to delete %s\n", path);
            return RT_ERROR;
        }
    }
    /* create log session */
    if (mkdir(path, 0x777) < 0) {
        LOG_E("fail to create %s, errno:%ld\n", path, rt_get_errno());
        return RT_ERROR;
    }

    return RT_EOK;
}

/**
 * Create rootfs folders if not existed.
 * 
 * @return FMT Errors.
 */
static rt_err_t create_rootfs(void)
{

}

/**
 * Get the current log session path.
 * 
 * @param path store the full path of current log session
 * @return FMT Errors.
 */
rt_err_t current_log_session(char* path)
{
    if (cws_id > 0) {
        /* get log session full path */
        sprintf(path, "/log/session_%d", cws_id);
        return RT_EOK;
    } else {
        return RT_ERROR;
    }
}

/**
 * Initialize the file system.
 * 
 * @return FMT Errors.
 */
rt_err_t file_manager_init(void)
{
    struct stat buf;
    int i = 0;

    /* clear mnt directory */
    fm_deldir("/mnt");

    /* create rootfs folder structure */
    while (1) {
        if (rfs_folder[i] != NULL) {
            if (stat(rfs_folder[i], &buf) < 0) {
                if (mkdir(rfs_folder[i], 0x777) < 0) {
                    LOG_E("fail to create %s, errno:%ld\n", rfs_folder[i], rt_get_errno());
                    return RT_ERROR;
                }
            }
            i++;
        } else {
            break;
        }
    }

    /* update and create log session */
    if (update_log_session() == RT_EOK) {
        if (create_log_session() != RT_EOK) {
            return RT_ERROR;
        }
    } else {
        return RT_ERROR;
    }

    return RT_EOK;
}
