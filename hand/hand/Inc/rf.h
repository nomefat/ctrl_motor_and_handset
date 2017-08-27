#ifndef RF_H_
#define RF_H_



#pragma pack(1)

typedef struct _rf_cmd
{
	unsigned short head;
	unsigned short id;
	unsigned short code;
	unsigned char cmd;
	unsigned int data;
	unsigned char crc;
#define CMD_HAND_ZZ                   1	 //遥控正转指令
#define CMD_HAND_FZ                   2	 //遥控反转指令	
#define CMD_HAND_GET_DEV_STAT         3  //遥控获取设备状态指令
#define CMD_HAND_SET_DEV_ZZ_SEC       4  //遥控设置设备正转时间
#define CMD_HAND_SET_DEV_FZ_SEC       5  //遥控设置设备反转时间
#define CMD_HAND_GET_DEV_ZZ_SEC       6  //遥控获取设备正转时间
#define CMD_HAND_GET_DEV_FZ_SEC       7  //遥控获取设备反转时间	
#define CMD_HAND_SET_DEV_ID           8  //遥控设置设备ID
#define CMD_HAND_SET_DEV_CODE         9  //遥控设置设备区域编码
#define CMD_HAND_SET_DEV_CURRENT_L    10  //遥控设置设备电流等级
#define CMD_HAND_GET_DEV_CURRENT_L    11  //遥控设置设备电流等级	
#define CMD_HAND_SET_LOCK_TIME        12  //设置锁定时间

	
	
#define CMD_DEV_STAT                  50   //设备返回状态	
#define CMD_DEV_ZZ_SEC                51   //设备返回正转时间
#define CMD_DEV_FZ_SEC                52   //设备返回反转时间	
#define CMD_DEV_NEW_ID                53   //设备返回新ID，然后会切换ID
#define CMD_DEV_NEW_CODE              54   //设备返回新CODE 然后会切换CODE
#define CMD_DEV_CURRENT_L             55   //设备返回电流等级
#define CMD_DEV_LOCK_TIME             56	 //返回锁定时间
	
	
	
}struct_rf_cmd;

#pragma pack()

extern struct_rf_cmd rf_cmd;


void rf_send_cmd(unsigned short id,unsigned char cmd,unsigned int data);




#endif


