/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

#include "autopilot.h"
#include <math.h>
#include "autopilot_def.h"
#include "extdevice.h"
#include "rtdevice.h"
#include "module/math/conversion.h"
#include "rtdef.h"
#include "rttypes.h"

#define LOG_TAG "drv.ms5611"
#define LOG_LVL DBG_INFO
#include "ulog.h"


#ifdef BIT
    #undef BIT
#endif
#define BIT(_idx) (1 << _idx)

#define SPL06_PRS_B2   0x00
#define SPL06_PRS_B1   0x01
#define SPL06_PRS_B0   0x02
#define SPL06_TMP_B2   0x03
#define SPL06_TMP_B1   0x04
#define SPL06_TMP_B0   0x05
#define SPL06_PRS_CFG  0x06
#define SPL06_TMP_CFG  0x07
#define SPL06_MEAS_CFG 0x08
#define SPL06_CFG_REG  0x09
#define SPL06_INT_STS  0x0A
#define SPL06_FIFO_STS 0x0B
#define SPL06_RESET    0x0C
#define SPL06_ID       0x0D
#define SPL06_COEF     0x10

#define PM_RATE_1   (0)
#define PM_RATE_2   (BIT(4))
#define PM_RATE_4   (BIT(5))
#define PM_RATE_8   (BIT(4) | BIT(5))
#define PM_RATE_16  (BIT(6))
#define PM_RATE_32  (BIT(4) | BIT(6))
#define PM_RATE_64  (BIT(5) | BIT(6))
#define PM_RATE_128 (BIT(4) | BIT(5) | BIT(6))

#define PM_PRC_1   (0)
#define PM_PRC_2   (BIT(0))
#define PM_PRC_4   (BIT(1))
#define PM_PRC_8   (BIT(0) | BIT(1))
#define PM_PRC_16  (BIT(2))
#define PM_PRC_32  (BIT(0) | BIT(2))
#define PM_PRC_64  (BIT(1) | BIT(2))
#define PM_PRC_128 (BIT(0) | BIT(1) | BIT(2))

#define TMP_RATE_1   (0)
#define TMP_RATE_2   (BIT(4))
#define TMP_RATE_4   (BIT(5))
#define TMP_RATE_8   (BIT(4) | BIT(5))
#define TMP_RATE_16  (BIT(6))
#define TMP_RATE_32  (BIT(4) | BIT(6))
#define TMP_RATE_64  (BIT(5) | BIT(6))
#define TMP_RATE_128 (BIT(4) | BIT(5) | BIT(6))

#define TMP_PRC_1   (0)
#define TMP_PRC_2   (BIT(0))
#define TMP_PRC_4   (BIT(1))
#define TMP_PRC_8   (BIT(0) | BIT(1))
#define TMP_PRC_16  (BIT(2))
#define TMP_PRC_32  (BIT(0) | BIT(2))
#define TMP_PRC_64  (BIT(1) | BIT(2))
#define TMP_PRC_128 (BIT(0) | BIT(1) | BIT(2))

typedef struct {
    uint8_t setbits;
    uint8_t clearbits;
} reg_val_t;

static rt_device_t baro_spi_dev;

static int16_t c0, c1, c01, c11, c20, c21, c30;
static int32_t c00, c10;
static uint32_t _kP, _kT;

/**
 * @brief 用RT-Thread SPI接口实现寄存器读取（单字节）
 * @param spi_dev SPI设备句柄
 * @param reg 寄存器地址
 * @param value 读取到的值
 * @return 操作结果（RT_EOK表示成功）
 */
static rt_err_t spi_read_reg8(rt_device_t spi_dev, rt_uint8_t reg, rt_uint8_t* value)
{
    struct rt_spi_device* spi = (struct rt_spi_device*)spi_dev;
    rt_uint8_t send_buf[1] = {reg | 0x80};  // 最高位置1表示读操作
    rt_uint8_t recv_buf[1] = {0};

    // 先发送寄存器地址，再接收1字节数据
    rt_err_t ret = rt_spi_send_then_recv(spi, send_buf, 1, recv_buf, 1);
    if (ret == RT_EOK) {
        *value = recv_buf[0];
    }
    return ret;
}

/**
 * @brief 用RT-Thread SPI接口实现寄存器写入（单字节）
 * @param spi_dev SPI设备句柄
 * @param reg 寄存器地址
 * @param value 要写入的值
 * @return 操作结果（RT_EOK表示成功）
 */
static rt_err_t spi_write_reg8(rt_device_t spi_dev, rt_uint8_t reg, rt_uint8_t value)
{
    struct rt_spi_device* spi = (struct rt_spi_device*)spi_dev;
    rt_uint8_t send_buf[2] = {reg & 0x7F, value};  // 最高位清0表示写操作

    // 发送寄存器地址和数据（共2字节）
    return rt_spi_send(spi, send_buf, 2);
}

