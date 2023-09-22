#ifndef __KEY_H
#define __KEY_H
#include "stm32f10x.h"                  // Device header



//��KEY����ʾ����������״̬
#define KEY GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)

//����״̬ö�٣����¡�̧��
typedef enum{
	KEY_UNPRESSED = 0,
	KEY_PRESSED
}KeyStateEnum;

//��������״̬�ṹ��
typedef struct{
	//������Ϣ
	FlagStatus longPressed;				//������־λ
	FlagStatus shortPressed;			//������־λ
	FlagStatus doublePressed;			//˫����־λ
	FlagStatus doublePressed_JudgeFlag;	//˫������־λ�����ڵ���֮����˫����
	//����״̬
	KeyStateEnum Key_State;				//�Զ���ö��
	
	//��������
	unsigned int Key_holdon_ms;			//1kHz��ⳤ��������
	unsigned int Key_press_gap_ms;		//����������������������ֵ�����˫��
}myKey_ValueTypeDef;


extern myKey_ValueTypeDef myKey_Value;	//����������ʹ��

void KEY_Init(void) ;					//��ʼ������
void keyValueReset(void);				//����״̬��־ȫ��λ����


void TIM_CLock_Count(void);				//ʱ�Ӽ����������������豸ʱ�䣩
void TIM_SetClock(uint32_t TIM_ClockStartPoint);//ʱ����ʼ�����ã�����������ʼʱ�䣩
uint32_t TIM_GetClock(void);			//��ȡʱ�Ӽ�������ֵ�����ڼ��㵱ǰʱ��

#endif
