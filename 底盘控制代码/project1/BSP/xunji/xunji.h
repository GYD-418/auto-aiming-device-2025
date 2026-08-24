#ifndef XUNJI_H
#define XUNJI_H

#include <stdint.h>

// 返回 8 路传感器的原始状态，每一位对应一路
// bit7 … bit0 = x8 x7 x6 x5 x4 x3 x2 x1
uint8_t Sensor_GetState(void);

// 返回“偏差量”，范围大约是 –7 … +7（*100 缩放后 –700…+700）
// 正值表示偏向右侧，负值表示偏向左侧
int16_t Sensor_GetPosition(void);
int jy61p(float mz);

#endif // XUNJI_H
