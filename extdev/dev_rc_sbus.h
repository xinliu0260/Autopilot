/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#ifndef DEV_RC_SBUS_H
#define DEV_RC_SBUS_H

#include <stdint.h>
#include <stdbool.h>

// SBUS协议相关定义
#define SBUS_PACKET_SIZE 25      // SBUS数据包大小
#define SBUS_CHANNELS 16         // 通道数量
#define SBUS_START_BYTE 0x0F     // 数据包起始字节
#define SBUS_END_BYTE 0x00       // 数据包结束字节

// SBUS接收状态枚举
typedef enum {
    SBUS_STATE_WAIT_START,       // 等待起始字节
    SBUS_STATE_RECEIVE_DATA,     // 接收数据字节
    SBUS_STATE_RECEIVE_END       // 接收结束字节
} SBusState;

// SBUS接收结构体
typedef struct {
    SBusState state;             // 接收状态
    uint8_t buffer[SBUS_PACKET_SIZE]; // 数据缓冲区
    uint8_t buffer_idx;          // 缓冲区索引
    uint16_t channels[SBUS_CHANNELS]; // 解析后的通道数据
    bool data_updated;           // 数据更新标志
} SBusReceiver;

// 初始化SBUS接收器
void sbus_init(SBusReceiver *receiver);

// 处理接收到的字节
void sbus_process_byte(SBusReceiver *receiver, uint8_t byte);

// 检查是否有新数据更新
bool sbus_data_available(SBusReceiver *receiver);

// 获取指定通道的值 (0-15)
uint16_t sbus_get_channel(SBusReceiver *receiver, uint8_t channel);

#endif // SBUS_RECEIVER_H
