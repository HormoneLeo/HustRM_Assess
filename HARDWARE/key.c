#include "stm32f10x.h"                  // Device header
#include "key.h"


#define KEY GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)


myKey_ValueTypeDef myKey_Value;



void KEY_Init(void)
{
	//时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	//GPIO配置部分
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_IPU;//上拉输入模式
		GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//GPIO复用为外部中断
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
	
		EXTI_InitTypeDef EXTI_InitStructure;
		EXTI_InitStructure.EXTI_Line=EXTI_Line1;//选择外部中断1号线
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//选择中断触发
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能
	EXTI_Init(&EXTI_InitStructure); 

		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn; //使能按键外部中断通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级 2，
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //子优先级 2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//初始化 NVIC

}


//按键值结构体复位函数
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




//外部中断处理函数
void EXTI1_IRQHandler(void)
{
    if(KEY == 0) 
    {
		
        myKey_Value.Key_State = KEY_PRESSED;//生成按键按下标志
    }
    EXTI_ClearITPendingBit(EXTI_Line1); //清除 LINE1 上的中断标志位
}


//TIM2触发频率为1KHz,也就是每1ms对按键状态采样一次
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)		//读取中断标志位
	{
		if(myKey_Value.Key_State == KEY_PRESSED)		//确认按键被按下
		{
			if(KEY == 0)	//长按识别
			{
				if(myKey_Value.Key_holdon_ms < 1000)	//规定长按为1000ms以上
				{
					myKey_Value.Key_holdon_ms ++;
				}else{
					myKey_Value.Key_holdon_ms = 0;		//长按计数器清零
					myKey_Value.shortPressed  = RESET;	//单击标志复位
					myKey_Value.longPressed   = SET;	//长按标志置位
					myKey_Value.Key_State     =KEY_UNPRESSED;	//按键视为已经抬起，等待下一次外部中断
					myKey_Value.doublePressed_JudgeFlag = SET;	//同时开启抬起时间计数器标志
				}
			}else{										//按下按键但不是长按
				if(myKey_Value.Key_holdon_ms > 50)		//忽略低于50ms的按键抖动
				{
					myKey_Value.Key_holdon_ms = 0;
					//myKey_Value.shortPressed  = SET;	//此处暂时不认定为单击，而是等待判断是否为双击之后
					myKey_Value.longPressed   = RESET;
					myKey_Value.Key_State     = KEY_UNPRESSED;//视为按键已经抬起，等待下一次外部中断
					
					myKey_Value.doublePressed_JudgeFlag = SET;//开启抬起时间计数器标志
					
					if(myKey_Value.Key_press_gap_ms >100 && myKey_Value.Key_press_gap_ms <= 300)//如果距离上一次抬起时间在100-300ms之间，则视为双击
					{
						myKey_Value.doublePressed = SET;		//双击标志置位
						myKey_Value.shortPressed  = RESET;		//单击标志复位
					}
				}else{											//低于50ms的低电平视为无效抖动
					myKey_Value.Key_holdon_ms = 0;				
					myKey_Value.shortPressed  = RESET;
					myKey_Value.longPressed   = RESET;
					myKey_Value.Key_State     = KEY_UNPRESSED;
				}					
			}
		}
		
		if(myKey_Value.doublePressed_JudgeFlag == SET)			//由抬起时间计数器标志决定计数器的计数
			myKey_Value.Key_press_gap_ms ++;
		
		if(myKey_Value.Key_press_gap_ms > 300 && myKey_Value.doublePressed == RESET && myKey_Value.longPressed  == RESET)	//识别到一次单击300ms以后认为是单击，单击标志位置位
			myKey_Value.shortPressed  = SET;
		
		if(myKey_Value.Key_press_gap_ms > 500)					//每500ms按键状态自动复位
		{
			myKey_Value.Key_press_gap_ms = 0;
			myKey_Value.doublePressed_JudgeFlag = RESET;
			keyValueReset();
		}
		
	}
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//清除中断标志位
}


