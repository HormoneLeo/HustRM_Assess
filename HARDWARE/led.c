#include "stm32f10x.h"                  // Device header
#include "led.h"

//初始化函数
void LED_Init(void)
{
	//时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//构建结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	//结构体成员设置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	
	//初始化
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//熄灭
	GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1);
}


//LED0翻转函数
void LED0_Turn(void)
{
	if (LED0_State == 0 )
		GPIO_SetBits(GPIOA, GPIO_Pin_0 );
	else 
		GPIO_ResetBits(GPIOA, GPIO_Pin_0 );
}

//LED1翻转函数
void LED1_Turn(void)
{
	if (LED1_State == 0 )
		GPIO_SetBits(GPIOA, GPIO_Pin_1 );
	else 
		GPIO_ResetBits(GPIOA, GPIO_Pin_1 );
}