#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "Serial.h"
//// -------------------- ȫ�ֱ��� --------------------
volatile uint8_t send_flag = 0; // 50Hz ���ͱ�־
uint8_t debug_output = 0; // ���������־
Gbc2GcuPkt_t g_response; // �������ݻ���
float ned_angle[3] = {0}; // ��̬�ǣ���λ 1deg
// ģ���ػ�����
float simulated_ned_angle[3] = {0.0f, 0.0f, 0.0f}; // �ػ���̬�ǣ���λ 1deg
float simulated_ned_accel[3] = {0.0f, 0.0f, 0.0f}; // �ػ����ٶȣ���λ 1m/s2

uint8_t Serial_RxPacket_2[7];
uint8_t Serial_RxFlag_2;
uint8_t RxData;

void UART1_Init(void)
{
 GPIO_InitTypeDef GPIO_InitStruct;
 USART_InitTypeDef USART_InitStruct;
 NVIC_InitTypeDef NVIC_InitStruct;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
 // PA9(TX): �����������
 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOA, &GPIO_InitStruct);
 // PA10(RX): ��������
 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 GPIO_Init(GPIOA, &GPIO_InitStruct);
// USART1 ���ã�115200, 8N1
 USART_InitStruct.USART_BaudRate = 115200;
 USART_InitStruct.USART_WordLength = USART_WordLength_8b;
 USART_InitStruct.USART_StopBits = USART_StopBits_1;
 USART_InitStruct.USART_Parity = USART_Parity_No;
 USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
 USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
 USART_Init(USART1, &USART_InitStruct);
 // ʹ�ܽ����ж�
 USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
 USART_Cmd(USART1, ENABLE);
 // �����ж����ȼ�
 NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
 NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
 NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStruct);
}
void UART2_Init(void)
{
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);    //开启USART时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //开启GPIO时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;  //结构体
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出，一个IO口只能有一个输出，但是可以有多个输入  串口空闲状态时是高电平，不能使用下拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;         //配置通道映射引脚  TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);      //初始化 GPIOA 结构体

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //上拉输入，一个IO口只能有一个输出，但是可以有多个输入  串口空闲状态时是高电平，不能使用下拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;         //配置通道映射引脚   RX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);      //初始化 GPIOA 结构体
	
	USART_InitTypeDef USART_InitStructure;     //结构体
	USART_InitStructure.USART_BaudRate = 115200;    //波特率 9600 11520
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //硬件流控制（不使用控流）
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //TX 发送   Rx接收
	USART_InitStructure.USART_Parity = USART_Parity_No;    //无校验   （无校验 奇校验 偶校验）
	USART_InitStructure.USART_StopBits = USART_StopBits_1;    //停止位   一般使用1位校验位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;   //字长 八位或者九位
	USART_Init(USART2, &USART_InitStructure);       //初始化 USART 结构体
	//如果使用（查询/中断）中断模式  开NVIC通道
	//一旦置1   就向NVIC申请中断 在中断中接收数据
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);  //（USART_CR1寄存器中的RXNEIE为1，则产生中断。）开启RXNE标志位到NVIC的输出 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断分组

	NVIC_InitTypeDef    NVIC_InitStructure;   //初始化NVIC的USART1通道
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;     //中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;  //优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //优先级
	
	NVIC_Init(&NVIC_InitStructure);
	USART_Cmd(USART2, ENABLE);
}
void TIM2_Init(void)
{
 TIM_TimeBaseInitTypeDef TIM_InitStruct;
 NVIC_InitTypeDef NVIC_InitStruct;
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
 // ���ö�ʱ��Ϊ 50H��z 72MHz ��Ƶ��
 TIM_InitStruct.TIM_Prescaler = 7200 - 1; // 72MHz / 7200 = 10kHz
 TIM_InitStruct.TIM_Period = 200 - 1; // 10kHz / 200 = 50Hz
 TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
 TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
 TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
 // ʹ���ж�
 TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
 TIM_Cmd(TIM2, DISABLE);
 // �����ж����ȼ�
 NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
 NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
 NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStruct);
}
// -------------------- ���Ϳ����������Ҫ������̨ģʽ�������ػ����� ---------------
uint8_t trig_counter = 0;   // �����������
void SendControlPacket(float x, float y)
{
 static uint8_t last_cmd = 0;  // ��һ��������
 Gcu2GbcPkt_t packet = {0};
 packet.sync[0] = 0xA9;
 packet.sync[1] = 0x5B;
 packet.cmd.value = 4; // ������ 4���ֶ�����
// packet.cmd.trig = 0; // �����ظ������ֶ��������ֻ��ִ��һ�μ���
 // �����������������Ҫ����ͬ������ı����ֵ��
    if(last_cmd == 4) {
        trig_counter = (trig_counter + 1) % 8;  // 0-7ѭ������
    } else {
        trig_counter = 0;
    }
    last_cmd = 4;
    packet.cmd.trig = trig_counter;
 // ��ת�᣺����ģʽ + �Ƕȿ��ƣ�0deg��
 packet.gbc[0].wk_mode = 1; // ����ģʽ
 packet.gbc[0].op_type = 0; // �Ƕȿ���
 packet.gbc[0].op_value = 0;
 // �����᣺����ģʽ + ��ʵ���ٶȿ��ƣ�0deg/s��
 packet.gbc[1].wk_mode = 2;
 packet.gbc[1].op_type = 0;
 packet.gbc[1].op_value = x*100;
 // ƫ���᣺����ģʽ + ��ʵ���ٶȿ��ƣ�0deg/s��+
 packet.gbc[2].wk_mode = 2;
 packet.gbc[2].op_type = 0;
 packet.gbc[2].op_value = y*100;
 // �ػ����ݣ�ģ��ֵ��
 packet.uav.valid = 0; // �ػ�������Ч������׼ȷ��ʵ���ػ���̬�Ǻͼ��ٶȣ������������ 0�����������ػ����ݻᵼ�´������̨����̬��
 // ŷ����ת����deg-> 0.01deg��
 packet.uav.angle[0] = (int16_t)(simulated_ned_angle[0] * 100); // ��ת
 packet.uav.angle[1] = (int16_t)(simulated_ned_angle[1] * 100); // ����
 packet.uav.angle[2] = (int16_t)(simulated_ned_angle[2] * 100); // ƫ��
 // ���ٶ�ת����m/s2-> 0.01m/s2��
 packet.uav.accel[0] = (int16_t)(simulated_ned_accel[0] * 100); // ����
 packet.uav.accel[1] = (int16_t)(simulated_ned_accel[1] * 100); // ����
 packet.uav.accel[2] = (int16_t)(simulated_ned_accel[2] * 100); // ����
// CRC16 У��
 uint16_t crc = CalculateCrc16((uint8_t *)&packet, sizeof(packet) - 2);
 packet.crc[0] = (crc>> 8) & 0xFF;
 packet.crc[1] = crc& 0xFF;
 for (uint8_t i = 0; i<sizeof(packet); i++)
 {
 USART_SendData(USART1, *((uint8_t *)&packet + i));
 while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
 ;
 }
 if (debug_output)
 {
 PrintSendFrame(&packet);
 }
}
// -------------------- UART1 �����ж� --------------------
void USART1_IRQHandler(void)
{
 static uint8_t rx_buffer[sizeof(Gbc2GcuPkt_t)];
 static uint8_t rx_index = 0;
 if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
 {
 rx_buffer[rx_index++] = USART_ReceiveData(USART1);
 // ��������һ֡����
 if (rx_index == sizeof(Gbc2GcuPkt_t))
 {
 Gbc2GcuPkt_t *pkt = (Gbc2GcuPkt_t *)rx_buffer;
 // У��Э��ͷ�� CRC
 if (pkt->sync[0] == 0xB5 && pkt->sync[1] == 0x9A)
 {
 uint16_t crc = CalculateCrc16(rx_buffer, sizeof(Gbc2GcuPkt_t) - 2);
 if (pkt->crc[0] == (crc>> 8) && pkt->crc[1] == (crc& 0xFF))
 {
 memcpy(&g_response, pkt, sizeof(Gbc2GcuPkt_t));
 }
 }
 rx_index = 0;
 }
 }
}

