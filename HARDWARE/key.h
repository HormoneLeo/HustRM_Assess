#ifndef __KEY_H
#define __KEY_H
#include "stm32f10x.h"                  // Device header

typedef enum{
	KEY_UNPRESSED = 0,
	KEY_PRESSED
}KeyStateEnum;



typedef struct{
	//按键消息
	FlagStatus longPressed;
	FlagStatus shortPressed;
	FlagStatus doublePressed;
	FlagStatus doublePressed_JudgeFlag;
	//按键状态
	KeyStateEnum Key_State;
	
	//按键触发
	unsigned int Key_holdon_ms;
	unsigned int Key_press_gap_ms;
}myKey_ValueTypeDef;


extern myKey_ValueTypeDef myKey_Value;

void KEY_Init(void) ;
void keyValueReset(void);

#endif
