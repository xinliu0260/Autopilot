/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */


#ifndef BUTTER_H__
#define BUTTER_H__

#include "autopilot.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    float A[4];
    float B[4];
    float X[4];
    float Y[4];
} Butter3;

/* butter lpf filter */
Butter3* butter3_filter_create(float b[4], float a[4]);
float butter3_filter_process(float in, Butter3* butter);

#ifdef __cplusplus
}
#endif

#endif
