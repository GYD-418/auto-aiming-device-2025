#include "tb6612.h"
#include "board.h"
#include <stdint.h>

static volatile Encoder Encoder_A;
static volatile Encoder Encoder_B;
static volatile Encoder Encoder_C;
static volatile Encoder Encoder_D;

static float Velocity_Kp=100,  Velocity_Ki=5,  Velocity_Kd=10; //相关速度PID参数
static int pwma=0,pwmb=0,pwmc=0,pwmd=0;


void get_speet(int pwma1,int pwmb1,int pwmc1,int pwmd1)
{
	pwma=pwma1;
	pwmb=pwmb1;
	pwmc=pwmc1;
	pwmd=pwmd1;
}
void get_pid(float kp)
{
	Velocity_Kd=kp;
}

/******************************************************************
 * 函 数 名 称：Motor_Init
 * 函 数 说 明：电机与编码器初始化
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：无
******************************************************************/
void Motor_Init(void)
{
	//编码器引脚外部中断
	NVIC_ClearPendingIRQ(ENCODER_GPIOB_INT_IRQN);
    NVIC_EnableIRQ   (ENCODER_GPIOB_INT_IRQN);
    NVIC_ClearPendingIRQ(ENCODER_GPIOA_INT_IRQN);
    NVIC_EnableIRQ   (ENCODER_GPIOA_INT_IRQN);

    //定时器中断
	NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);
    NVIC_EnableIRQ   (TIMER_0_INST_INT_IRQN);

}
//0
void Motor_Set_PWM(int pwma,int pwmb,int pwmc,int pwmd)
{
    if (pwma <= 0) { AIN1_OUT(0); AIN2_OUT(1); }
    else             { AIN1_OUT(1); AIN2_OUT(0); }
    DL_TimerG_setCaptureCompareValue(PWM_1_INST, ABS(pwma), GPIO_PWM_1_C1_IDX);

    if (pwmb <= 0) { BIN1_OUT(0); BIN2_OUT(1); }
    else             { BIN1_OUT(1); BIN2_OUT(0); }
    DL_TimerG_setCaptureCompareValue(PWM_1_INST, ABS(pwmb), GPIO_PWM_1_C0_IDX);

    if (pwmc <= 0) { CIN1_OUT(0); CIN2_OUT(1); }
    else             { CIN1_OUT(1); CIN2_OUT(0); }
    DL_TimerG_setCaptureCompareValue(PWM_2_INST, ABS(pwmc), GPIO_PWM_2_C0_IDX);

    if (pwmd <= 0) { DIN1_OUT(0); DIN2_OUT(1); }
    else             { DIN1_OUT(1); DIN2_OUT(0); }
    DL_TimerG_setCaptureCompareValue(PWM_0_INST, ABS(pwmd), GPIO_PWM_0_C1_IDX);
}

/***************************************************************************
函数功能：电机的PID闭环控制
入口参数：左右电机的编码器值
返回值  ：电机的PWM
***************************************************************************/
int Velocity_A(int TargetVelocity, int CurrentVelocity)
{
    int Bias = TargetVelocity - CurrentVelocity;
    // 静态变量：上一次和上上一次的偏差，以及控制增量和
    static int Last_biasA    = 0;
    static int Last2_biasA   = 0;
    static int ControlA      = 0;

    // 增量式 PID：
    //   Δu = Kp*(e(k)-e(k-1)) + Ki*e(k) + Kd*(e(k)-2e(k-1)+e(k-2))
    float delta_u_f = Velocity_Kp * (Bias - Last_biasA)
                    + Velocity_Ki *  Bias
                    + Velocity_Kd * (Bias - 2*Last_biasA + Last2_biasA);
    int   delta_u   = (int)delta_u_f;

    ControlA += delta_u;

    // 更新历史偏差
    Last2_biasA = Last_biasA;
    Last_biasA  = Bias;

    // 输出限幅
    if (ControlA >  999) ControlA =  999;
    if (ControlA < -999) ControlA = -999;

    return ControlA;
}

int Velocity_B(int TargetVelocity, int CurrentVelocity)
{
    int Bias = TargetVelocity - CurrentVelocity;
    // 静态变量：上一次和上上一次的偏差，以及控制增量和
    static int Last_biasA    = 0;
    static int Last2_biasA   = 0;
    static int ControlA      = 0;

    // 增量式 PID：
    //   Δu = Kp*(e(k)-e(k-1)) + Ki*e(k) + Kd*(e(k)-2e(k-1)+e(k-2))
    float delta_u_f = Velocity_Kp * (Bias - Last_biasA)
                    + Velocity_Ki *  Bias
                    + Velocity_Kd * (Bias - 2*Last_biasA + Last2_biasA);
    int   delta_u   = (int)delta_u_f;

    ControlA += delta_u;

    // 更新历史偏差
    Last2_biasA = Last_biasA;
    Last_biasA  = Bias;

    // 输出限幅
    if (ControlA >  999) ControlA =  999;
    if (ControlA < -999) ControlA = -999;

    return ControlA;
}

