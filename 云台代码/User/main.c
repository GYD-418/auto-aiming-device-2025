#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include "Key.h"

float yaw_x,yaw_y;
extern uint8_t Serial_RxPacket_2[];
int x,y,last_x,last_y,count,aaa,bbb,gyd;
extern uint8_t RxData;
float xxx=10.0f,yyy=0.0f,x1=0.0f,y1=0.0f;
uint8_t hexToDec(uint8_t hexValue) {
  uint8_t decValue = 0;
  uint8_t base = 1;
  while (hexValue != 0) {
    uint8_t digit = hexValue % 10;
    decValue += digit * base;
    base *= 16;
    hexValue /= 10;
  }
  return decValue;
}

int main(void)
{
 SystemInit(); // 初始化系统时钟 (72MHz)
 UART1_Init(); // UART1：云台通信
 UART2_Init(); // UART2：调试输出
 TIM2_Init(); // TIM2：50Hz 中断
 LED_Init();
 Key_Init();
	PWM_Init();
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	Delay_ms(400);
	TIM_Cmd(TIM2,DISABLE);
//	PWM_WSetCompare(1550);
 while (1)
 {
	 uint8_t key_number = Key_GetNum();
	 if(key_number==2)
	 {		 
		 PWM_WSetCompare(1550);
		 TIM_Cmd(TIM2,ENABLE);
		 x = hexToDec(Serial_RxPacket_2[0])*100 + hexToDec(Serial_RxPacket_2[1])*10 + hexToDec(Serial_RxPacket_2[2])*1;//420，所需打击x中心
		 y = hexToDec(Serial_RxPacket_2[3])*100 + hexToDec(Serial_RxPacket_2[4])*10 + hexToDec(Serial_RxPacket_2[5])*1;//245，所需打击y中心
	 }
	 if(key_number==3)
	 {
		 PWM_WSetCompare(1500);
		 TIM_Cmd(TIM2,ENABLE);
		 x = hexToDec(Serial_RxPacket_2[0])*100 + hexToDec(Serial_RxPacket_2[1])*10 + hexToDec(Serial_RxPacket_2[2])*1;//420，所需打击x中心
		 y = hexToDec(Serial_RxPacket_2[3])*100 + hexToDec(Serial_RxPacket_2[4])*10 + hexToDec(Serial_RxPacket_2[5])*1;//245，所需打击y中心
	 }

		if(x!=0){PWM_WSetCompare(1500);}
	 if (send_flag==1)
	 {
		 
//		 x =420;//420，所需打击x中心
//		y = 245;//245，所需打击y中心
		 if(x!=0)
		 {
				x = hexToDec(Serial_RxPacket_2[0])*100 + hexToDec(Serial_RxPacket_2[1])*10 + hexToDec(Serial_RxPacket_2[2])*1;//420，所需打击x中心
				y = hexToDec(Serial_RxPacket_2[3])*100 + hexToDec(Serial_RxPacket_2[4])*10 + hexToDec(Serial_RxPacket_2[5])*1;//245，所需打击y中心
				if(x<380&&x>0){xxx-=0.7;}
			 else if(x>380&&x<415){xxx-=0.07;}
			 else if(x<640&&x>450){xxx+=0.7;}
			 else if(x>425&&x<460){xxx+=0.07;}
			 if(y<215&&y>0){yyy+=0.6;}
			 else if(y>215&&y<241){yyy+=0.06;}
			 else if(y>275&&y<480){yyy-=0.6;}
			 else if(y>249&&y<275){yyy-=0.06;}	 
		 if(x>415&&x<425){GPIO_SetBits(GPIOB,GPIO_Pin_5);}
		 SendControlPacket(yyy, xxx);  //设置无刷云台x和y角度		
		 }
		 send_flag = 0;
	 }
 }
}
// -------------------- 定时器中断（50Hz） --------------------
void TIM2_IRQHandler(void)
{
 if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
 {
	 static int count = 0;
	 count++;
	 if(count>2)
	 {
			send_flag = 1; // 触发发送
			count = 0;
	 }
	 
	 TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
 }
}

