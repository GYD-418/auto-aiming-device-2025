
#include "key.h"
#include "oled.h"
#include <stdio.h>


/* 当前菜单选择 */
static uint8_t sel = MENU_MIN;

/* 按键去抖 & 检测函数 */
/* “＋”键 */
static bool key_pressed1(void)
{
    if (DL_GPIO_readPins(KEY_KEY1_PORT, KEY_KEY1_PIN) == 0) {
        delay_ms(20);
        if (DL_GPIO_readPins(KEY_KEY1_PORT, KEY_KEY1_PIN) == 0) {
            while (DL_GPIO_readPins(KEY_KEY1_PORT, KEY_KEY1_PIN) == 0) { }
            return true;
        }
    }
    return false;
}

/* “－”键 */
static bool key_pressed2(void)
{
    if (DL_GPIO_readPins(KEY_KEY2_PORT, KEY_KEY2_PIN) == 0) {
        delay_ms(20);
        if (DL_GPIO_readPins(KEY_KEY2_PORT, KEY_KEY2_PIN) == 0) {
            while (DL_GPIO_readPins(KEY_KEY2_PORT, KEY_KEY2_PIN) == 0) { }
            return true;
        }
    }
    return false;
}

/* “确认”键 */
static bool key_pressed3(void)
{
    if (DL_GPIO_readPins(KEY_KEY3_PORT, KEY_KEY3_PIN) == 0) {
        delay_ms(20);
        if (DL_GPIO_readPins(KEY_KEY3_PORT, KEY_KEY3_PIN) == 0) {
            while (DL_GPIO_readPins(KEY_KEY3_PORT, KEY_KEY3_PIN) == 0) { }
            return true;
        }
    }
    return false;
}

/* （可选）第四键，如果有的话 */
bool key_pressed4(void)
{
    if (DL_GPIO_readPins(KEY_KEY4_PORT, KEY_KEY4_PIN) == 0) {
        delay_ms(20);
        if (DL_GPIO_readPins(KEY_KEY4_PORT, KEY_KEY4_PIN) == 0) {
            while (DL_GPIO_readPins(KEY_KEY4_PORT, KEY_KEY4_PIN) == 0) { }
            return true;
        }
    }
    return false;
}

/**
 * @brief  显示当前 sel 值到 OLED
 * @param  sel: 1 ~ MENU_MAX
 */
void KeyMenu_ShowSelection(uint8_t sel)
{
    char buf[8];

    OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t *)"Selected:", 16, 1);
    snprintf(buf, sizeof(buf), "%d", sel);
    OLED_ShowString(95, 0, (uint8_t *)buf, 16, 1);//70~120
    OLED_Refresh();
}

/**
 * @brief  初始化菜单：sel 置为最小值并显示
 */
void KeyMenu_Init(void)
{
    sel = MENU_MIN;
    KeyMenu_ShowSelection(sel);
}

/**
 * @brief  主循环中周期调用：检测按键、更新 sel、刷新 OLED，确认键时执行 run_progX()
 */
void KeyMenu_Update(void)
{
    /* “＋” */
    if (key_pressed1()) {
        sel = (sel < MENU_MAX) ? (sel + 1) : MENU_MIN;
        KeyMenu_ShowSelection(sel);
    }
    /* “－” */
    if (key_pressed2()) {
        sel = (sel > MENU_MIN) ? (sel - 1) : MENU_MAX;
        KeyMenu_ShowSelection(sel);
    }
    /* “确认” */
    if (key_pressed3()) {
        switch (sel) {
            case 1: run_prog1(); break;
            case 2: run_prog2(); break;
            case 3: run_prog3(); break;
            case 4: run_prog4(); break;
            case 5: run_prog5(); break;
            case 6: run_prog6(); break;
            case 7: run_prog7(); break;
            default: break;
        }
        /* 保持当前 sel，再次刷新显示 */
        KeyMenu_ShowSelection(sel);
    }
}

/**
 * @brief  获取当前 sel 值
 * @return 1 ~ MENU_MAX
 */
uint8_t KeyMenu_GetSelection(void)
{
    return sel;
}

/* 以下为占位的 run_progX 实现，可在其他模块重写或在此处填充具体功能 */
void run_prog1(void) { 
    TB6612_ON;
    NVIC_EnableIRQ(TIMER_1_INST_INT_IRQN);
 }
void run_prog2(void) { 
    TB6612_OFF;
    NVIC_DisableIRQ(TIMER_1_INST_INT_IRQN);
}
void run_prog3(void) { /* TODO: 功能3 */ }
void run_prog4(void) { /* TODO: 功能4 */ }
void run_prog5(void) { /* TODO: 功能5 */ }
void run_prog6(void) { /* TODO: 功能6 */ }
void run_prog7(void) { /* TODO: 功能7 */ }
