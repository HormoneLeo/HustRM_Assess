#include "stm32f10x.h"                  // Device header
#include "led.h"

//��ʼ������
void LED_Init(void)
{
	//ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//�����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	//�ṹ���Ա����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	
	//��ʼ��
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//Ϩ��
	GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1);
}


//LED0��ת����
void LED0_Turn(void)
{
	if (LED0_State == 0 )
		GPIO_SetBits(GPIOA, GPIO_Pin_0 );
	else 
		GPIO_ResetBits(GPIOA, GPIO_Pin_0 );
}

//LED1��ת����
void LED1_Turn(void)
{
	if (LED1_State == 0 )
		GPIO_SetBits(GPIOA, GPIO_Pin_1 );
	else 
		GPIO_ResetBits(GPIOA, GPIO_Pin_1 );
}