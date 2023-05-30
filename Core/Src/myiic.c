#include "myiic.h"
#include "delay.h"
#include "main.h"

//IIC��ʼ��
void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();   //ʹ��GPIOBʱ��
    
    //PH4,5��ʼ������
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed= GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    IIC_SDA(GPIO_PIN_SET);
    IIC_SCL(GPIO_PIN_SET);  
}
void SDA_SET_IN()
{
	  GPIO_InitTypeDef GPIO_Initure;
    GPIO_Initure.Pin=GPIO_PIN_9;
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //��������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed= GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

}
void SDA_SET_OUT()
{
    GPIO_InitTypeDef GPIO_Initure;
    GPIO_Initure.Pin=GPIO_PIN_9;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed= GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

}

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA(GPIO_PIN_SET);	  	  
	IIC_SCL(GPIO_PIN_SET);
	delay_us(20);
 	IIC_SDA(GPIO_PIN_RESET);//START:when CLK is high,DATA change form high to low 
	delay_us(20);
	IIC_SCL(GPIO_PIN_RESET);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL(GPIO_PIN_RESET);
	IIC_SDA(GPIO_PIN_RESET);//STOP:when CLK is high DATA change form low to high
 	delay_us(20);
	IIC_SCL(GPIO_PIN_SET); 
	delay_us(20);			
	IIC_SDA(GPIO_PIN_SET);//����I2C���߽����ź�				   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	IIC_SCL(GPIO_PIN_RESET);//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	  SDA_OUT(); 
    delay_us(10);	
    IIC_SCL(GPIO_PIN_RESET);//����ʱ�ӿ�ʼ���ݴ���
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
		delay_us(20);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL(GPIO_PIN_SET);
		delay_us(20); 
		IIC_SCL(GPIO_PIN_RESET);	
		delay_us(20);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
//	IIC_SDA=0;
	
	SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}


