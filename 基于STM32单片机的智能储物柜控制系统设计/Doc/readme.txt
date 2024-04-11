


/*
	上电之后对Flash内部进行扫描，扫描是否有之前存储的数据，并更新变量unavailable（不可用数量），其次对舵机复位，
	主菜单中的使用定时器下方每5秒刷新unavailable与available，便于直观查看可用数量
	蓝牙串口软件 发送格式为：
		1、发送手机号接收验证码（Hex格式）
			每两位十进制数字占一个字节，所以手机号一共占6个字节
			例如：13 50 23 78 82 40		这里的手机号一共有6个字节，在这个软件中，最后一个十进制为默认是一个字节中的高4位
			发送 13 50 23 78 82 40		匹配成功返回 09 03 02 01 00（只是举例，实际上验证码有TIM2的计数值作为随机数的种子，重复的可能极低）
										匹配失败返回 FF FF FF FF FF (验证码是5个字节，返回成功的每一个十进制数占一个字节的低4位)
		2、发送验证码驱动舵机（Hex格式） 接收格式为 UTF-8 OR GBK
		发送验证码有固定的包头格式  包头格式为FF 同样后面也是每个字节一个十进制数（低4位）
		例如发送： FF 09 03 02 01 00 	匹配成功返回 “Pickup successful!\r\n”（注意要用UTF-8或GBK的编码格式接收数据，否则会乱码）
		



*/








/****************************** 以下为托管代码 *************************************/
/* 代码例程 */
	/* 标准库配置 PA0 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
	
	
	/* 位带操作 */
	int main(void){
		RCC->APB2ENR = 0x04;
		GPIOA->CRL = 0x03;
		OLED_Init();

		while(1){
			// PAout(0) = 1;
			// WritePin(A,0) = 1;
			TogglePin(A,0);
			OLED_ShowNum(1,1,ReadPin(A,0),4);
			Delay_ms(250);
			
			// PAout(0) = 0;
			// WritePin(A,0) = 1;
			TogglePin(A,0);
			OLED_ShowNum(1,1,ReadPin(A,0),4);
			Delay_ms(250);
		}
	}
	
	/* 配置寄存器 闪烁PC13 */
	int main(void){

		RCC->APB2ENR = 0x10;
		GPIOC->CRH = 0x00300000;
		/* GPIOC->ODR = 0X2000; */

		while(1){
			GPIOC->BSRR = 0X2000;
			Delay_ms(200);
			
			GPIOC->BRR = 0x2000;
			Delay_ms(200);
		}
	}
	
	// LCD_ShowNum(1,1,a+99,9,20);
 	// LCD_ShowNum(1,21,1238964,9,20);	
	//	LCD_ShowChar(1,80,BLUE,YELLOW,'m',24,0);
	//	LCD_ShowString(1,90,16,"Ïã¸Û",0);
	
	
	
	/*--------------------------------------------------------------------------------------*/
			/* test SG90 */
//		SG90_Start_Zeroing_test_180();
//		delay_ms(1500);
//		SG90_Start_Zeroing_Init();
//		delay_ms(1500);
	
	
	/*	以上为托管代码	*/