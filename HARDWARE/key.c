#include "stm32f10x.h"                  // Device header
#include "key.h"
#include "Delay.h"




void KEY_Init(void)
{
	//时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//构建结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	//结构体成员设置
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_IPU;//上拉输入模式
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_1;
	
	//初始化
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}




