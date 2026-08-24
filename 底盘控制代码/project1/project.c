// main.c

#include "ti_msp_dl_config.h"
#include "board.h"
#include "oled.h"
#include "key.h"
#include "jy61p.h"
#include "tb6612.h"
#include <stdio.h>
#include "xunji.h"

int t=0;
int t1=0;
int encoder_count1,encoder_count2,encoder_count3,encoder_count4;
int main(void)
{
    SYSCFG_DL_init();
    OLED_Init(); OLED_Clear(); OLED_Refresh();
    jy61pInit();
    KeyMenu_Init();
    Motor_Init();
    NVIC_ClearPendingIRQ(TIMER_1_INST_INT_IRQN);

    char buf[16];
    char buf1[16];
    char buf2[16];
    int i1 = 0;
    float i2 = 0;
    int i3 = 0;
    int time=0;
   
    while (1) {
        // get_speet(200,200,200,200);
        // Motor_Set_PWM(1000,1000,1000,1000);
        encoder_count1 = Motor_Get_Encoder(0);
        encoder_count2 = Motor_Get_Encoder(1);
        encoder_count3 = Motor_Get_Encoder(2);
        encoder_count4 = Motor_Get_Encoder(3);
        OLED_ShowNum(28,32,encoder_count1,4,8,1);
        OLED_ShowNum(28,95,encoder_count2,4,8,1);
        OLED_ShowNum(80,32,encoder_count3,4,8,1);
        OLED_ShowNum(80,95,encoder_count4,4,8,1);
        OLED_ShowNum(28,40,i3,2,16,1);
        KeyMenu_Update();  
        if(key_pressed4())
        {
            i1+=1;
            t=181*i1;
        }
        i2=t1/10;
        i3=Sensor_GetPosition();
        get_speet(4.85+i3*1.4,4.85-i3*1.4,4.85+i3*1.4,4.85-i3*1.4);
        snprintf(buf, sizeof(buf), "%d", i1);
        OLED_ShowString(0, 16, (uint8_t*)buf,16,1);
        snprintf(buf1, sizeof(buf1), "%0.1f", i2);
        OLED_ShowString(0, 32, (uint8_t*)buf1,16,1);
        snprintf(buf2, sizeof(buf2), "%d", i3);
        OLED_ShowString(0, 48, (uint8_t*)buf2,16,1);
        
        OLED_Refresh();
    }
}

void TIMER_1_INST_IRQHandler(void)
{
    // 读取当前的 pending 中断源
    uint32_t pending = DL_TimerG_getPendingInterrupt(TIMER_1_INST);

    // 如果是 0 溢出中断
    if (pending == DL_TIMER_IIDX_ZERO) {
        if(t)
        {
            t--;
            t1++;
        }
        else {
            TB6612_OFF;
            NVIC_DisableIRQ(TIMER_1_INST_INT_IRQN);
        }
    }
    
}
