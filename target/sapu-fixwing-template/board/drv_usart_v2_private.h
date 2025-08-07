#pragma once

/* DMA1 stream0 */
#define UART1_DMA_TX_IRQHandler           DMA1_Stream0_IRQHandler
#define UART1_TX_DMA_RCC                  RCC_AHB1ENR_DMA1EN
#define UART1_TX_DMA_INSTANCE             DMA1_Stream0
#define UART1_TX_DMA_IRQ                  DMA1_Stream0_IRQn
#define UART1_TX_DMA_REQUEST              DMA_REQUEST_USART1_TX


/* DMA1 stream1 */
#define UART2_DMA_TX_IRQHandler           DMA1_Stream1_IRQHandler
#define UART2_TX_DMA_RCC                  RCC_AHB1ENR_DMA1EN
#define UART2_TX_DMA_INSTANCE             DMA1_Stream1
#define UART2_TX_DMA_IRQ                  DMA1_Stream1_IRQn
#define UART2_TX_DMA_REQUEST              DMA_REQUEST_USART2_TX

/* DMA1 stream2 */
#define UART3_DMA_TX_IRQHandler           DMA1_Stream2_IRQHandler
#define UART3_TX_DMA_RCC                  RCC_AHB1ENR_DMA1EN
#define UART3_TX_DMA_INSTANCE             DMA1_Stream2
#define UART3_TX_DMA_IRQ                  DMA1_Stream2_IRQn
#define UART3_TX_DMA_REQUEST              DMA_REQUEST_USART3_TX

/* DMA1 stream3 */
#define UART4_DMA_TX_IRQHandler           DMA1_Stream3_IRQHandler
#define UART4_TX_DMA_RCC                  RCC_AHB1ENR_DMA1EN
#define UART4_TX_DMA_INSTANCE             DMA1_Stream3
#define UART4_TX_DMA_IRQ                  DMA1_Stream3_IRQn
#define UART4_TX_DMA_REQUEST              DMA_REQUEST_UART4_TX

/* DMA1 stream4 */
#define UART5_DMA_TX_IRQHandler           DMA1_Stream4_IRQHandler
#define UART5_TX_DMA_RCC                  RCC_AHB1ENR_DMA1EN
#define UART5_TX_DMA_INSTANCE             DMA1_Stream4
#define UART5_TX_DMA_IRQ                  DMA1_Stream4_IRQn
#define UART5_TX_DMA_REQUEST              DMA_REQUEST_UART5_TX

/* DMA1 stream5 */
#define UART6_DMA_TX_IRQHandler           DMA1_Stream5_IRQHandler
#define UART6_TX_DMA_RCC                  RCC_AHB1ENR_DMA1EN
#define UART6_TX_DMA_INSTANCE             DMA1_Stream5
#define UART6_TX_DMA_IRQ                  DMA1_Stream5_IRQn
#define UART6_TX_DMA_REQUEST              DMA_REQUEST_USART6_TX

/* DMA1 stream6 */
#define UART7_DMA_TX_IRQHandler           DMA1_Stream6_IRQHandler
#define UART7_TX_DMA_RCC                  RCC_AHB1ENR_DMA1EN
#define UART7_TX_DMA_INSTANCE             DMA1_Stream6
#define UART7_TX_DMA_IRQ                  DMA1_Stream6_IRQn
#define UART7_TX_DMA_REQUEST              DMA_REQUEST_UART7_TX

/* DMA1 stream7 */
#define UART8_DMA_TX_IRQHandler           DMA1_Stream7_IRQHandler
#define UART8_TX_DMA_RCC                  RCC_AHB1ENR_DMA1EN
#define UART8_TX_DMA_INSTANCE             DMA1_Stream7
#define UART8_TX_DMA_IRQ                  DMA1_Stream7_IRQn
#define UART8_TX_DMA_REQUEST              DMA_REQUEST_UART8_TX


/* DMA2 stream0 */
#define UART1_DMA_RX_IRQHandler          DMA2_Stream0_IRQHandler
#define UART1_RX_DMA_RCC                 RCC_AHB1ENR_DMA2EN
#define UART1_RX_DMA_INSTANCE            DMA2_Stream0
#define UART1_RX_DMA_REQUEST             DMA_REQUEST_USART1_RX
#define UART1_RX_DMA_IRQ                 DMA2_Stream0_IRQn

