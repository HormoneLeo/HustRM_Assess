#include "stm32f10x.h" // Device header
#include "key.h"	//����ģ��ͷ�ļ�
#include "led.h"	//LEDģ��ͷ�ļ�
#include "Timer.h"	//��ʱ��ģ��ͷ�ļ�
#include "Delay.h"	//��ʱģ��ͷ�ļ�



uint8_t LED_State;//��ʾLED״̬��ȡֵ0��1��2�ֱ����������ͬ��״̬��״̬���������LED_ShowFlow()
uint8_t TIM_Count;//��ʱ���жϴ������������ǵ�10ʱ�������㣬LED_State++,LED״̬�ı䣬�Ӷ��������ڸı�״̬





//��ʱ���ؼ�������
void TIM_Restart(void)
{
	TIM_SetCounter(TIM1, 0);//TIM���¼���
	TIM_Count = 0;			//TIM_Count���¼���
}






int main(void)
{
	//��ʼ��LED����������ʱ��
	LED_Init();
	KEY_Init();
	Timer_Init();

	
	//��ѭ��
	while (1)
	{
		if( KEY0 == 0 ){							//��׽���������£�KEY0��ͷ�ļ�key.h�еĺ궨�壬�������˿ڵ������ƽ
			
			Delay_ms(20);						//����
			while( KEY0 == 0 )	TIM_Count = 0;	//��������ʱ����ʱ���жϴ���һֱ���㣬���ı�LED״̬
			Delay_ms(20);						//����
			
			if( LED_State++ >= 2) LED_State = 0;	//�������²��ɿ���LED״̬�ı䣬����������2�����㣬��ͨ������һ�����������ö�ʱ������
			
			TIM_Restart();//��������LED״̬�ı䣬��ʱ�����¼���
			
		}
	}
	
	
}



//LED��ʾ����
void LED_ShowFlow(void)
{
		switch (LED_State){
			
		case 0:{ 			//״̬һ��LED0����LED1��
			LED0_ON();		
			LED1_OFF(); 
			}break;
			
		case 1:{ 			//״̬����LED0��LED1��
			LED0_OFF();	
			LED1_ON(); 
			}break;
		
		case 2:{ 			//״̬����LED0��LED1��ת״̬��
			LED0_Turn();	
			LED1_Turn();
			}break;
		
		default :{			//Ĭ��״̬��LED0��LED1Ϩ��
			LED0_OFF();	
			LED1_OFF(); 
			}
		}
}


//��ʱ���ж�ִ�к���

/*
	��ʱ������ÿ0.5�봥��һ���ж�
	��û�а������µ�����£�
	ÿ���ж�ִ��LED_ShowFlow()����TIM_Count++
	ÿ��TIM_Count��0�ǵ�10����Ҫ5�룩��Ҳ����ִ����10��LED_ShowFlow()������LEDά���ڵ�ǰ״̬������5��
	��ʱTIM_Count���㣬LED_State++,LED��ʾ״̬�ı�
*/
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)//����־λ
	{
		
		
		if( TIM_Count++ >=10 )		
		{
			TIM_Count = 0;
			if( LED_State ++ >= 2 )
				LED_State = 0;
		}
		
		LED_ShowFlow();
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//�����־λ
		
	}
}