/**
 * @brief 用RT-Thread SPI接口实现多寄存器连续读取
 * @param spi_dev SPI设备句柄
 * @param reg 起始寄存器地址
 * @param buf 接收缓冲区
 * @param len 要读取的字节数
 * @return 操作结果（RT_EOK表示成功）
 */
static rt_err_t spi_read_multi_reg8(rt_device_t spi_dev, rt_uint8_t reg, rt_uint8_t* buf, rt_size_t len)
{
    struct rt_spi_device* spi = (struct rt_spi_device*)spi_dev;
    rt_uint8_t send_buf[1] = {reg | 0x80};  // 最高位置1表示读操作

    // 先发送起始地址，再接收len字节数据
    return rt_spi_send_then_recv(spi, send_buf, 1, buf, len);
}


static rt_err_t modify_reg(rt_device_t spi_device, rt_uint8_t reg, reg_val_t reg_val)
{
    uint8_t value;

    RT_TRY(spi_read_reg8(spi_device, reg, &value));

    value &= ~reg_val.clearbits;
    value |= reg_val.setbits;

    RT_TRY(spi_write_reg8(spi_device, reg, value));

    return RT_EOK;
}

static rt_err_t read_coef(void)
{
    uint8_t coef_data[18];

    RT_TRY(spi_read_multi_reg8(baro_spi_dev, SPL06_COEF, coef_data, sizeof(coef_data)));

    c0 = ((int16_t)coef_data[0] << 4) | ((coef_data[1] >> 4) & 0x0F);
    c0 = (c0 & 0x0800) ? (0xF000 | c0) : c0;
    c1 = (((int16_t)coef_data[1] & 0x0F) << 8) | coef_data[2];
    c1 = (c1 & 0x0800) ? (0xF000 | c1) : c1;
    c00 = ((int32_t)coef_data[3] << 12) | ((int32_t)coef_data[4] << 4) | ((coef_data[5] >> 4) & 0x0F);
    c00 = (c00 & 0x080000) ? (0xFFF00000 | c00) : c00;
    c10 = (((int32_t)coef_data[5] & 0x0f) << 16) | ((int32_t)coef_data[6] << 8) | coef_data[7];
    c10 = (c10 & 0x080000) ? (0xFFF00000 | c10) : c10;
    c01 = ((int16_t)coef_data[8] << 8) | coef_data[9];
    c11 = ((int16_t)coef_data[10] << 8) | coef_data[11];
    c20 = ((int16_t)coef_data[12] << 8) | coef_data[13];
    c21 = ((int16_t)coef_data[14] << 8) | coef_data[15];
    c30 = ((int16_t)coef_data[16] << 8) | coef_data[17];

    return RT_EOK;
}

static rt_err_t config_pressure(uint8_t rate, uint8_t prc)
{
    switch (prc) {
    case PM_PRC_1:
        _kP = 524288;
        break;
    case PM_PRC_2:
        _kP = 1572864;
        break;
    case PM_PRC_4:
        _kP = 3670016;
        break;
    case PM_PRC_8:
        _kP = 7864320;
        break;
    case PM_PRC_16:
        _kP = 253952;
        break;
    case PM_PRC_32:
        _kP = 516096;
        break;
    case PM_PRC_64:
        _kP = 1040384;
        break;
    case PM_PRC_128:
        _kP = 2088960;
        break;
    default:
        return RT_EINVAL;
    }

    RT_TRY(spi_write_reg8(baro_spi_dev, SPL06_PRS_CFG, rate | prc));

    if (prc > PM_PRC_8) {
        /* must set P_SHIFT when oversampling rate is > 8 */
        modify_reg(baro_spi_dev, SPL06_CFG_REG, (reg_val_t) { .setbits = BIT(2), .clearbits = 0 });
    }

    return RT_EOK;
}

static rt_err_t config_temprature(uint8_t rate, uint8_t prc)
{
    switch (prc) {
    case TMP_PRC_1:
        _kT = 524288;
        break;
    case TMP_PRC_2:
        _kT = 1572864;
        break;
    case TMP_PRC_4:
        _kT = 3670016;
        break;
    case TMP_PRC_8:
        _kT = 7864320;
        break;
    case TMP_PRC_16:
        _kT = 253952;
        break;
    case TMP_PRC_32:
        _kT = 516096;
        break;
    case TMP_PRC_64:
        _kT = 1040384;
        break;
    case TMP_PRC_128:
        _kT = 2088960;
        break;
    default:
        return RT_EINVAL;
    }

    /* we need set TMP_EXT to use external sensor */
    RT_TRY(spi_write_reg8(baro_spi_dev, SPL06_TMP_CFG, BIT(7) | rate | prc));

    if (prc > TMP_PRC_8) {
        /* must set P_SHIFT when oversampling rate is > 8 */
        modify_reg(baro_spi_dev, SPL06_CFG_REG, (reg_val_t) { .setbits = BIT(3), .clearbits = 0 });
    }

    return RT_EOK;
}

