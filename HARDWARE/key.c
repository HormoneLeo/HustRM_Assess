#include "stm32f10x.h"                  // Device header
#include "key.h"
#include "Delay.h"




void KEY_Init(void)
{
	//ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//�����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	//�ṹ���Ա����
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_IPU;//��������ģʽ
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_1;
	
	//��ʼ��
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}




