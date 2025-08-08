#ifndef AUTOPILOT_DEF_H__
#define AUTOPILOT_DEF_H__

/* Thread Prority */
#define VEHICLE_THREAD_PRIORITY    3
#define FMTIO_THREAD_PRIORITY      7
#define DRONECAN_THREAD_PRIORITY   4
#define MAVLINK_RX_THREAD_PRIORITY 10
#define MAVOBC_THREAD_PRIORITY     11
#define MAVGCS_THREAD_PRIORITY     12
#define LOGGER_THREAD_PRIORITY     13
#define STATUS_THREAD_PRIORITY     14


#include <stdint.h>

#ifndef true
    #define true (1U)
#endif

#ifndef false
    #define false (0U)
#endif

#ifndef PI
    #define PI 3.14159265358979f
#endif

#define RT_TRY(__exp)                                                                                \
    do {                                                                                             \
        rt_err_t err = (__exp);                                                                      \
        if (err != RT_EOK) {                                                                         \
            LOG_E("RT_TRY failed at function:%s, line:%u, err:%d\n", __FUNCTION__, __LINE__, err); \
            return err;                                                                              \
        }                                                                                            \
    } while (false)

#define RT_CHECK(func)  RT_ASSERT(func == RT_EOK)

typedef struct {
    uint32_t period;
    char* info;
} model_info_t;


uint64_t autopilot_system_time_us(void);
uint32_t autopilot_system_time_ms(void);


#define PERIOD_EXECUTE(_name,_period,_operation)                    \
    static uint32_t last_time_##_name = 0;                          \
    do{                                                             \
        if(autopilot_system_time_ms()-last_time_##_name>=_period)  \
            _operation                                              \
    }while(0)                                                       \


#endif