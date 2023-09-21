#include "stm32f10x.h"                  // Device header
#include "key.h"


#define KEY GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)


myKey_ValueTypeDef myKey_Value;



void KEY_Init(void)
{
	//时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	

		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_IPU;//上拉输入模式
		GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
	
		EXTI_InitTypeDef EXTI_InitStructure;
		EXTI_InitStructure.EXTI_Line=EXTI_Line1;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); //④初始化中断线参数

		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn; //使能按键外部中断通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级 2，
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //子优先级 2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//初始化 NVIC

}

void keyValueReset(void)
{
	myKey_Value.shortPressed = RESET;
	myKey_Value.longPressed  = RESET;
	myKey_Value.doublePressed  = RESET;
	myKey_Value.doublePressed_JudgeFlag = RESET;
	myKey_Value.Key_holdon_ms = 0;
	myKey_Value.Key_press_gap_ms = 0;
	myKey_Value.Key_State = KEY_UNPRESSED;
}

void EXTI1_IRQHandler(void)
{
    if(KEY == 0) 
    {
		
        myKey_Value.Key_State = KEY_PRESSED;//生成按键按下标志
    }
    EXTI_ClearITPendingBit(EXTI_Line1); //清除 LINE1 上的中断标志位
}



void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		if(myKey_Value.Key_State == KEY_PRESSED)
		{
			if(KEY == 0)
			{
				if(myKey_Value.Key_holdon_ms < 1000)
				{
					myKey_Value.Key_holdon_ms ++;
				}else{
					myKey_Value.Key_holdon_ms = 0;
					myKey_Value.shortPressed  = RESET;
					myKey_Value.longPressed   = SET;
					myKey_Value.Key_State     =KEY_UNPRESSED;
					myKey_Value.doublePressed_JudgeFlag = SET;
				}
			}else{
				if(myKey_Value.Key_holdon_ms > 50)
				{
					myKey_Value.Key_holdon_ms = 0;
					//myKey_Value.shortPressed  = SET;
					myKey_Value.longPressed   = RESET;
					myKey_Value.Key_State     = KEY_UNPRESSED;
					
					myKey_Value.doublePressed_JudgeFlag = SET;
					
					if(myKey_Value.Key_press_gap_ms >100 && myKey_Value.Key_press_gap_ms <= 300)
					{
						myKey_Value.doublePressed = SET;
						myKey_Value.shortPressed  = RESET;
					}
				}else{
					myKey_Value.Key_holdon_ms = 0;
					myKey_Value.shortPressed  = RESET;
					myKey_Value.longPressed   = RESET;
					myKey_Value.Key_State     = KEY_UNPRESSED;
				}					
			}
		}
		
		if(myKey_Value.doublePressed_JudgeFlag == SET)
			myKey_Value.Key_press_gap_ms ++;
		
		if(myKey_Value.Key_press_gap_ms > 300 && myKey_Value.doublePressed == RESET && myKey_Value.longPressed  == RESET)
			myKey_Value.shortPressed  = SET;
		
		if(myKey_Value.Key_press_gap_ms > 500)
		{
			myKey_Value.Key_press_gap_ms = 0;
			myKey_Value.doublePressed_JudgeFlag = RESET;
			keyValueReset();
		}
		
	}
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}


