/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-5      SummerGift   first version
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#include <rtthread.h>
#include <stm32h7xx.h>
#include "drv_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define STM32_FLASH_START_ADRESS     ((uint32_t)0x08000000)
#define STM32_FLASH_SIZE             (2048 * 1024)
#define STM32_FLASH_END_ADDRESS      ((uint32_t)(STM32_FLASH_START_ADRESS + STM32_FLASH_SIZE))

#define STM32_SRAM_SIZE           (512)
#define STM32_SRAM_END            (0x24000000 + STM32_SRAM_SIZE * 1024)

#if defined(__ARMCC_VERSION)
extern int Image$$RW_IRAM1$$ZI$$Limit;
#define HEAP_BEGIN      (&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
#pragma section="CSTACK"
#define HEAP_BEGIN      (__segment_end("CSTACK"))
#else
extern int __bss_end;
#define HEAP_BEGIN      (&__bss_end)
#endif

#define HEAP_END        STM32_SRAM_END

void _Error_Handler(char *s, int num);

#ifndef Error_Handler
#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#endif

#define DCACHE_LINE_SIZE 32

/**
 * @brief 安全的D-Cache按地址无效化函数（处理非对齐地址）
 * @param addr 起始地址（可以是非对齐）
 * @param dsize 数据大小（字节）
 * @note 解决非32字节对齐地址导致的无效内存区域被无效化问题
 */
__STATIC_FORCEINLINE void SCB_SafeInvalidateDCache_by_Addr(volatile void *addr, int32_t dsize)
{
#if defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
	if (dsize <= 0 || addr == RT_NULL) return;

	uint32_t addr_align = (uint32_t)addr;
	// 检查地址是否已按32字节对齐
	if ((addr_align & (DCACHE_LINE_SIZE - 1)) == 0)
	{
		SCB_InvalidateDCache_by_Addr((uint32_t *)addr, dsize);
		return;
	}
	// 计算需要处理的完整范围（包含非对齐部分）
	uint32_t start_offset = addr_align & (DCACHE_LINE_SIZE - 1); 
	uint32_t end_addr = addr_align + dsize;
	uint32_t end_offset = (DCACHE_LINE_SIZE - (end_addr & (DCACHE_LINE_SIZE - 1))) & (DCACHE_LINE_SIZE - 1);
	uint32_t total_size = dsize + start_offset + end_offset;
	uint32_t new_start = addr_align - start_offset;
	uint32_t new_end = end_addr + end_offset;
	uint8_t head_temp[32];
	uint8_t tail_temp[32];
	rt_memcpy(head_temp, (void*)new_start, start_offset);
	rt_memcpy(tail_temp, (void*)end_addr, end_offset);
	__DSB();
	SCB_InvalidateDCache_by_Addr((uint32_t *)new_start, total_size);
	rt_memcpy((void*)new_start, (void*)head_temp, start_offset);
	rt_memcpy((void*)end_addr, (void*)tail_temp, end_offset);
#endif
}

#ifdef __cplusplus
}
#endif

#endif
