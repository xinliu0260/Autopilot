/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#include "dev_rc_sbus.h"


// SBUS协议相关定义
#define SBUS_PACKET_SIZE 25      // SBUS数据包大小
#define SBUS_CHANNELS 16         // 通道数量
#define SBUS_START_BYTE 0x0F     // 数据包起始字节
#define SBUS_END_BYTE 0x00       // 数据包结束字节


// 初始化SBUS接收器
void sbus_init(SBusReceiver *receiver) {
    receiver->state = SBUS_STATE_WAIT_START;
    receiver->buffer_idx = 0;
    receiver->data_updated = false;
    
    // 初始化通道数据
    for (int i = 0; i < SBUS_CHANNELS; i++) {
        receiver->channels[i] = 0;
    }
}

// 处理接收到的字节
void sbus_process_byte(SBusReceiver *receiver, uint8_t byte) {
    switch (receiver->state) {
        case SBUS_STATE_WAIT_START:
            // 等待起始字节
            if (byte == SBUS_START_BYTE) {
                receiver->state = SBUS_STATE_RECEIVE_DATA;
                receiver->buffer[receiver->buffer_idx++] = byte;
            }
            break;
            
        case SBUS_STATE_RECEIVE_DATA:
            // 接收数据字节
            receiver->buffer[receiver->buffer_idx++] = byte;
            
            // 检查是否已接收完所有数据字节
            if (receiver->buffer_idx == SBUS_PACKET_SIZE - 1) {
                receiver->state = SBUS_STATE_RECEIVE_END;
            }
            break;
            
        case SBUS_STATE_RECEIVE_END:
            // 检查结束字节
            if (byte == SBUS_END_BYTE) {
                receiver->buffer[receiver->buffer_idx++] = byte;
                
                // 解析数据包
                if (receiver->buffer_idx == SBUS_PACKET_SIZE) {
                    // 解析16个通道数据
                    receiver->channels[0]  = ((uint16_t)receiver->buffer[1]       | ((uint16_t)receiver->buffer[2] << 8)) & 0x07FF;
                    receiver->channels[1]  = (((uint16_t)receiver->buffer[2] >> 3) | ((uint16_t)receiver->buffer[3] << 5)) & 0x07FF;
                    receiver->channels[2]  = (((uint16_t)receiver->buffer[3] >> 6) | ((uint16_t)receiver->buffer[4] << 2) | ((uint16_t)receiver->buffer[5] << 10)) & 0x07FF;
                    receiver->channels[3]  = (((uint16_t)receiver->buffer[5] >> 1) | ((uint16_t)receiver->buffer[6] << 7)) & 0x07FF;
                    receiver->channels[4]  = (((uint16_t)receiver->buffer[6] >> 4) | ((uint16_t)receiver->buffer[7] << 4)) & 0x07FF;
                    receiver->channels[5]  = (((uint16_t)receiver->buffer[7] >> 7) | ((uint16_t)receiver->buffer[8] << 1) | ((uint16_t)receiver->buffer[9] << 9)) & 0x07FF;
                    receiver->channels[6]  = (((uint16_t)receiver->buffer[9] >> 2) | ((uint16_t)receiver->buffer[10] << 6)) & 0x07FF;
                    receiver->channels[7]  = (((uint16_t)receiver->buffer[10] >> 5) | ((uint16_t)receiver->buffer[11] << 3)) & 0x07FF;
                    receiver->channels[8]  = ((uint16_t)receiver->buffer[12]      | ((uint16_t)receiver->buffer[13] << 8)) & 0x07FF;
                    receiver->channels[9]  = (((uint16_t)receiver->buffer[13] >> 3) | ((uint16_t)receiver->buffer[14] << 5)) & 0x07FF;
                    receiver->channels[10] = (((uint16_t)receiver->buffer[14] >> 6) | ((uint16_t)receiver->buffer[15] << 2) | ((uint16_t)receiver->buffer[16] << 10)) & 0x07FF;
                    receiver->channels[11] = (((uint16_t)receiver->buffer[16] >> 1) | ((uint16_t)receiver->buffer[17] << 7)) & 0x07FF;
                    receiver->channels[12] = (((uint16_t)receiver->buffer[17] >> 4) | ((uint16_t)receiver->buffer[18] << 4)) & 0x07FF;
                    receiver->channels[13] = (((uint16_t)receiver->buffer[18] >> 7) | ((uint16_t)receiver->buffer[19] << 1) | ((uint16_t)receiver->buffer[20] << 9)) & 0x07FF;
                    receiver->channels[14] = (((uint16_t)receiver->buffer[20] >> 2) | ((uint16_t)receiver->buffer[21] << 6)) & 0x07FF;
                    receiver->channels[15] = (((uint16_t)receiver->buffer[21] >> 5) | ((uint16_t)receiver->buffer[22] << 3)) & 0x07FF;

                    receiver->data_updated = true;
                }
            }
            
            // 重置状态，准备接收下一个数据包
            receiver->state = SBUS_STATE_WAIT_START;
            receiver->buffer_idx = 0;
            break;
    }
}

// 检查是否有新数据更新
bool sbus_data_available(SBusReceiver *receiver) {
    return receiver->data_updated;
}

// 获取指定通道的值 (0-15)
uint16_t sbus_get_channel(SBusReceiver *receiver, uint8_t channel) {
    if (channel >= SBUS_CHANNELS) {
        return 0; // 无效通道
    }
    
    receiver->data_updated = false; // 清除更新标志
    return receiver->channels[channel];
}


