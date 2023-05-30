#include "main.h"
#include "stdio.h"
#include "delay.h"

static uint32_t fac_us=0;							//us��ʱ������

//��ʼ���ӳٺ���
//��ʹ��ucos��ʱ��,�˺������ʼ��ucos��ʱ�ӽ���
//SYSTICK��ʱ�ӹ̶�ΪAHBʱ��
//SYSCLK:ϵͳʱ��Ƶ��
void delay_init(uint8_t SYSCLK)
{
#if SYSTEM_SUPPORT_OS 						//�����Ҫ֧��OS.
	u32 reload;
#endif
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTickƵ��ΪHCLK
	  fac_us=SYSCLK;						//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
#if SYSTEM_SUPPORT_OS 						//�����Ҫ֧��OS.
	reload=SYSCLK;					    //ÿ���ӵļ������� ��λΪK	   
	reload*=1000000/delay_ostickspersec;	//����delay_ostickspersec�趨���ʱ��
											//reloadΪ24λ�Ĵ���,���ֵ:16777216,��180M��,Լ��0.745s����	
	fac_ms=1000/delay_ostickspersec;		//����OS������ʱ�����ٵ�λ	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//����SYSTICK�ж�
	SysTick->LOAD=reload; 					//ÿ1/OS_TICKS_PER_SEC���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //����SYSTICK
#else
#endif
}		

void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOAD��ֵ	    	 
	ticks=nus*fac_us; 						//��Ҫ�Ľ����� 
	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};
}

//��ʱnms
//nms:Ҫ��ʱ��ms��
void delay_ms(uint16_t nms)
{
	uint32_t i;
	for(i=0;i<nms;i++) delay_us(1000);
}

void delay_ns(uint32_t nus)
{
	while(nus--);
}