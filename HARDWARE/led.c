#include "stm32f10x.h"                  // Device header

//LED³õÊ¼»¯º¯Êý
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1);
}

//LED0µãÁÁ
void LED0_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}
//LED0Ï¨Ãð
void LED0_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
}
//LED0·­×ª×´Ì¬
void LED0_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_0);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	}
}
//LED1µãÁÁ
void LED1_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}
//LED1Ï¨Ãð
void LED1_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}
//LED1·­×ª×´Ì¬
void LED1_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_1);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	}
}

