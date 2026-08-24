/* tb6612.h */
#ifndef __TB6612_H
#define __TB6612_H

#include "board.h"
#include <stdint.h>

#define ABS(a)      ( (a) > 0 ? (a) : -(a) )

#define AIN1_OUT(X)  ( (X) ? DL_GPIO_setPins(TB6612_AIN1_PORT,TB6612_AIN1_PIN) \
                          : DL_GPIO_clearPins(TB6612_AIN1_PORT,TB6612_AIN1_PIN) )
#define AIN2_OUT(X)  ( (X) ? DL_GPIO_setPins(TB6612_AIN2_PORT,TB6612_AIN2_PIN) \
                          : DL_GPIO_clearPins(TB6612_AIN2_PORT,TB6612_AIN2_PIN) )
#define BIN1_OUT(X)  ( (X) ? DL_GPIO_setPins(TB6612_BIN1_PORT,TB6612_BIN1_PIN) \
                          : DL_GPIO_clearPins(TB6612_BIN1_PORT,TB6612_BIN1_PIN) )
#define BIN2_OUT(X)  ( (X) ? DL_GPIO_setPins(TB6612_BIN2_PORT,TB6612_BIN2_PIN) \
                          : DL_GPIO_clearPins(TB6612_BIN2_PORT,TB6612_BIN2_PIN) )
#define CIN1_OUT(X)  ( (X) ? DL_GPIO_setPins(TB6612_CIN1_PORT,TB6612_CIN1_PIN) \
                          : DL_GPIO_clearPins(TB6612_CIN1_PORT,TB6612_CIN1_PIN) )
#define CIN2_OUT(X)  ( (X) ? DL_GPIO_setPins(TB6612_CIN2_PORT,TB6612_CIN2_PIN) \
                          : DL_GPIO_clearPins(TB6612_CIN2_PORT,TB6612_CIN2_PIN) )
#define DIN1_OUT(X)  ( (X) ? DL_GPIO_setPins(TB6612_DIN1_PORT,TB6612_DIN1_PIN) \
                          : DL_GPIO_clearPins(TB6612_DIN1_PORT,TB6612_DIN1_PIN) )
#define DIN2_OUT(X)  ( (X) ? DL_GPIO_setPins(TB6612_DIN2_PORT,TB6612_DIN2_PIN) \
                          : DL_GPIO_clearPins(TB6612_DIN2_PORT,TB6612_DIN2_PIN) )

typedef struct{
    int Should_Get_Encoder_Count;   // 将要获得的编码器计数
    int Obtained_Get_Encoder_Count; // 得到的编码器的计数
}Encoder;

void Motor_Init(void);
void Motor_Set_PWM(int pwma,int pwmb,int pwmc,int pwmd);
int Motor_Get_Encoder(int dir);
void get_speet(int pwma1,int pwmb1,int pwmc1,int pwmd1);
void get_pid(float kp);

#endif /* __TB6612_H */