int Velocity_C(int TargetVelocity, int CurrentVelocity)
{
    int Bias = TargetVelocity - CurrentVelocity;
    // 静态变量：上一次和上上一次的偏差，以及控制增量和
    static int Last_biasA    = 0;
    static int Last2_biasA   = 0;
    static int ControlA      = 0;

    // 增量式 PID：
    //   Δu = Kp*(e(k)-e(k-1)) + Ki*e(k) + Kd*(e(k)-2e(k-1)+e(k-2))
    float delta_u_f = Velocity_Kp * (Bias - Last_biasA)
                    + Velocity_Ki *  Bias
                    + Velocity_Kd * (Bias - 2*Last_biasA + Last2_biasA);
    int   delta_u   = (int)delta_u_f;

    ControlA += delta_u;

    // 更新历史偏差
    Last2_biasA = Last_biasA;
    Last_biasA  = Bias;

    // 输出限幅
    if (ControlA >  999) ControlA =  999;
    if (ControlA < -999) ControlA = -999;

    return ControlA;
}
int Velocity_D(int TargetVelocity, int CurrentVelocity)
{
    int Bias = TargetVelocity - CurrentVelocity;
    // 静态变量：上一次和上上一次的偏差，以及控制增量和
    static int Last_biasA    = 0;
    static int Last2_biasA   = 0;
    static int ControlA      = 0;

    // 增量式 PID：
    //   Δu = Kp*(e(k)-e(k-1)) + Ki*e(k) + Kd*(e(k)-2e(k-1)+e(k-2))
    float delta_u_f = Velocity_Kp * (Bias - Last_biasA)
                    + Velocity_Ki *  Bias
                    + Velocity_Kd * (Bias - 2*Last_biasA + Last2_biasA);
    int   delta_u   = (int)delta_u_f;

    ControlA += delta_u;

    // 更新历史偏差
    Last2_biasA = Last_biasA;
    Last_biasA  = Bias;

    // 输出限幅
    if (ControlA >  999) ControlA =  999;
    if (ControlA < -999) ControlA = -999;

    return ControlA;
}

/******************************************************************
 * 函 数 名 称：Motor_Get_Encoder
 * 函 数 说 明：获取编码器的值
 * 函 数 形 参：dir=0获取前左轮编码器值  dir=1获取前右轮编码器值 
               dir=2获取后左轮编码器值  dir=3获取后右轮编码器值
 * 函 数 返 回：返回对应的编码器值
 * 作       者：LCKFB
 * 备       注：无
******************************************************************/
int Motor_Get_Encoder(int dir)
{
	switch (dir){
        case 0:return Encoder_A.Obtained_Get_Encoder_Count;
        case 1:return Encoder_B.Obtained_Get_Encoder_Count;
        case 2:return Encoder_C.Obtained_Get_Encoder_Count;
        case 3:return Encoder_D.Obtained_Get_Encoder_Count;
        default: break;
    }
    return 0;
}

