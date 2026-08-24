#ifndef _SERIAL_H
#define _SERIAL_H

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>


// -------------------- Э�����ݰ����� --------------------
#pragma pack(1)
typedef struct
{
 uint8_t sync[2]; // Э��ͷ 0xA9 0x5B
 struct
 {
 uint8_t trig : 3; // ���������
 uint8_t value : 5; // ������
 } cmd;
 struct
 {
 uint8_t : 3;
 int8_t ff_sens : 5; // FPV ����������
 } aux;
 struct
 {
 uint8_t : 3;
 uint8_t go_zero : 1; // ���д���
 uint8_t wk_mode : 2; // ����ģʽ
 uint8_t op_type : 2; // ����ģʽ
 int16_t op_value; // ��̨������
 } gbc[3]; // 0- ��ת��1- ������2- ƫ��
 struct
 {
 uint8_t : 7;
 uint8_t valid : 1; // �ػ��ߵ�������Ч��־
 int16_t angle[3]; // �ػ���̬��
 int16_t accel[3]; // �ػ����ٶ�
 } uav;
 struct
 {
 uint32_t vert_fovlx : 7; // ��� 1x ��ֱ�ӳ���
 uint32_t zoom_value : 24; // �������
 uint32_t reserved : 1;
 float target_angle[2]; // Ŀ��ƫ�ƽǶ�
 } cam;
 uint8_t crc[2]; // CRC16 У��
} Gcu2GbcPkt_t;

typedef struct
{
 uint8_t sync[2]; // Э��ͷ 0xB5 0x9A
 uint8_t fw_ver; // �̼��汾��
 uint8_t hw_err; // Ӳ���������
 uint8_t inv_ffag : 1; // ��װ / ��װ��־
 uint8_t gbc_stat : 3; // ��̨״̬
 uint8_t tca_ffag : 1; // �¿ؾ�����־
 uint8_t : 3;
 struct
 {
 uint8_t stat : 3; // ����ִ��״̬
 uint8_t value : 5; // ������ر�
 } cmd;
 int16_t cam_rate[3]; // �����̬���ٶ�
 int16_t cam_angle[3]; // �����̬��
 int16_t mtr_angle[3]; // �����Խ�
 uint8_t crc[2]; // CRC16 У��
} Gbc2GcuPkt_t;
#pragma pack()
// -------------------- ȫ�ֱ��� --------------------
extern volatile uint8_t send_flag; // 50Hz ���ͱ�־
extern uint8_t debug_output; // ���������־
extern Gbc2GcuPkt_t g_response; // �������ݻ���
extern float ned_angle[3]; // ��̬�ǣ���λ 1deg
// ģ���ػ�����
extern float simulated_ned_angle[3]; // �ػ���̬�ǣ���λ 1deg
extern float simulated_ned_accel[3]; // �ػ����ٶȣ���λ 1m/s2
// -------------------- �������� --------------------
void UART1_Init(void);
void UART2_Init(void);
void TIM2_Init(void);
void SendControlPacket(float x, float y);
uint16_t CalculateCrc16(uint8_t *ptr, uint8_t len);
void PrintSendFrame(const Gcu2GbcPkt_t *pkt);
void PrintGimbalStatus(const Gbc2GcuPkt_t *pkt);
int fputc(int ch, FILE *f);

#endif
