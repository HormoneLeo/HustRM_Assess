#include "stm32f10x.h"                  // Device header
#include "key.h"


#define KEY GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)


myKey_ValueTypeDef myKey_Value;



void KEY_Init(void)
{
	//ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	

		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_IPU;//��������ģʽ
		GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
	
		EXTI_InitTypeDef EXTI_InitStructure;
		EXTI_InitStructure.EXTI_Line=EXTI_Line1;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); //�ܳ�ʼ���ж��߲���

		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn; //ʹ�ܰ����ⲿ�ж�ͨ��
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //��ռ���ȼ� 2��
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //�����ȼ� 2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//��ʼ�� NVIC

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
		
        myKey_Value.Key_State = KEY_PRESSED;//���ɰ������±�־
    }
    EXTI_ClearITPendingBit(EXTI_Line1); //��� LINE1 �ϵ��жϱ�־λ
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


