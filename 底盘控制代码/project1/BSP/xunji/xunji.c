#include "xunji.h"
#include "ti_msp_dl_config.h"  // 包含 DL_GPIO_* 系列函数
#include "jy61p.h"

// 读取 5 路 GPIO，拼成一个字节返回
uint8_t Sensor_GetState(void)
{
    uint8_t state = 0;
    state |= (DL_GPIO_readPins(xunji_PORT, xunji_x1_PIN) > 0 ? 1 : 0) << 0;
    state |= (DL_GPIO_readPins(xunji_PORT, xunji_x2_PIN) > 0 ? 1 : 0) << 1;
    state |= (DL_GPIO_readPins(xunji_PORT, xunji_x3_PIN) > 0 ? 1 : 0) << 2;
    state |= (DL_GPIO_readPins(xunji_PORT, xunji_x4_PIN) > 0 ? 1 : 0) << 3;
    state |= (DL_GPIO_readPins(xunji_PORT, xunji_x5_PIN) > 0 ? 1 : 0) << 4;
    // state |= (DL_GPIO_readPins(xunji_PORT, xunji_x6_PIN) > 0 ? 1 : 0) << 5;
    // state |= (DL_GPIO_readPins(xunji_PORT, xunji_x7_PIN) > 0 ? 1 : 0) << 6;
    // state |= (DL_GPIO_readPins(xunji_PORT, xunji_x8_PIN) > 0 ? 1 : 0) << 7;
    return state;
}

int jy61p(float mz)
{
    Gyro_Struct *bng = get_angle();
    return (int)((mz - bng->z)/10);
}
// 根据每路检测到的黑/白，计算“偏差量”
// 先定义一个权重数组：越左边用负值，越右边用正值
// weights[0] 对应 x1，weights[4] 对应 x5
static const int8_t weights[5] = {-13, -5, 0, +5, +8}; 

int16_t Sensor_GetPosition(void)
{
    uint8_t s = Sensor_GetState();
    int32_t sumW = 0, count = 0;

    for (uint8_t i = 0; i < 5; i++) {
        if (s & (1u << i)) {
            sumW += weights[i];
            count++;
        }
    }

    if (count == 0) {
        // 没有检测到任何黑线，可以返回一个特殊值
        return -7;
    }

    // 返回加权平均
    return (int16_t)( sumW / count );
}
