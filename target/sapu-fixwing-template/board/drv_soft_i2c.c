/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-08     balanceTWK   first version
 * 2023-06-27     Meco Man     replace stm32_udelay as rt_hw_us_delay
 */

#include "drv_soft_i2c.h"
#include "drivers/dev_i2c_bit_ops.h"
#include "drv_gpio.h"

#if defined(BSP_USING_I2C1) 


#define DBG_TAG     "drv.i2c"
#define DBG_LVL     DBG_INFO
#include <rtdbg.h>

struct stm32_i2c i2c1;
struct stm32_soft_i2c_config i2c1_cfg;
/**
 * This function initializes the i2c pin.
 *
 * @param Stm32 i2c dirver class.
 */
static void stm32_i2c_pin_init(void)
{
    rt_pin_mode(i2c1_cfg.scl, PIN_MODE_OUTPUT_OD);
    rt_pin_mode(i2c1_cfg.sda, PIN_MODE_OUTPUT_OD);

    rt_pin_write(i2c1_cfg.scl, PIN_HIGH);
    rt_pin_write(i2c1_cfg.sda, PIN_HIGH);
}

/**
 * This function sets the sda pin.
 *
 * @param Stm32 config class.
 * @param The sda pin state.
 */
static void stm32_set_sda(void *data, rt_int32_t state)
{
    struct stm32_soft_i2c_config* cfg = (struct stm32_soft_i2c_config*)data;
    if (state)
    {
        rt_pin_write(cfg->sda, PIN_HIGH);
    }
    else
    {
        rt_pin_write(cfg->sda, PIN_LOW);
    }
}

/**
 * This function sets the scl pin.
 *
 * @param Stm32 config class.
 * @param The scl pin state.
 */
static void stm32_set_scl(void *data, rt_int32_t state)
{
    struct stm32_soft_i2c_config* cfg = (struct stm32_soft_i2c_config*)data;
    if (state)
    {
        rt_pin_write(cfg->scl, PIN_HIGH);
    }
    else
    {
        rt_pin_write(cfg->scl, PIN_LOW);
    }
}

/**
 * This function gets the sda pin state.
 *
 * @param The sda pin state.
 */
static rt_int32_t stm32_get_sda(void *data)
{
    struct stm32_soft_i2c_config* cfg = (struct stm32_soft_i2c_config*)data;
    return rt_pin_read(cfg->sda);
}

/**
 * This function gets the scl pin state.
 *
 * @param The scl pin state.
 */
static rt_int32_t stm32_get_scl(void *data)
{
    struct stm32_soft_i2c_config* cfg = (struct stm32_soft_i2c_config*)data;
    return rt_pin_read(cfg->scl);
}

static const struct rt_i2c_bit_ops stm32_bit_ops_default =
{
    .data     = RT_NULL,
    .pin_init = stm32_i2c_pin_init,
    .set_sda  = stm32_set_sda,
    .set_scl  = stm32_set_scl,
    .get_sda  = stm32_get_sda,
    .get_scl  = stm32_get_scl,
    .udelay   = rt_hw_us_delay,
    .delay_us = 1,
    .timeout  = 100,
    .i2c_pin_init_flag = RT_FALSE
};

/**
 * if i2c is locked, this function will unlock it
 *
 * @param stm32 config class
 *
 * @return RT_EOK indicates successful unlock.
 */
static rt_err_t stm32_i2c_bus_unlock(const struct stm32_soft_i2c_config *cfg)
{
    rt_int32_t i = 0;

    if (PIN_LOW == rt_pin_read(cfg->sda))
    {
        while (i++ < 9)
        {
            rt_pin_write(cfg->scl, PIN_HIGH);
            rt_hw_us_delay(100);
            rt_pin_write(cfg->scl, PIN_LOW);
            rt_hw_us_delay(100);
        }
    }
    if (PIN_LOW == rt_pin_read(cfg->sda))
    {
        return -RT_ERROR;
    }

    return RT_EOK;
}

/* I2C initialization function */
int rt_hw_i2c_init(void)
{
    rt_err_t result;
    i2c1_cfg.bus_name = "i2c1";
    i2c1_cfg.scl = GET_PIN(F, 1);
    i2c1_cfg.sda = GET_PIN(F, 0);
    i2c1.ops = stm32_bit_ops_default;
    i2c1.ops.data = (void*)&i2c1_cfg;
    i2c1.i2c_bus.priv = &i2c1.ops;
    RT_ASSERT(result == RT_EOK);
    stm32_i2c_bus_unlock(&i2c1_cfg);

    LOG_D("software simulation %s init done, pin scl: %d, pin sda %d",
                        i2c1_cfg.bus_name,
                        i2c1_cfg.scl,
                        i2c1_cfg.sda);
    result = rt_i2c_bit_add_bus(&i2c1.i2c_bus, i2c1_cfg.bus_name);

    return RT_EOK;
}
INIT_BOARD_EXPORT(rt_hw_i2c_init);

#endif 
