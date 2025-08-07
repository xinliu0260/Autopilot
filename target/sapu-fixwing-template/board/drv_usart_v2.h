/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-06-01     KyleChan     first version
 */

#ifndef __DRV_USART_V2_H__
#define __DRV_USART_V2_H__

#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>
#include <drv_common.h>


int rt_hw_usart_init(void);

#define UART_SET_TDR(__HANDLE__, __DATA__)  ((__HANDLE__)->Instance->TDR = (__DATA__))
#define UART_GET_RDR(__HANDLE__, MASK)            ((__HANDLE__)->Instance->RDR & MASK)

#define UART_INSTANCE_CLEAR_FUNCTION    __HAL_UART_CLEAR_IT


#define UART_RX_DMA_IT_IDLE_FLAG        0x00
#define UART_RX_DMA_IT_HT_FLAG          0x01
#define UART_RX_DMA_IT_TC_FLAG          0x02


/* stm32 config class */
struct stm32_uart_config
{
    const char *name;
    USART_TypeDef *Instance;
    IRQn_Type irq_type;
    struct dma_config *dma_rx;
    struct dma_config *dma_tx;
};

/* stm32 uart dirver class */
struct stm32_uart
{
    UART_HandleTypeDef handle;
    struct stm32_uart_config *config;
    struct
    {
        DMA_HandleTypeDef handle;
        rt_size_t remaining_cnt;
    } dma_rx;
    struct
    {
        DMA_HandleTypeDef handle;
    } dma_tx;
    rt_uint16_t uart_dma_flag;
    struct rt_serial_device serial;
};

#endif  /* __DRV_USART_H__ */
