#ifndef RF_H_
#define RF_H_



#pragma pack(1)

typedef struct _rf_cmd
{
	unsigned short head;
	unsigned short id;
	unsigned short code;
	unsigned char cmd;
	unsigned char data[4];
	unsigned char crc;
#define CMD_HAND_ZZ                   1	 //ң����תָ��
#define CMD_HAND_FZ                   2	 //ң�ط�תָ��	
#define CMD_HAND_GET_DEV_STAT         3  //ң�ػ�ȡ�豸״ָ̬��
#define CMD_HAND_SET_DEV_ZZ_SEC       4  //ң�������豸��תʱ��
#define CMD_HAND_SET_DEV_FZ_SEC       5  //ң�������豸��תʱ��
#define CMD_HAND_GET_DEV_ZZ_SEC       6  //ң�ػ�ȡ�豸��תʱ��
#define CMD_HAND_GET_DEV_FZ_SEC       7  //ң�ػ�ȡ�豸��תʱ��	
#define CMD_HAND_SET_DEV_ID           8  //ң�������豸ID
#define CMD_HAND_SET_DEV_CODE         9  //ң�������豸�������
#define CMD_HAND_SET_DEV_CURRENT_L    10  //ң�������豸�����ȼ�
#define CMD_HAND_GET_DEV_CURRENT_L    11  //ң�������豸�����ȼ�	
#define CMD_HAND_SET_LOCK_TIME        12  //��������ʱ��
#define CMD_HAND_GET_LOCK_TIME        13  //��������ʱ��
	
	
#define CMD_DEV_STAT                  50   //�豸����״̬	
#define CMD_DEV_ZZ_SEC                51   //�豸������תʱ��
#define CMD_DEV_FZ_SEC                52   //�豸���ط�תʱ��	
#define CMD_DEV_NEW_ID                53   //�豸������ID��Ȼ����л�ID
#define CMD_DEV_NEW_CODE              54   //�豸������CODE Ȼ����л�CODE
#define CMD_DEV_CURRENT_L             55   //�豸���ص����ȼ�
#define CMD_DEV_LOCK_TIME             56	 //��������ʱ��
	
	
	
}struct_rf_cmd;

#pragma pack()


typedef enum _enum_motor_status
{
  none_,              
  forward,                  //��ת
  forward_to_stop,          //��תֹͣ
  forward_time_to_stop,     //��תʱ�䵽��ֹͣ
  reverse,                  //��ת
  reverse_to_stop,        //��תֹͣ
  reverse_time_to_stop,     //��תʱ�䵽��ֹͣ
  forward_overflow_to_stop,  //��ת���������ƺ�ֹͣ
  reverse_voceflow_to_stop,  //��ת���������ƺ�ֹͣ
}enummotor_status;


extern struct_rf_cmd rf_cmd;


void rf_send_cmd(unsigned short id,unsigned char cmd,unsigned int data);




#endif


