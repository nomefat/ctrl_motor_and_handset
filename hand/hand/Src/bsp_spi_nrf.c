/**
  ******************************************************************************
  * @file    bsp_xxx.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   2.4g����ģ��/nrf24l01+/������� Ӧ��bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "bsp_spi_nrf.h"

extern SPI_HandleTypeDef hspi1;

 u8 RX_BUF[RX_PLOAD_WIDTH];		//�������ݻ���
 u8 TX_BUF[TX_PLOAD_WIDTH];		//�������ݻ���
 u8 TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};  // ����һ����̬���͵�ַ
 u8 RX_ADDRESS[RX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};

 
 enum_rf_mode rf_mode;
 u8 rf_send_status = 0;
u8 rxbuf[32];
 
 
void Delay(__IO u32 nCount)
{
  for(; nCount != 0; nCount--);
} 





/**
  * @brief   ������NRF�ض��ļĴ���д������
  * @param   
  *		@arg reg:NRF������+�Ĵ�����ַ
  *		@arg dat:��Ҫ��Ĵ���д�������
  * @retval  NRF��status�Ĵ�����״̬
  */
u8 SPI_NRF_WriteReg(u8 reg,u8 dat)
{
 	u8 ret;
	u8 data[2];
	u8 status[2];
	
	data[0] = reg;
	data[1] = dat;
	 NRF_CE_LOW();
	/*�õ�CSN��ʹ��SPI����*/
    NRF_CSN_LOW();
				
		ret = HAL_SPI_TransmitReceive(&hspi1,data,status,2,1);
	          
	/*CSN���ߣ����*/	   
  	NRF_CSN_HIGH();	
		
	/*����״̬�Ĵ�����ֵ*/
   	return(status[0]);
}

/**
  * @brief   ���ڴ�NRF�ض��ļĴ�����������
  * @param   
  *		@arg reg:NRF������+�Ĵ�����ַ
  * @retval  �Ĵ����е�����
  */
u8 SPI_NRF_ReadReg(u8 reg)
{
 	u8 ret;
	u8 data;
	NRF_CE_LOW();
	/*�õ�CSN��ʹ��SPI����*/
 	NRF_CSN_LOW();
				
	ret = HAL_SPI_TransmitReceive(&hspi1,&reg,&data,1,1);
	            
   	/*CSN���ߣ����*/
	NRF_CSN_HIGH();		
   	
	return data;
}	

/**
  * @brief   ������NRF�ļĴ�����д��һ������
  * @param   
  *		@arg reg : NRF������+�Ĵ�����ַ
  *		@arg pBuf�����ڴ洢���������ļĴ������ݵ����飬�ⲿ����
  * 	@arg bytes: pBuf�����ݳ���
  * @retval  NRF��status�Ĵ�����״̬
  */
u8 SPI_NRF_ReadBuf(u8 reg,u8 *pBuf,u8 bytes)
{
 	u8 ret, byte_cnt;
	u8 status;
	  NRF_CE_LOW();
	/*�õ�CSN��ʹ��SPI����*/
	NRF_CSN_LOW();
		
	/*���ͼĴ�����*/		
	ret = HAL_SPI_TransmitReceive(&hspi1,&reg,&status,1,1);

 	/*��ȡ����������*/
	ret = HAL_SPI_Receive(&hspi1,pBuf,bytes,1);

	 /*CSN���ߣ����*/
	NRF_CSN_HIGH();	
		
 	return status;		//���ؼĴ���״ֵ̬
}

/**
  * @brief   ������NRF�ļĴ�����д��һ������
  * @param   
  *		@arg reg : NRF������+�Ĵ�����ַ
  *		@arg pBuf���洢�˽�Ҫд��д�Ĵ������ݵ����飬�ⲿ����
  * 	@arg bytes: pBuf�����ݳ���
  * @retval  NRF��status�Ĵ�����״̬
  */
u8 SPI_NRF_WriteBuf(u8 reg ,u8 *pBuf,u8 bytes)
{
	 u8 status,byte_cnt,ret;
	 NRF_CE_LOW();
   	 /*�õ�CSN��ʹ��SPI����*/
	 NRF_CSN_LOW();			

	 /*���ͼĴ�����*/	
	ret = HAL_SPI_TransmitReceive(&hspi1,&reg,&status,1,1);
 	
  	  /*�򻺳���д������*/
	ret = HAL_SPI_Transmit(&hspi1,pBuf,bytes,1); 
	  	   
	/*CSN���ߣ����*/
	NRF_CSN_HIGH();			
  
  	return (status);	//����NRF24L01��״̬ 		
}

/**
  * @brief  ���ò��������ģʽ
  * @param  ��
  * @retval ��
  */
void NRF_RX_Mode(void)

{
	NRF_CE_LOW();	

   SPI_NRF_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ

   SPI_NRF_WriteReg(NRF_WRITE_REG+EN_AA,0x00);    //ʹ��ͨ��0���Զ�Ӧ��    

   SPI_NRF_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ    

   SPI_NRF_WriteReg(NRF_WRITE_REG+RF_CH,CHANAL);      //����RFͨ��Ƶ��    

   SPI_NRF_WriteReg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ��      

   SPI_NRF_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0f); //����TX�������,0db����,2Mbps,���������濪��   

   SPI_NRF_WriteReg(NRF_WRITE_REG+CONFIG, 0x0f);  //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 

/*CE���ߣ��������ģʽ*/	
  NRF_CE_HIGH();

	rf_mode = rf_rx;
}    

/**
  * @brief  ���÷���ģʽ
  * @param  ��
  * @retval ��
  */