/*******************************************************
函数功能：外部中断模拟编码器信号
入口函数：无
返回  值：无
***********************************************************/
void GROUP1_IRQHandler(void)
{
    uint32_t gpio_interrup = 0;
    uint32_t s1 = 0;
    uint32_t s2 = 0;
    
	//获取中断信号
	s1 = DL_GPIO_getEnabledInterruptStatus(ENCODER_E1A_PORT,ENCODER_E1A_PIN);
    s2 = DL_GPIO_getEnabledInterruptStatus(ENCODER_E1B_PORT,ENCODER_E1B_PIN|ENCODER_E2A_PIN|ENCODER_E2B_PIN|
                                           ENCODER_E3A_PIN|ENCODER_E3B_PIN|ENCODER_E4A_PIN|ENCODER_E4B_PIN );
    gpio_interrup = s1 | s2;
    // encoderA
	if((gpio_interrup & ENCODER_E1A_PIN)==ENCODER_E1A_PIN)
	{
		if(!DL_GPIO_readPins(ENCODER_E1B_PORT,ENCODER_E1B_PIN))
		{
			Encoder_A.Should_Get_Encoder_Count--;
		}
		else
		{
			Encoder_A.Should_Get_Encoder_Count++;
		}
	}
	else if((gpio_interrup & ENCODER_E1B_PIN)==ENCODER_E1B_PIN)
	{
		if(!DL_GPIO_readPins(ENCODER_E1A_PORT,ENCODER_E1A_PIN))
		{
			Encoder_A.Should_Get_Encoder_Count++;
		}
		else
		{
			Encoder_A.Should_Get_Encoder_Count--;
		}
	}
    
	// encoderB
	if((gpio_interrup & ENCODER_E2A_PIN)==ENCODER_E2A_PIN)
	{
		if(!DL_GPIO_readPins(ENCODER_E1B_PORT,ENCODER_E2B_PIN))
		{
			Encoder_B.Should_Get_Encoder_Count--;
		}
		else
		{
			Encoder_B.Should_Get_Encoder_Count++;
		}
	}
	else if((gpio_interrup & ENCODER_E2B_PIN)==ENCODER_E2B_PIN)
	{
		if(!DL_GPIO_readPins(ENCODER_E1B_PORT,ENCODER_E2A_PIN))
		{
			Encoder_B.Should_Get_Encoder_Count++;
		}
		else
		{
			Encoder_B.Should_Get_Encoder_Count--;
		}
	}

    // encoderC
	if((gpio_interrup & ENCODER_E3A_PIN)==ENCODER_E3A_PIN)
	{
		if(!DL_GPIO_readPins(ENCODER_E1B_PORT,ENCODER_E3B_PIN))
		{
			Encoder_C.Should_Get_Encoder_Count--;
		}
		else
		{
			Encoder_C.Should_Get_Encoder_Count++;
		}
	}
	else if((gpio_interrup & ENCODER_E3B_PIN)==ENCODER_E3B_PIN)
	{
		if(!DL_GPIO_readPins(ENCODER_E1B_PORT,ENCODER_E3A_PIN))
		{
			Encoder_C.Should_Get_Encoder_Count++;
		}
		else
		{
			Encoder_C.Should_Get_Encoder_Count--;
		}
	}

    // encoderD
	if((gpio_interrup & ENCODER_E4A_PIN)==ENCODER_E4A_PIN)
	{
		if(!DL_GPIO_readPins(ENCODER_E1B_PORT,ENCODER_E4B_PIN))
		{
			Encoder_D.Should_Get_Encoder_Count--;
		}
		else
		{
			Encoder_D.Should_Get_Encoder_Count++;
		}
	}
	else if((gpio_interrup & ENCODER_E4B_PIN)==ENCODER_E4B_PIN)
	{
		if(!DL_GPIO_readPins(ENCODER_E1B_PORT,ENCODER_E4A_PIN))
		{
			Encoder_D.Should_Get_Encoder_Count++;
		}
		else
		{
			Encoder_D.Should_Get_Encoder_Count--;
		}
	}
    DL_GPIO_clearInterruptStatus(ENCODER_E1A_PORT,ENCODER_E1A_PIN);
    DL_GPIO_clearInterruptStatus(ENCODER_E1B_PORT,ENCODER_E1B_PIN|ENCODER_E2A_PIN|ENCODER_E2B_PIN|
                                 ENCODER_E3A_PIN|ENCODER_E3B_PIN|ENCODER_E4A_PIN|ENCODER_E4B_PIN );
}

//电机编码器脉冲计数
void TIMER_0_INST_IRQHandler(void)
{
	//编码器速度计算
	if( DL_TimerG_getPendingInterrupt(TIMER_0_INST) == DL_TIMER_IIDX_ZERO )
	{
        /* 两个电机安装相反，所以编码器值也要相反 */
        Encoder_A.Obtained_Get_Encoder_Count = -Encoder_A.Should_Get_Encoder_Count;
        Encoder_B.Obtained_Get_Encoder_Count = Encoder_B.Should_Get_Encoder_Count;
        Encoder_C.Obtained_Get_Encoder_Count = -Encoder_C.Should_Get_Encoder_Count;
        Encoder_D.Obtained_Get_Encoder_Count = Encoder_D.Should_Get_Encoder_Count;

        /* 编码器计数值清零 */
        Encoder_A.Should_Get_Encoder_Count = 0;
        Encoder_B.Should_Get_Encoder_Count = 0;
        Encoder_C.Should_Get_Encoder_Count = 0;
        Encoder_D.Should_Get_Encoder_Count = 0;

		int pwmA = (int)Velocity_A(pwma, Encoder_A.Obtained_Get_Encoder_Count);
        int pwmB = (int)Velocity_B(pwmb, Encoder_B.Obtained_Get_Encoder_Count);
        int pwmC = (int)Velocity_C(pwmc, Encoder_C.Obtained_Get_Encoder_Count);
        int pwmD = (int)Velocity_D(pwmd, Encoder_D.Obtained_Get_Encoder_Count);
        Motor_Set_PWM(pwmA, pwmB, pwmC, pwmD);
	}
}