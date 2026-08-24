#ifndef __LED_H
#define __LED_H 

#include "board.h"
#include "stdlib.h"	

#define LED_OFF DL_GPIO_clearPins(LED_PORT,LED_PIN_PIN);//输出低电平
#define LED_ON DL_GPIO_setPins(LED_PORT,LED_PIN_PIN);  //输出高电平

#define BEEP_ON DL_GPIO_clearPins(BEEP_PORT,BEEP_beep_PIN);//输出低电平
#define BEEP_OFF DL_GPIO_setPins(BEEP_PORT,BEEP_beep_PIN);  //输出高电平

#define TB6612_OFF DL_GPIO_clearPins(TB6612_STBY_PORT,TB6612_STBY_PIN);//输出低电平
#define TB6612_ON DL_GPIO_setPins(TB6612_STBY_PORT,TB6612_STBY_PIN);  //输出高电平

#endif