/* DMA2 stream1 */
#define UART2_DMA_RX_IRQHandler          DMA2_Stream1_IRQHandler
#define UART2_RX_DMA_RCC                 RCC_AHB1ENR_DMA2EN
#define UART2_RX_DMA_INSTANCE            DMA2_Stream1
#define UART2_RX_DMA_REQUEST             DMA_REQUEST_USART2_RX
#define UART2_RX_DMA_IRQ                 DMA2_Stream1_IRQn

/* DMA2 stream2 */
#define UART3_DMA_RX_IRQHandler          DMA2_Stream2_IRQHandler
#define UART3_RX_DMA_RCC                 RCC_AHB1ENR_DMA2EN
#define UART3_RX_DMA_INSTANCE            DMA2_Stream2
#define UART3_RX_DMA_REQUEST             DMA_REQUEST_USART3_RX
#define UART3_RX_DMA_IRQ                 DMA2_Stream2_IRQn

/* DMA2 stream3 */
#define UART4_DMA_RX_IRQHandler          DMA2_Stream3_IRQHandler
#define UART4_RX_DMA_RCC                 RCC_AHB1ENR_DMA2EN
#define UART4_RX_DMA_INSTANCE            DMA2_Stream3
#define UART4_RX_DMA_REQUEST             DMA_REQUEST_UART4_RX
#define UART4_RX_DMA_IRQ                 DMA2_Stream3_IRQn

/* DMA2 stream4 */
#define UART5_DMA_RX_IRQHandler          DMA2_Stream4_IRQHandler
#define UART5_RX_DMA_RCC                 RCC_AHB1ENR_DMA2EN
#define UART5_RX_DMA_INSTANCE            DMA2_Stream4
#define UART5_RX_DMA_REQUEST             DMA_REQUEST_UART5_RX
#define UART5_RX_DMA_IRQ                 DMA2_Stream4_IRQn

/* DMA2 stream5 */
#define UART6_DMA_RX_IRQHandler          DMA2_Stream5_IRQHandler
#define UART6_RX_DMA_RCC                 RCC_AHB1ENR_DMA2EN
#define UART6_RX_DMA_INSTANCE            DMA2_Stream5
#define UART6_RX_DMA_REQUEST             DMA_REQUEST_USART6_RX
#define UART6_RX_DMA_IRQ                 DMA2_Stream5_IRQn

/* DMA2 stream6 */
#define UART7_DMA_RX_IRQHandler          DMA2_Stream6_IRQHandler
#define UART7_RX_DMA_RCC                 RCC_AHB1ENR_DMA2EN
#define UART7_RX_DMA_INSTANCE            DMA2_Stream6
#define UART7_RX_DMA_REQUEST             DMA_REQUEST_UART7_RX
#define UART7_RX_DMA_IRQ                 DMA2_Stream6_IRQn

/* DMA2 stream7 */
#define UART8_DMA_RX_IRQHandler          DMA2_Stream7_IRQHandler
#define UART8_RX_DMA_RCC                 RCC_AHB1ENR_DMA2EN
#define UART8_RX_DMA_INSTANCE            DMA2_Stream7
#define UART8_RX_DMA_REQUEST             DMA_REQUEST_UART8_RX
#define UART8_RX_DMA_IRQ                 DMA2_Stream7_IRQn


#define BSP_UART1_RX_BUFSIZE 256
#define BSP_UART1_TX_BUFSIZE 256

//struct stm32_uart_config
#define UART1_CONFIG                                                \
    {                                                               \
        .name = "serial1",                                          \
        .Instance = USART1,                                         \
        .irq_type = USART1_IRQn,                                    \
    }
//struct serial_configure
#define UART1_SERIAL_USER_CONFIGS                     \
{                                                     \
    BAUD_RATE_230400,           /* 230400 bits/s */   \
    DATA_BITS_8,                /* 8 databits */      \
    STOP_BITS_1,                /* 1 stopbit */       \
    PARITY_NONE,                /* No parity  */      \
    BIT_ORDER_LSB,              /* LSB first sent */  \
    NRZ_NORMAL,                 /* Normal mode */     \
    512,      /* rxBuf size */       \
    512,      /* txBuf size */       \
    RT_SERIAL_FLOWCONTROL_NONE, /* Off flowcontrol */ \
    0                                                 \
}
//struct stm32_uart_config.struct dma_config
#define UART1_DMA_RX_CONFIG                                         \
    {                                                               \
        .Instance = UART1_RX_DMA_INSTANCE,                          \
        .request = UART1_RX_DMA_REQUEST,                            \
        .dma_rcc = UART1_RX_DMA_RCC,                                \
        .dma_irq = UART1_RX_DMA_IRQ,                                \
    }

