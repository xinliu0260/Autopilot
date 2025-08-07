/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-10     SummerGift   first version
 * 2020-10-14     Dozingfiretruck   Porting for stm32wbxx
 */

#ifndef __DRV_DMA_H_
#define __DRV_DMA_H_

#include <rtthread.h>
#include <board.h>

#ifdef __cplusplus
extern "C" {
#endif


struct dma_config {
    DMA_Stream_TypeDef *Instance;
    rt_uint32_t dma_rcc;
    IRQn_Type dma_irq;
    rt_uint32_t request;
};

#ifdef __cplusplus
}
#endif

#endif /*__DRV_DMA_H_ */