void USART2_IRQHandler(void)
{
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		RxData = USART_ReceiveData(USART2);
		if (RxState == 0)     //等待包头
		{
			if (RxData == 0xAA)  //接受包头
			{
				RxState = 1;
				pRxPacket = 0;
			}
		}
		else if (RxState == 1)   //等待数据     多if判断时用else if 因为全局变量变化的时候 可能会造成两个if判断都成立的结果
		{
			Serial_RxPacket_2[pRxPacket] = RxData;    //缓存数据    每进入一次判断 缓存一次数组中的一位 位置每一次都加一
			pRxPacket ++;
			if (pRxPacket >= 6)
			{
				RxState = 2;
			}
		}
		else if (RxState == 2)   //等待包尾
		{
			if (RxData == 0xFF)     //接收包尾
			{
				RxState = 0;
				Serial_RxFlag_2 = 1;
			}
		}
		
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}

// -------------------- CRC16 У�麯�� --------------------
uint16_t CalculateCrc16(uint8_t *ptr, uint8_t len)
{
 uint16_t crc = 0;
 const uint16_t crc_ta[16] = {
 0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
 0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF};
 while (len--)
 {
 uint8_t da = (crc>> 12);
 crc = (crc<< 4) ^ crc_ta[da ^ (*ptr>> 4)];
 da = (crc>> 12);
 crc = (crc<< 4) ^ crc_ta[da ^ (*ptr& 0x0F)];
 ptr++;
 }
 return crc;
}