void NRF_TX_Mode(void)
{  
	NRF_CE_LOW();		

   SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);    //дTX�ڵ��ַ 

   SPI_NRF_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK   

   SPI_NRF_WriteReg(NRF_WRITE_REG+EN_AA,0x00);     //ʹ��ͨ��0���Զ�Ӧ��    

   SPI_NRF_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x00); //ʹ��ͨ��0�Ľ��յ�ַ  

   SPI_NRF_WriteReg(NRF_WRITE_REG+SETUP_RETR,0x00);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��

   SPI_NRF_WriteReg(NRF_WRITE_REG+RF_CH,CHANAL);       //����RFͨ��ΪCHANAL

   SPI_NRF_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
	
   SPI_NRF_WriteReg(NRF_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�

/*CE���ߣ����뷢��ģʽ*/	
  NRF_CE_HIGH();
	
	rf_mode = rf_tx;
	
  Delay(1000000); //CEҪ����һ��ʱ��Ž��뷢��ģʽ
}

/**
  * @brief  ��Ҫ����NRF��MCU�Ƿ���������
  * @param  ��
  * @retval SUCCESS/ERROR ��������/����ʧ��
  */
u8 NRF_Check(void)
{
	u8 buf[5]={0xC2,0xC2,0xC2,0xC2,0xC2};
	u8 buf1[5];
	u8 i; 
	 
	/*д��5���ֽڵĵ�ַ.  */  
	SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR,buf,5);

	/*����д��ĵ�ַ */
	SPI_NRF_ReadBuf(TX_ADDR,buf1,5); 
	 
	/*�Ƚ�*/               
	for(i=0;i<5;i++)
	{
		if(buf1[i]!=0xC2)
		break;
	} 
	       
	if(i==5)
		return SUCCESS ;        //MCU��NRF�ɹ����� 
	else
		return ERROR ;        //MCU��NRF����������
}

/**
  * @brief   ������NRF�ķ��ͻ�������д������
  * @param   
  *		@arg txBuf���洢�˽�Ҫ���͵����ݵ����飬�ⲿ����	
  * @retval  ���ͽ�����ɹ�����TXDS,ʧ�ܷ���MAXRT��ERROR
  */
u8 NRF_Tx_Dat(u8 *txbuf,u8 len)
{
	u8 state;  

	 /*ceΪ�ͣ��������ģʽ1*/
	NRF_CE_LOW();

	/*д���ݵ�TX BUF ��� 32���ֽ�*/						
   SPI_NRF_WriteBuf(WR_TX_PLOAD,txbuf,len);

      /*CEΪ�ߣ�txbuf�ǿգ��������ݰ� */   
 	 NRF_CE_HIGH();
	  	
//	  /*�ȴ���������ж� */                            
//	while(NRF_Read_IRQ()!=0); 	
//	
//	/*��ȡ״̬�Ĵ�����ֵ */                              
//	state = SPI_NRF_ReadReg(STATUS);

//	 /*���TX_DS��MAX_RT�жϱ�־*/                  
//	SPI_NRF_WriteReg(NRF_WRITE_REG+STATUS,state); 	

//	SPI_NRF_WriteReg(FLUSH_TX,NOP);    //���TX FIFO�Ĵ��� 

//	 /*�ж��ж�����*/    
//	if(state&MAX_RT)                     //�ﵽ����ط�����
//			 return MAX_RT; 

//	else if(state&TX_DS)                  //�������
//		 	return TX_DS;
//	 else						  
//			return ERROR;                 //����ԭ����ʧ��
} 


void NRF_Tx_over(void)
{
		u8 state;  

	/*��ȡ״̬�Ĵ�����ֵ */                              
	state = SPI_NRF_ReadReg(STATUS);

	 /*���TX_DS��MAX_RT�жϱ�־*/                  
	SPI_NRF_WriteReg(NRF_WRITE_REG+STATUS,state); 	

	SPI_NRF_WriteReg(FLUSH_TX,NOP);    //���TX FIFO�Ĵ��� 

	rf_send_status = state;
//	 /*�ж��ж�����*/    
//	if(state&MAX_RT)                     //�ﵽ����ط�����
//			 rf_send_status = MAX_RT; 

//	else if(state&TX_DS)                  //�������
//		 	rf_send_status = TX_DS;
//	 else						  
//			rf_send_status = ERROR;                 //����ԭ����ʧ��	
//	 
	 rf_mode = rf_tx_over;
}






u8 fuck = 0;

/**
  * @brief   ���ڴ�NRF�Ľ��ջ������ж�������
  * @param   
  *		@arg rxBuf �����ڽ��ո����ݵ����飬�ⲿ����	
  * @retval 
  *		@arg ���ս��
  */
u8 NRF_Rx_Dat(void)
{
	u8 state; 
//	NRF_CE_HIGH();	 //�������״̬
//	 /*�ȴ������ж�*/
//	while(NRF_Read_IRQ()!=0)
//	{
//		fuck=SPI_NRF_ReadReg(STATUS);
//	}		
	
	NRF_CE_LOW();  	 //�������״̬
	/*��ȡstatus�Ĵ�����ֵ  */               
	state=SPI_NRF_ReadReg(STATUS);
	 
	/* ����жϱ�־*/      
	SPI_NRF_WriteReg(NRF_WRITE_REG+STATUS,state);

	/*�ж��Ƿ���յ�����*/
	if(state&RX_DR)                                 //���յ�����
	{
	  SPI_NRF_ReadBuf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
	  SPI_NRF_WriteReg(FLUSH_RX,NOP);          //���RX FIFO�Ĵ���
		NRF_CE_HIGH();	
	  return RX_DR; 
	}
	else    
		return ERROR;                    //û�յ��κ�����
}

void NRF_go_RX_mode(void)
{
	if(rf_mode == rf_tx_over)
	{
		NRF_RX_Mode();
	}
}

/*********************************************END OF FILE**********************/
