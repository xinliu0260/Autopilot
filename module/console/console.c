/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

#include "autopilot.h"
#include <rtdevice.h>
#include <string.h>
#include "stdio.h"


#define CONSOLE_BUFF_SIZE 1024

/* console write hook function, can be reimplemented by other modules. */
rt_weak void console_write_hook(const char* content, uint32_t len);

static rt_device_t console_dev;
static char console_buffer[CONSOLE_BUFF_SIZE];

/**
 * Write raw data to console device.
 *
 * @param content data to be written
 * @param len length of data
 *
 * @return length has been written.
 */
int console_write(const char* content, uint32_t len)
{
    if (console_dev == NULL) {
        return 0;
    }

    /* write content into console device */
    uint32_t size = rt_device_write(console_dev, 0, (void*)content, len);

    /* call write hook */
    if (console_write_hook)
        console_write_hook(content, len);

    return size;
}

/**
 * Console print arguments.
 *
 * @param fmt string format
 * @param args arguments list
 *
 * @return length of output.
 */
int console_print_args(const char* fmt, va_list args)
{
    int length;

    length = vsnprintf(console_buffer, CONSOLE_BUFF_SIZE, fmt, args);

    return console_write(console_buffer, length);
}

/**
 * Console printf.
 *
 * @param fmt string format
 *
 * @return length of output.
 */
int console_printf(const char* fmt, ...)
{
    va_list args;
    int length;

    va_start(args, fmt);
    length = vsnprintf(console_buffer, CONSOLE_BUFF_SIZE, fmt, args);
    va_end(args);

    return console_write(console_buffer, length);
}

/**
 * Console print line.
 *
 * @param fmt string format
 * @return length of output.
 */
int console_println(const char* fmt, ...)
{
    va_list args;
    int length;

    va_start(args, fmt);
    length = vsnprintf(console_buffer, CONSOLE_BUFF_SIZE - 1, fmt, args);
    va_end(args);

    console_buffer[length++] = '\n';

    return console_write(console_buffer, length);
}

/**
 * Format string into the buffer.
 */
void console_format(char* buffer, const char* fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vsprintf(buffer, fmt, args);
    va_end(args);
}

/**
 * Get current console device.
 *
 * @return current console device. NULL indicates no device.
 */
rt_device_t console_get_device(void)
{
    return console_dev;
}

/**
 * Set console to a specific device.
 * @note console input/output will be enabled as well
 *
 * @return RT Errors.
 */
rt_err_t console_set_device(const char* device_name)
{
    rt_device_t new;
    rt_err_t err;

    new = rt_device_find(device_name);
    if (new == RT_NULL) {
        /* can not find console device */
        return RT_EINVAL;
    }

    /* switch console to new device */
    console_dev = new;

    return err;
}