static rt_err_t measure(baro_report_t* report)
{
    uint8_t data[3];
    int32_t Praw, TRaw;
    float Praw_sc, Traw_sc;

    RT_TRY(spi_read_multi_reg8(baro_spi_dev, SPL06_PRS_B2, data, sizeof(data)));

    Praw = (((int32_t)data[0] << 24) | ((int32_t)data[1] << 16) | ((int32_t)data[2] << 8));
    Praw = Praw >> 8;
    Praw_sc = (float)Praw / _kP;

    RT_TRY(spi_read_multi_reg8(baro_spi_dev, SPL06_TMP_B2, data, sizeof(data)));

    TRaw = (((int32_t)data[0] << 24) | ((int32_t)data[1] << 16) | ((int32_t)data[2] << 8));
    TRaw = TRaw >> 8;
    Traw_sc = (float)TRaw / _kT;

    report->pressure_Pa = c00 + Praw_sc * (c10 + Praw_sc * (c20 + Praw_sc * c30)) + Traw_sc * c01 + Traw_sc * Praw_sc * (c11 + Praw_sc * c21);
    report->temperature_deg = 0.5f * c0 + Traw_sc * c1;
    report->altitude_m = 44330.0f * (1.0f - powf(report->pressure_Pa / 101325.0f, 1.0f / 5.255f));
    report->timestamp_ms = rt_tick_get_millisecond();

    return RT_EOK;
}

static rt_err_t lowlevel_init(void)
{
    uint8_t chip_id;
    uint8_t reg_val;

    /* check chip id */
    RT_TRY(spi_read_reg8(baro_spi_dev, SPL06_ID, &chip_id));
    if (chip_id != 0x10) {
        LOG_W("spl06 wrong chip id:0x%x\n", chip_id);
        return RT_ERROR;
    }

    /* wait untill COEF_RDY is set */
    do {
        RT_TRY(spi_read_reg8(baro_spi_dev, SPL06_MEAS_CFG, &reg_val));
    } while ((reg_val & BIT(7)) == 0);

    RT_TRY(read_coef());

    RT_TRY(config_temprature(TMP_RATE_128, TMP_PRC_2));
    RT_TRY(config_pressure(PM_RATE_128, PM_PRC_4));

    /* wait untill SENSOR_RDY is set */
    do {
        RT_TRY(spi_read_reg8(baro_spi_dev, SPL06_MEAS_CFG, &reg_val));
    } while ((reg_val & BIT(6)) == 0);

    /* start continuous pressume and temperature measurement */
    RT_TRY(modify_reg(baro_spi_dev, SPL06_MEAS_CFG, (reg_val_t) { .setbits = BIT(0) | BIT(1) | BIT(2), .clearbits = 0 }));

    return RT_EOK;
}

static rt_err_t baro_control(baro_dev_t baro, int cmd, void* arg)
{
    switch (cmd) {
    case BARO_CMD_CHECK_READY: {
        *(uint8_t*)arg = RT_FALSE;
        PERIOD_EXECUTE(baro_interval, 10, 
            {
                *(uint8_t*)arg = RT_TRUE;
            }
        );
    } break;
    default:
        break;
    }

    return RT_EOK;
}

static rt_size_t baro_read(baro_dev_t baro, baro_report_t* report)
{
    rt_size_t size = 0;

    if (measure(report) == RT_EOK) {
        size = sizeof(baro_report_t);
    }

    return size;
}

static struct baro_ops _baro_ops = {
    .baro_control = baro_control,
    .baro_read = baro_read
};

rt_err_t drv_spl06_init(const char* spi_device_name, const char* baro_device_name)
{
    static struct baro_device baro_dev = {
        .ops = &_baro_ops
    };

    baro_spi_dev = rt_device_find(spi_device_name);
    RT_ASSERT(baro_spi_dev != NULL);

    /* config spi */
    {
        struct rt_spi_configuration cfg;
        cfg.data_width = 8;
        cfg.mode = RT_SPI_MODE_3 | RT_SPI_MSB; /* SPI Compatible Modes 3 */
        cfg.max_hz = 7000000;

        struct rt_spi_device* spi_device_t = (struct rt_spi_device*)baro_spi_dev;
        spi_device_t->config.data_width = cfg.data_width;
        spi_device_t->config.mode = cfg.mode & RT_SPI_MODE_MASK;
        spi_device_t->config.max_hz = cfg.max_hz;

        RT_TRY(rt_spi_configure(spi_device_t, &cfg));
    }

    /* device low-level initialization */
    RT_TRY(lowlevel_init());
    /* register barometer device */
    RT_TRY(dev_baro_register(&baro_dev, baro_device_name, RT_DEVICE_FLAG_RDWR, RT_NULL));

    return RT_EOK;
}