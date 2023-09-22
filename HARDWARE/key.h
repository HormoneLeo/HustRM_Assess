#ifndef __KEY_H
#define __KEY_H
#include "stm32f10x.h"                  // Device header

typedef enum{
	KEY_UNPRESSED = 0,
	KEY_PRESSED
}KeyStateEnum;



typedef struct{
	//������Ϣ
	FlagStatus longPressed;
	FlagStatus shortPressed;
	FlagStatus doublePressed;
	FlagStatus doublePressed_JudgeFlag;
	//����״̬
	KeyStateEnum Key_State;
	
	//��������
	unsigned int Key_holdon_ms;
	unsigned int Key_press_gap_ms;
}myKey_ValueTypeDef;


extern myKey_ValueTypeDef myKey_Value;

void KEY_Init(void) ;
void keyValueReset(void);


void TIM_CLock_Count(void);
void TIM_SetClock(uint32_t TIM_ClockStartPoint);
uint32_t TIM_GetClock(void);

#endif
