#ifndef __KEY_H
#define __KEY_H

#include <stdint.h>
#include <stdbool.h>
#include "ti_msp_dl_config.h"
#include "board.h"
#include "led.h"

#define MENU_MIN        1  /* 菜单最小值 */
#define MENU_MAX        7  /* 菜单最大值 */

/* 初始化按键菜单，配置 IOMUX/GPIO 并显示初始选项 */
void KeyMenu_Init(void);

/* 在主循环中周期调用，处理按键并执行对应程序 */
void KeyMenu_Update(void);

/* 获取当前选择值 sel */
uint8_t KeyMenu_GetSelection(void);

/* 显示当前选择，由用户实现（数码管/OLED/串口等） */
void KeyMenu_ShowSelection(uint8_t sel);

/* 程序执行函数，由用户在其他模块中实现或在此处修改 */
void run_prog1(void);
void run_prog2(void);
void run_prog3(void);
void run_prog4(void);
void run_prog5(void);
void run_prog6(void);
void run_prog7(void);

bool key_pressed4(void);

#endif 