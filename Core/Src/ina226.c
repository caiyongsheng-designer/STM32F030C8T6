#include "ina226.h"
#include "delay.h"
#include "myiic.h"

uint16_t INA226_Read2Byte(uint8_t reg_addr)
{
	uint16_t reg_data=0;
	uint16_t temp=0;
	IIC_Start();
	IIC_Send_Byte(WRITE_ADDR);
	if(IIC_Wait_Ack())return 0;
	IIC_Send_Byte(reg_addr);   
	if(IIC_Wait_Ack())return 0;
	IIC_Start();
	IIC_Send_Byte(READ_ADDR);
	if(IIC_Wait_Ack())return 0;
	reg_data= IIC_Read_Byte(1);
	reg_data=(reg_data<<8)&0xFF00;
	temp=IIC_Read_Byte(0);
	IIC_Stop();
	reg_data|=temp;
	return reg_data;
}

uint8_t INA226_Write2Byte(uint8_t reg_addr,uint16_t reg_data)
{        
	uint8_t data_high=(uint8_t)((reg_data&0xFF00)>>8);
	uint8_t data_low=(uint8_t)reg_data&0x00FF;
	IIC_Start();
	IIC_Send_Byte(WRITE_ADDR);   
	if(IIC_Wait_Ack())return 0;
	IIC_Send_Byte(reg_addr );    
	if(IIC_Wait_Ack())return 0;        
	IIC_Send_Byte(data_high);
	if(IIC_Wait_Ack())return 0;        
	IIC_Send_Byte(data_low);
	if(IIC_Wait_Ack())return 0;                 
	IIC_Stop();
	delay_ms(2);
	return 1;
}

void INA226_Init(void)
{
	INA226_Write2Byte(Config_Reg, 0x4527);//0100_010_100_100_111 //16次平均,1.1ms,1.1ms,连续测量分流电压和总线电压
	INA226_Write2Byte(Calib_Reg, 0x0200);  //0x0a00
}