//struct stm32_uart_config.struct dma_config
#define UART1_DMA_TX_CONFIG                                         \
    {                                                               \
        .Instance = UART1_TX_DMA_INSTANCE,                          \
        .request = UART1_TX_DMA_REQUEST,                            \
        .dma_rcc = UART1_TX_DMA_RCC,                                \
        .dma_irq = UART1_TX_DMA_IRQ,                                \
    }

#define BSP_UART2_RX_BUFSIZE 1024
#define BSP_UART2_TX_BUFSIZE 2048

#define UART2_CONFIG                                                \
    {                                                               \
        .name = "serial2",                                            \
        .Instance = USART2,                                         \
        .irq_type = USART2_IRQn,                                    \
    }
#define UART2_SERIAL_USER_CONFIGS                     \
{                                                     \
    BAUD_RATE_230400,           /* 230400 bits/s */   \
    DATA_BITS_8,                /* 8 databits */      \
    STOP_BITS_1,                /* 1 stopbit */       \
    PARITY_NONE,                /* No parity  */      \
    BIT_ORDER_LSB,              /* LSB first sent */  \
    NRZ_NORMAL,                 /* Normal mode */     \
    BSP_UART2_RX_BUFSIZE,      /* rxBuf size */       \
    BSP_UART2_TX_BUFSIZE,      /* txBuf size */       \
    RT_SERIAL_FLOWCONTROL_NONE, /* Off flowcontrol */ \
    0                                                 \
}
#define UART2_DMA_RX_CONFIG                                         \
    {                                                               \
        .Instance = UART2_RX_DMA_INSTANCE,                          \
        .request = UART2_RX_DMA_REQUEST,                            \
        .dma_rcc = UART2_RX_DMA_RCC,                                \
        .dma_irq = UART2_RX_DMA_IRQ,                                \
    }

#define UART2_DMA_TX_CONFIG                                         \
    {                                                               \
        .Instance = UART2_TX_DMA_INSTANCE,                          \
        .request = UART2_TX_DMA_REQUEST,                            \
        .dma_rcc = UART2_TX_DMA_RCC,                                \
        .dma_irq = UART2_TX_DMA_IRQ,                                \
    }


#define BSP_UART3_RX_BUFSIZE 256
#define BSP_UART3_TX_BUFSIZE 256

#define UART3_CONFIG                                                \
    {                                                               \
        .name = "serial3",                                            \
        .Instance = USART3,                                         \
        .irq_type = USART3_IRQn,                                    \
    }

#define UART3_SERIAL_USER_CONFIGS \
{                                                     \
    BAUD_RATE_115200,           /* 115200 bits/s */   \
    DATA_BITS_8,                /* 8 databits */      \
    STOP_BITS_1,                /* 1 stopbit */       \
    PARITY_NONE,                /* No parity  */      \
    BIT_ORDER_LSB,              /* LSB first sent */  \
    NRZ_NORMAL,                 /* Normal mode */     \
    BSP_UART3_RX_BUFSIZE,      /* rxBuf size */       \
    BSP_UART3_TX_BUFSIZE,      /* txBuf size */       \
    RT_SERIAL_FLOWCONTROL_NONE, /* Off flowcontrol */ \
    0                                                 \
}

#define UART3_DMA_RX_CONFIG                                         \
    {                                                               \
        .Instance = UART3_RX_DMA_INSTANCE,                          \
        .request = UART3_RX_DMA_REQUEST,                            \
        .dma_rcc = UART3_RX_DMA_RCC,                                \
        .dma_irq = UART3_RX_DMA_IRQ,                                \
    }

#define UART3_DMA_TX_CONFIG                                         \
    {                                                               \
        .Instance = UART3_TX_DMA_INSTANCE,                          \
        .request = UART3_TX_DMA_REQUEST,                            \
        .dma_rcc = UART3_TX_DMA_RCC,                                \
        .dma_irq = UART3_TX_DMA_IRQ,                                \
    }

#define BSP_UART4_RX_BUFSIZE 256
#define BSP_UART4_TX_BUFSIZE 256

