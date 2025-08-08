#include "autopilot_def.h"
#include "rttypes.h"
#include <rtthread.h>
#include <stdint.h>
#include <stm32h7xx.h>


extern __IO uint32_t uwTick;
static uint32_t _systick_ms = 1;

/**
 * This is the timer interrupt service routine.
 *
 */
void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
        HAL_IncTick();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}

uint32_t HAL_GetTick(void)
{
    if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
        HAL_IncTick();

    return uwTick;
}

void HAL_IncTick(void)
{
    uwTick += _systick_ms;
}

void HAL_SuspendTick(void)
{
}

void HAL_ResumeTick(void)
{

    
}

void HAL_Delay(__IO uint32_t Delay)
{
    if (rt_thread_self())
    {
        rt_thread_mdelay(Delay);
    }
    else
    {
        for (rt_uint32_t count = 0; count < Delay; count++)
        {
            rt_hw_us_delay(1000);
        }
    }
}

/* SysTick configuration */
void rt_hw_systick_init(void)
{
    // Updates the variable SystemCoreClock
    SystemCoreClockUpdate();

    HAL_SYSTICK_Config(SystemCoreClock / RT_TICK_PER_SECOND);

    NVIC_SetPriority(SysTick_IRQn, 0xFF);

    _systick_ms = 1000u / RT_TICK_PER_SECOND;
    if (_systick_ms == 0)
        _systick_ms = 1;
}


/* re-implement tick interface for STM32 HAL */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    rt_hw_systick_init();

    /* Return function status */
    return HAL_OK;
}


void rt_hw_us_delay(rt_uint32_t us)
{
    rt_uint64_t ticks;
    rt_uint32_t told, tnow, tcnt = 0;
    rt_uint32_t reload = SysTick->LOAD;

    ticks = us * (reload / (1000000 / RT_TICK_PER_SECOND));
    told = SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}

uint64_t autopilot_system_time_us(void)
{
    uint32_t ms, reload_val, cnt;
    uint32_t current_ms;

    reload_val = SysTick->LOAD;

    // 解决时间同步问题：确保ms和cnt在同一时间窗口内
    do {
        ms = HAL_GetTick();
        cnt = SysTick->VAL;
        current_ms = HAL_GetTick();
    } while (current_ms != ms);  // 如果期间发生了ms进位，重新读取

    // 计算当前毫秒内已流逝的微秒数
    // 总周期数是reload_val + 1（从LOAD到0的完整计数）
    uint32_t us_in_ms = (reload_val - cnt) * 1000UL / (reload_val + 1);

    return (uint64_t)ms * 1000UL + us_in_ms;
}


uint32_t autopilot_system_time_ms(void){
    return rt_tick_get_millisecond();
}
