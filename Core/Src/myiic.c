#include "myiic.h"
#include "delay.h"
#include "main.h"

//IIC初始化
void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();   //使能GPIOB时钟
    
    //PH4,5初始化设置
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed= GPIO_SPEED_HIGH;     //快速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    IIC_SDA(GPIO_PIN_SET);
    IIC_SCL(GPIO_PIN_SET);  
}
void SDA_SET_IN()
{
	  GPIO_InitTypeDef GPIO_Initure;
    GPIO_Initure.Pin=GPIO_PIN_9;
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //设置输入
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed= GPIO_SPEED_HIGH;     //快速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

}
void SDA_SET_OUT()
{
    GPIO_InitTypeDef GPIO_Initure;
    GPIO_Initure.Pin=GPIO_PIN_9;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed= GPIO_SPEED_HIGH;     //快速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

}

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA(GPIO_PIN_SET);	  	  
	IIC_SCL(GPIO_PIN_SET);
	delay_us(20);
 	IIC_SDA(GPIO_PIN_RESET);//START:when CLK is high,DATA change form high to low 
	delay_us(20);
	IIC_SCL(GPIO_PIN_RESET);//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL(GPIO_PIN_RESET);
	IIC_SDA(GPIO_PIN_RESET);//STOP:when CLK is high DATA change form low to high
 	delay_us(20);
	IIC_SCL(GPIO_PIN_SET); 
	delay_us(20);			
	IIC_SDA(GPIO_PIN_SET);//发送I2C总线结束信号				   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA(GPIO_PIN_SET);delay_us(20);	   
	IIC_SCL(GPIO_PIN_SET);delay_us(20);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL(GPIO_PIN_RESET);//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL(GPIO_PIN_RESET);
	SDA_OUT();
	IIC_SDA(GPIO_PIN_RESET);
	delay_us(20);
	IIC_SCL(GPIO_PIN_SET);
	delay_us(20);
	IIC_SCL(GPIO_PIN_RESET);
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL(GPIO_PIN_RESET);
	SDA_OUT();
	IIC_SDA(GPIO_PIN_SET);
	delay_us(20);
	IIC_SCL(GPIO_PIN_SET);
	delay_us(20);
	IIC_SCL(GPIO_PIN_RESET);
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	  SDA_OUT(); 
    delay_us(10);	
    IIC_SCL(GPIO_PIN_RESET);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        if((txd&0x80)>>7)
				{
				IIC_SDA(GPIO_PIN_SET);
				}
				else{
				IIC_SDA(GPIO_PIN_RESET);
				}	
        txd<<=1; 	  
		delay_us(20);   //对TEA5767这三个延时都是必须的
		IIC_SCL(GPIO_PIN_SET);
		delay_us(20); 
		IIC_SCL(GPIO_PIN_RESET);	
		delay_us(20);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
//	IIC_SDA=0;
	
	SDA_IN();//SDA设置为输入
	delay_us(10);
    for(i=0;i<8;i++ )
	{
        IIC_SCL(GPIO_PIN_RESET); 
        delay_us(20);
		    IIC_SCL(GPIO_PIN_SET);
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(20); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}


