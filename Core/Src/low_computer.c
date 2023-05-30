#include "main.h"
void LOW_COMPUTER_CTROL(void)
{
	  struct UART_RECEIVE Uart1_Receive; 
	  uint8_t SPI_data_len=0;
	  uint8_t  SPI_receive_data[100];
	  uint8_t  SPI_Return_Data[10];
	  uint16_t Vout = 0;
	  memset(SPI_Return_Data,0x00,sizeof(SPI_Return_Data));
    Uart1_Receive = Uart1_Receive_data();
    if 	((Uart1_Receive.UART1_Receive_Flag ==1 )&& (Uart1_Receive.UART1_Receive_data_len != 0))
		{
		   if((Uart1_Receive.UART1_Receive_data[0]==0x5a)&&
				 (Uart1_Receive.UART1_Receive_data[1]==0xa5)&&
			   (Uart1_Receive.UART1_Receive_data[2]==0x01))            //TLE75602芯片
			 {
				 switch (Uart1_Receive.UART1_Receive_data[4] )
				 {
					 case 1 : 
						SPI_data_len =  Uart1_Receive.UART1_Receive_data[3]-1;
					 if(SPI_data_len>=51)
					 {
					    DEBUG("可发送字节长度最大为50，超长请重新发送");
					 }else{
						  MX_SPI2_Init();
						  delay_us(100);
					    SPI_CS_Enable();						 
					    HAL_SPI_TransmitReceive(&hspi2,Uart1_Receive.UART1_Receive_data+5,
						                          SPI_receive_data,SPI_data_len, 1000);
						  SPI_CS_Disable();			 
					    for (uint8_t i=0;i<SPI_data_len;i++)
					    {
					     DEBUG("Reveive data %x",SPI_receive_data[i]);
					    }	 
					 }	

           break;
					 
					 case 2 : 
					 SPI_data_len =  Uart1_Receive.UART1_Receive_data[3]-1;
					 if(SPI_data_len>=51)
					 {
					    DEBUG("可发送字节长度最大为50，超长请重新发送");
					 }else{
						 
						  if(Uart1_Receive.UART1_Receive_data[5] == 0xff)
							{
								 if(Uart1_Receive.UART1_Receive_data[6] == 0x01)
											{
										    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
											}else if(Uart1_Receive.UART1_Receive_data[6] == 0x00)
											{
											  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
											}else{;}	
						   
						   USART1_IdleCallback(Uart1_Receive.UART1_Receive_data,5+SPI_data_len);		
							}
							else if(Uart1_Receive.UART1_Receive_data[5] == 0xfe)
							{
							 if(Uart1_Receive.UART1_Receive_data[6] == 0x01)
											{
										    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
											}else if(Uart1_Receive.UART1_Receive_data[6] == 0x00)
											{
											  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
											}else{};	
					
						 USART1_IdleCallback(Uart1_Receive.UART1_Receive_data,5+SPI_data_len);
							
							}else if(Uart1_Receive.UART1_Receive_data[5] == 0xfd)
							{
						 if(Uart1_Receive.UART1_Receive_data[6] == 0x01)
											{
										    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
												
											}else if(Uart1_Receive.UART1_Receive_data[6] == 0x00)
											{
											  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
												
											}else{;}
				
						 USART1_IdleCallback(Uart1_Receive.UART1_Receive_data,5+SPI_data_len);
							}else{
							
							MX_SPI2_Init();
						  delay_us(100);
					    SPI_CS_Enable();						 
					    HAL_SPI_TransmitReceive(&hspi2,Uart1_Receive.UART1_Receive_data+5,
						                          SPI_receive_data,SPI_data_len, 1000);
						  SPI_CS_Disable();
						  //printf("5AA5010302%d",SPI_75602_Receive);
						 SPI_Return_Data[0] = 0x5A;
						 SPI_Return_Data[1] = 0xA5;
						 SPI_Return_Data[2] = 0x01;
						 SPI_Return_Data[3] = SPI_data_len+1;
						 SPI_Return_Data[4] = 0x02;
						 SPI_Return_Data[5] = SPI_receive_data[0];
						 SPI_Return_Data[6] = SPI_receive_data[1];
						 
						 USART1_IdleCallback(SPI_Return_Data,5+SPI_data_len);
							
							}
						
              						 
					 }	

           break;
					 
					    default:
					//printf("输入指令错误，请重新输入\r\n");
				  break;					 
				 }
			 }
			 else if((Uart1_Receive.UART1_Receive_data[0]==0x5a)&&             //GPIO控制
				 (Uart1_Receive.UART1_Receive_data[1]==0xa5)&&
			   (Uart1_Receive.UART1_Receive_data[2]==0x02))
			   {
			      switch (Uart1_Receive.UART1_Receive_data[4] )
				          {
										case 1 : 			
					            if(Uart1_Receive.UART1_Receive_data[5] == 0xff)
											{
										    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
												USART1_IdleCallback(Uart1_Receive.UART1_Receive_data,
												                    Uart1_Receive.UART1_Receive_data_len);
											}else if(Uart1_Receive.UART1_Receive_data[5] == 0x01)
											{
											  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
												USART1_IdleCallback(Uart1_Receive.UART1_Receive_data,
												                    Uart1_Receive.UART1_Receive_data_len);
											}else{};
						          
											break;
					 
										case 2 : 
											 if(Uart1_Receive.UART1_Receive_data[5] == 0xff)
											{
										    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
												USART1_IdleCallback(Uart1_Receive.UART1_Receive_data,
												                    Uart1_Receive.UART1_Receive_data_len); 
											}else if(Uart1_Receive.UART1_Receive_data[5] == 0x01)
											{
											  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
												USART1_IdleCallback(Uart1_Receive.UART1_Receive_data,
												                    Uart1_Receive.UART1_Receive_data_len); 
											}else{};
											break;
										case 3:
											 
					             if(Uart1_Receive.UART1_Receive_data[5] == 0xff)
											{
										    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
												USART1_IdleCallback(Uart1_Receive.UART1_Receive_data,
												                    Uart1_Receive.UART1_Receive_data_len); 
											}else if(Uart1_Receive.UART1_Receive_data[5] == 0x01)
											{
											  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
												USART1_IdleCallback(Uart1_Receive.UART1_Receive_data,
												                    Uart1_Receive.UART1_Receive_data_len); 
											}else{};
											break;
										case 4:
											 
					             if(Uart1_Receive.UART1_Receive_data[5] == 0xff)
											{
										    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
												USART1_IdleCallback(Uart1_Receive.UART1_Receive_data,
												                    Uart1_Receive.UART1_Receive_data_len); 
											}else if(Uart1_Receive.UART1_Receive_data[5] == 0x01)
											{
											  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
												USART1_IdleCallback(Uart1_Receive.UART1_Receive_data,
												                    Uart1_Receive.UART1_Receive_data_len);
											}else{};
											break;
												
												
										default:
					           printf(".........\n\r");
				             USART1_IdleCallback(Uart1_Receive.UART1_Receive_data,
												                    Uart1_Receive.UART1_Receive_data_len);
				             printf(".........\n\r");
										break;		
									}			 
			  
			   }
				 else if((Uart1_Receive.UART1_Receive_data[0]==0x5a)&&             //电压回传
				 (Uart1_Receive.UART1_Receive_data[1]==0xa5)&&
			   (Uart1_Receive.UART1_Receive_data[2]==0x04))
				 {		    
				     Vout = INA226_Read2Byte(Bus_V_Reg);
					   SPI_Return_Data[0] = 0x5A;
						 SPI_Return_Data[1] = 0xA5;
						 SPI_Return_Data[2] = 0x04;
						 SPI_Return_Data[3] = 0x03;
						 SPI_Return_Data[4] = 0x06;
						 SPI_Return_Data[5] = (Vout >> 8)& 0xFF;
						 SPI_Return_Data[6] = 0xFF & Vout; 
					   USART1_IdleCallback(SPI_Return_Data,7);
				 } else{
					           printf(".........\n\r");
				             USART1_IdleCallback(Uart1_Receive.UART1_Receive_data,
												                    Uart1_Receive.UART1_Receive_data_len);
				             printf(".........\n\r");
				       };
					            Left_Move_Array(RxBuffer,RXBUFFERSIZE,Uart1_Receive.UART1_Receive_data_len);
                     // memset(RxBuffer,0x00,sizeof(RxBuffer));					 
			 }else{};
			 
		 }
