/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-03-05     whj4674672   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define GREEN_LED_PIN    GET_PIN(A, 1)
#define RED_LED_PIN    GET_PIN(A, 2)
#define BLUE_LED_PIN    GET_PIN(A, 3)

int main(void)
{
    int count = 1;
    /* set LED0 pin mode to output */	
    rt_pin_mode(GREEN_LED_PIN, PIN_MODE_OUTPUT);	
	
    while (count++)
    {
        rt_pin_write(GREEN_LED_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(GREEN_LED_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
    return RT_EOK;
}
