/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#ifndef CONVERSION_H__
#define CONVERSION_H__

#include "autopilot.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RAD2DEG(u) ((u)*180.0f / PI)
#define DEG2RAD(u) ((u)*PI / 180.0f)

void Msb2Lsb(uint8_t* data, uint8_t bytes);
int16_t int16_t_from_bytes(uint8_t bytes[]);

#ifdef __cplusplus
}
#endif

#endif
