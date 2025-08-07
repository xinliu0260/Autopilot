
#pragma once

#include <rtthread.h>

#ifdef BSP_USING_SPI1
#ifndef SPI1_BUS_CONFIG
#define SPI1_BUS_CONFIG                             \
    {                                               \
        .Instance = SPI1,                           \
        .bus_name = "spi1",                         \
        .irq_type = SPI1_IRQn,                      \
    }
#endif /* SPI1_BUS_CONFIG */
#endif /* BSP_USING_SPI1 */

#ifdef BSP_SPI1_TX_USING_DMA
#ifndef SPI1_TX_DMA_CONFIG
#define SPI1_TX_DMA_CONFIG                          \
    {                                               \
        .dma_rcc = SPI1_TX_DMA_RCC,                 \
        .Instance = SPI1_TX_DMA_INSTANCE,           \
        .dma_irq = SPI1_TX_DMA_IRQ,                 \
        .request = DMA_REQUEST_SPI1_TX              \
    }
#endif /* SPI1_TX_DMA_CONFIG */
#endif /* BSP_SPI1_TX_USING_DMA */

#ifdef BSP_SPI1_RX_USING_DMA
#ifndef SPI1_RX_DMA_CONFIG
#define SPI1_RX_DMA_CONFIG                          \
    {                                               \
        .dma_rcc = SPI1_RX_DMA_RCC,                 \
        .Instance = SPI1_RX_DMA_INSTANCE,           \
        .dma_irq = SPI1_RX_DMA_IRQ,                 \
        .request = DMA_REQUEST_SPI1_RX              \
    }
#endif /* SPI1_RX_DMA_CONFIG */
#endif /* BSP_SPI1_RX_USING_DMA */


#ifdef BSP_USING_SPI3
#ifndef SPI3_BUS_CONFIG
#define SPI3_BUS_CONFIG                             \
    {                                               \
        .Instance = SPI3,                           \
        .bus_name = "spi3",                         \
        .irq_type = SPI3_IRQn,                      \
    }
#endif /* SPI3_BUS_CONFIG */
#endif /* BSP_USING_SPI3 */

#ifdef BSP_SPI3_TX_USING_DMA
#ifndef SPI3_TX_DMA_CONFIG
#define SPI3_TX_DMA_CONFIG                          \
    {                                               \
        .dma_rcc = SPI3_TX_DMA_RCC,                 \
        .Instance = SPI3_TX_DMA_INSTANCE,           \
        .dma_irq = SPI3_TX_DMA_IRQ,                 \
        .request = DMA_REQUEST_SPI3_TX              \
    }
#endif /* SPI3_TX_DMA_CONFIG */
#endif /* BSP_SPI3_TX_USING_DMA */

#ifdef BSP_SPI3_RX_USING_DMA
#ifndef SPI3_RX_DMA_CONFIG
#define SPI3_RX_DMA_CONFIG                          \
    {                                               \
        .dma_rcc = SPI3_RX_DMA_RCC,                 \
        .Instance = SPI3_RX_DMA_INSTANCE,           \
        .dma_irq = SPI3_RX_DMA_IRQ,                 \
        .request = DMA_REQUEST_SPI3_RX              \
    }
#endif /* SPI3_RX_DMA_CONFIG */
#endif /* BSP_SPI3_RX_USING_DMA */


