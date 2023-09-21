#include "stm32f10x.h" // Device header
#include "key.h"	//按键模块头文件
#include "led.h"	//LED模块头文件
#include "Timer.h"	//定时器模块头文件
#include "Delay.h"	//延时模块头文件



uint8_t LED_State;//表示LED状态，取值0、1、2分别代表三个不同的状态，状态详情见函数LED_ShowFlow()
uint8_t TIM_Count;//定时器中断次数计数器，记到10时自身清零，LED_State++,LED状态改变，从而做到周期改变状态





//定时器重计数函数
void TIM_Restart(void)
{
	TIM_SetCounter(TIM1, 0);//TIM重新计数
	TIM_Count = 0;			//TIM_Count重新计数
}






int main(void)
{
	//初始化LED、按键、定时器
	LED_Init();
	KEY_Init();
	Timer_Init();

	
	//主循环
	while (1)
	{
		if( KEY0 == 0 ){							//捕捉到按键按下，KEY0是头文件key.h中的宏定义，代表按键端口的输入电平
			
			Delay_ms(20);						//消抖
			while( KEY0 == 0 )	TIM_Count = 0;	//长按按键时，定时器中断次数一直清零，不改变LED状态
			Delay_ms(20);						//消抖
			
			if( LED_State++ >= 2) LED_State = 0;	//按键按下并松开，LED状态改变，如果自身大于2则清零，且通过下面一个函数重新用定时器记数
			
			TIM_Restart();//按键触发LED状态改变，定时器重新记数
			
		}
	}
	
	
}



//LED显示函数
void LED_ShowFlow(void)
{
		switch (LED_State){
			
		case 0:{ 			//状态一：LED0亮，LED1灭
			LED0_ON();		
			LED1_OFF(); 
			}break;
			
		case 1:{ 			//状态二：LED0灭，LED1亮
			LED0_OFF();	
			LED1_ON(); 
			}break;
		
		case 2:{ 			//状态三：LED0、LED1翻转状态。
			LED0_Turn();	
			LED1_Turn();
			}break;
		
		default :{			//默认状态：LED0、LED1熄灭
			LED0_OFF();	
			LED1_OFF(); 
			}
		}
}


//定时器中断执行函数

/*
	定时器设置每0.5秒触发一次中断
	在没有按键按下的情况下：
	每次中断执行LED_ShowFlow()，且TIM_Count++
	每当TIM_Count从0记到10（需要5秒），也就是执行了10次LED_ShowFlow()，代表LED维持在当前状态持续了5秒
	此时TIM_Count清零，LED_State++,LED显示状态改变
*/
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)//检测标志位
	{
		
		
		if( TIM_Count++ >=10 )		
		{
			TIM_Count = 0;
			if( LED_State ++ >= 2 )
				LED_State = 0;
		}
		
		LED_ShowFlow();
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//清除标志位
		
	}
}