#define UART4_CONFIG                                                \
    {                                                               \
        .name = "serial4",                                            \
        .Instance = UART4,                                          \
        .irq_type = UART4_IRQn,                                     \
    }

#define UART4_SERIAL_USER_CONFIGS \
{                                                     \
    BAUD_RATE_115200,           /* 115200 bits/s */   \
    DATA_BITS_8,                /* 8 databits */      \
    STOP_BITS_1,                /* 1 stopbit */       \
    PARITY_NONE,                /* No parity  */      \
    BIT_ORDER_LSB,              /* LSB first sent */  \
    NRZ_NORMAL,                 /* Normal mode */     \
    BSP_UART4_RX_BUFSIZE,      /* rxBuf size */       \
    BSP_UART4_TX_BUFSIZE,      /* txBuf size */       \
    RT_SERIAL_FLOWCONTROL_NONE, /* Off flowcontrol */ \
    0                                                 \
}

#define UART4_DMA_RX_CONFIG                                         \
    {                                                               \
        .Instance = UART4_RX_DMA_INSTANCE,                          \
        .request = UART4_RX_DMA_REQUEST,                            \
        .dma_rcc = UART4_RX_DMA_RCC,                                \
        .dma_irq = UART4_RX_DMA_IRQ,                                \
    }

#define UART4_DMA_TX_CONFIG                                         \
    {                                                               \
        .Instance = UART4_TX_DMA_INSTANCE,                          \
        .request = UART4_TX_DMA_REQUEST,                            \
        .dma_rcc = UART4_TX_DMA_RCC,                                \
        .dma_irq = UART4_TX_DMA_IRQ,                                \
    }

#define BSP_UART5_RX_BUFSIZE 256
#define BSP_UART5_TX_BUFSIZE 256
#define UART5_CONFIG                                                \
    {                                                               \
        .name = "serial5",                                            \
        .Instance = UART5,                                          \
        .irq_type = UART5_IRQn,                                     \
    }

#define UART5_DMA_RX_CONFIG                                         \
    {                                                               \
        .Instance = UART5_RX_DMA_INSTANCE,                          \
        .request = UART5_RX_DMA_REQUEST,                            \
        .dma_rcc = UART5_RX_DMA_RCC,                                \
        .dma_irq = UART5_RX_DMA_IRQ,                                \
    }

#define UART5_SERIAL_USER_CONFIGS \
{                                                     \
    100000,           		        /* 100000 bits/s */   \
    DATA_BITS_9,                /* 9 databits */      \
    STOP_BITS_2,                /* 1 stopbit */       \
    PARITY_EVEN,                /* No parity  */      \
    BIT_ORDER_LSB,              /* LSB first sent */  \
    NRZ_NORMAL,                 /* Normal mode */     \
    BSP_UART5_RX_BUFSIZE,      /* rxBuf size */       \
    BSP_UART5_TX_BUFSIZE,      /* txBuf size */       \
    RT_SERIAL_FLOWCONTROL_NONE, /* Off flowcontrol */ \
    0                                                 \
}

#define UART5_DMA_TX_CONFIG                                         \
    {                                                               \
        .Instance = UART5_TX_DMA_INSTANCE,                          \
        .request = UART5_TX_DMA_REQUEST,                            \
        .dma_rcc = UART5_TX_DMA_RCC,                                \
        .dma_irq = UART5_TX_DMA_IRQ,                                \
    }

#define BSP_UART6_RX_BUFSIZE 1024
#define BSP_UART6_TX_BUFSIZE 1024

#define UART6_CONFIG                                                \
    {                                                               \
        .name = "serial6",                                            \
        .Instance = USART6,                                         \
        .irq_type = USART6_IRQn,                                    \
    }

#define UART6_SERIAL_USER_CONFIGS \
{                                                     \
    BAUD_RATE_230400,           /* 230400 bits/s */   \
    DATA_BITS_8,                /* 8 databits */      \
    STOP_BITS_1,                /* 1 stopbit */       \
    PARITY_NONE,                /* No parity  */      \
    BIT_ORDER_LSB,              /* LSB first sent */  \
    NRZ_NORMAL,                 /* Normal mode */     \
    BSP_UART6_RX_BUFSIZE,      /* rxBuf size */       \
    BSP_UART6_TX_BUFSIZE,      /* txBuf size */       \
    RT_SERIAL_FLOWCONTROL_NONE, /* Off flowcontrol */ \
    0                                                 \
}