// -------------------- ���ߺ��� --------------------
int fputc(int ch, FILE *f)
{
 USART_SendData(USART2, (uint8_t)ch);
 while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
 ;
 return ch;
}
// -------------------- ������Ϣ��� --------------------
void PrintSendFrame(const Gcu2GbcPkt_t *pkt)
{
 printf("\nSend frame: ");
 for (uint8_t i = 0; i<sizeof(Gcu2GbcPkt_t); i++) {
 printf("%02X ", ((uint8_t *)pkt)[i]);
 }
 printf("\n");
 printf("Parse send frame: \n");
 printf("cmdTrig : %-1d\n", pkt->cmd.trig);
 printf("cmdValue : %-2d\n", pkt->cmd.value);
 printf("FPV Sens : %-2d\n", pkt->aux.ff_sens);
 // ���������Ϣ
 const char *axis_names[] = {" ��ת ", " ���� ", " ƫ�� "};
 const char *wk_mode[] = {" ����ģʽ ", " ����ģʽ ", "FPV ģʽ "};
 const char *op_type[] = {" �Ƕȿ��� ", " �������ٶȿ��� ", " ��ʵ���ٶȿ��� "};
 for (uint8_t i = 0; i< 3; i++) {
 printf("%-5s : ", axis_names[i]);
 printf("go_zero=%-1d ", pkt->gbc[i].go_zero);
 printf("wk_mode=%-10s ", wk_mode[pkt->gbc[i].wk_mode<2 ? pkt->gbc[i].wk_mode : 2]);
 printf("op_type=%-10s ", op_type[pkt->gbc[i].op_type<3 ? pkt->gbc[i].op_type : 3]);
 printf("op_value=%-2d \n", pkt->gbc[i].op_value);
 }
 // �ػ���Ϣ
 printf("uav valid: %-1d\n", pkt->uav.valid);
 printf("uav Angle: [%6.2f, %6.2f, %6.2f] deg\n", 
 pkt->uav.angle[0]*0.01f, pkt->uav.angle[1]*0.01f, pkt->uav.angle[2]*0.01f);
 printf("uav Accel: [%6.2f, %6.2f, %6.2f] m/s2\n", 
 pkt->uav.accel[0]*0.01f, pkt->uav.accel[1]*0.01f, pkt->uav.accel[2]*0.01f);
 // �����Ϣ
 printf("cam vert_fovlx : %-3d\n", pkt->cam.vert_fovlx);
 printf("cam zoom_value : %-6d\n", pkt->cam.zoom_value);
 printf("cam target_angle : [%6.2f, %6.2f] deg\n", 
 pkt->cam.target_angle[0], pkt->cam.target_angle[1]);
}
void PrintGimbalStatus(const Gbc2GcuPkt_t *pkt)
{
 printf("\nRecv frame: ");
 for (uint8_t i = 0; i<sizeof(Gbc2GcuPkt_t); i++)
 {
 printf("%02X ", ((uint8_t *)pkt)[i]);
 }
 printf("\n");
 printf("Parse recv frame: \n");
 printf("FW Ver: %-2d\n", pkt->fw_ver);
 printf("HW Err: %-2d\n", pkt->hw_err);
 printf("Inv Flag: %-1d\n", pkt->inv_ffag);
 printf("GBC Stat: %-1d\n", pkt->gbc_stat);
 printf("TCA Flag: %-1d\n", pkt->tca_ffag);
 printf("Cmd Stat: %-1d\n", pkt->cmd.stat);
 printf("Cmd Value: %-2d\n", pkt->cmd.value);
 printf("Cam Rate: [%6d , %6d , %6d]\n",
 pkt->cam_rate[0],
 pkt->cam_rate[1],
 pkt->cam_rate[2]);
 printf("Cam Angle: [%6d , %6d , %6d]\n",
 pkt->cam_angle[0],
 pkt->cam_angle[1],
 pkt->cam_angle[2]);
 printf("Mtr Angle: [%6d , %6d , %6d]\n",
 pkt->mtr_angle[0],
 pkt->mtr_angle[1],
 pkt->mtr_angle[2]);
 for (uint8_t i = 0; i< 3; i++)
 ned_angle[i] = pkt->cam_angle[i] * 0.01f;
 printf("NED Angle: [%6.2f , %6.2f , %6.2f]\n\n",
 ned_angle[0],
 ned_angle[1],
 ned_angle[2]);
}
