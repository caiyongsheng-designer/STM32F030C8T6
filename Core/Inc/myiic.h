#ifndef _MYIIC_H
#define _MYIIC_H
#include "main.h"
		  
////////////////////////////////////////////////////////////////////////////////// 	
//IO��������
#define SDA_IN()  SDA_SET_IN();	//PB9����ģʽ
#define SDA_OUT() SDA_SET_OUT();
//IO����
#define IIC_SCL(x)  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,x); //SCL
#define IIC_SDA(x)  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,x); //SDA
#define READ_SDA    HAL_GPIO_ReadPin (GPIOB,GPIO_PIN_9) //����SDA

//IIC���в�������
void SDA_SET_IN(void);
void SDA_SET_OUT(void);
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	 
#endif