#define UART6_DMA_RX_CONFIG                                         \
    {                                                               \
        .Instance = UART6_RX_DMA_INSTANCE,                          \
        .request = UART6_RX_DMA_REQUEST,                            \
        .dma_rcc = UART6_RX_DMA_RCC,                                \
        .dma_irq = UART6_RX_DMA_IRQ,                                \
    }

#define UART6_DMA_TX_CONFIG                                         \
    {                                                               \
        .Instance = UART6_TX_DMA_INSTANCE,                          \
        .request = UART6_TX_DMA_REQUEST,                            \
        .dma_rcc = UART6_TX_DMA_RCC,                                \
        .dma_irq = UART6_TX_DMA_IRQ,                                \
    }


#define BSP_UART7_RX_BUFSIZE 256
#define BSP_UART7_TX_BUFSIZE 256

#define UART7_CONFIG                                                \
    {                                                               \
        .name = "serial7",                                            \
        .Instance = UART7,                                          \
        .irq_type = UART7_IRQn,                                     \
    }

#define UART7_SERIAL_USER_CONFIGS \
{                                                     \
    BAUD_RATE_115200,           /* 115200 bits/s */   \
    DATA_BITS_8,                /* 8 databits */      \
    STOP_BITS_1,                /* 1 stopbit */       \
    PARITY_NONE,                /* No parity  */      \
    BIT_ORDER_LSB,              /* LSB first sent */  \
    NRZ_NORMAL,                 /* Normal mode */     \
    BSP_UART7_RX_BUFSIZE,      /* rxBuf size */       \
    BSP_UART7_TX_BUFSIZE,      /* txBuf size */       \
    RT_SERIAL_FLOWCONTROL_NONE, /* Off flowcontrol */ \
    0                                                 \
}

#define UART7_DMA_RX_CONFIG                                         \
    {                                                               \
        .Instance = UART7_RX_DMA_INSTANCE,                          \
        .request = UART7_RX_DMA_REQUEST,                            \
        .dma_rcc = UART7_RX_DMA_RCC,                                \
        .dma_irq = UART7_RX_DMA_IRQ,                                \
    }

#define UART7_DMA_TX_CONFIG                                         \
    {                                                               \
        .Instance = UART7_TX_DMA_INSTANCE,                          \
        .request = UART7_TX_DMA_REQUEST,                            \
        .dma_rcc = UART7_TX_DMA_RCC,                                \
        .dma_irq = UART7_TX_DMA_IRQ,                                \
    }

#define BSP_UART8_RX_BUFSIZE 256
#define BSP_UART8_TX_BUFSIZE 256

#define UART8_CONFIG                                                \
    {                                                               \
        .name = "serial8",                                            \
        .Instance = UART8,                                          \
        .irq_type = UART8_IRQn,                                     \
    }

#define UART8_SERIAL_USER_CONFIGS \
{                                                     \
    BAUD_RATE_115200,           /* 115200 bits/s */   \
    DATA_BITS_8,                /* 8 databits */      \
    STOP_BITS_1,                /* 1 stopbit */       \
    PARITY_NONE,                /* No parity  */      \
    BIT_ORDER_LSB,              /* LSB first sent */  \
    NRZ_NORMAL,                 /* Normal mode */     \
    BSP_UART8_RX_BUFSIZE,      /* rxBuf size */       \
    BSP_UART8_TX_BUFSIZE,      /* txBuf size */       \
    RT_SERIAL_FLOWCONTROL_NONE, /* Off flowcontrol */ \
    0                                                 \
}

#define UART8_DMA_RX_CONFIG                                         \
    {                                                               \
        .Instance = UART8_RX_DMA_INSTANCE,                          \
        .request = UART8_RX_DMA_REQUEST,                            \
        .dma_rcc = UART8_RX_DMA_RCC,                                \
        .dma_irq = UART8_RX_DMA_IRQ,                                \
    }

#define UART8_DMA_TX_CONFIG                                         \
    {                                                               \
        .Instance = UART8_TX_DMA_INSTANCE,                          \
        .request = UART8_TX_DMA_REQUEST,                            \
        .dma_rcc = UART8_TX_DMA_RCC,                                \
        .dma_irq = UART8_TX_DMA_IRQ,                                \
    }


