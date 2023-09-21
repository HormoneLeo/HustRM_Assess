#include "stm32f10x.h"                  // Device header



void Timer_Init(void)
{
	//开启TIM时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//配置TIM时钟源
	TIM_InternalClockConfig(TIM2);
	
	//TIM初始化：结构体配置时基单元，主频72MHz，定时器更新频率：1 / （100*720） = 1 kHz
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
		TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInitStructure.TIM_Period = 100-1;		//重载值
		TIM_TimeBaseInitStructure.TIM_Prescaler = 720-1;	//预分频
		TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 5;//高级定时器有效
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	//清除初始化产生的标志位
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	
	//使能中断控制
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	//初始化NVIC：设置为更新中断
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_Init(&NVIC_InitStructure);
	
	//使能定时器
	TIM_Cmd(TIM2, ENABLE);
	
}



//中断执行函数写在key.c文件里
//void TIM2_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
//	{
//		
//		
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//		
//	}
//}






