#ifndef __KEY_H
#define __KEY_H
#include "stm32f10x.h"                  // Device header



//用KEY来表示按键的物理状态
#define KEY GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)

//按键状态枚举：按下、抬起
typedef enum{
	KEY_UNPRESSED = 0,
	KEY_PRESSED
}KeyStateEnum;

//按键触发状态结构体
typedef struct{
	//按键消息
	FlagStatus longPressed;				//长按标志位
	FlagStatus shortPressed;			//单击标志位
	FlagStatus doublePressed;			//双击标志位
	FlagStatus doublePressed_JudgeFlag;	//双击检测标志位（用于单击之后检测双击）
	//按键状态
	KeyStateEnum Key_State;				//自定义枚举
	
	//按键触发
	unsigned int Key_holdon_ms;			//1kHz检测长按计数器
	unsigned int Key_press_gap_ms;		//单击间隔计数器，用于区分单击和双击
}myKey_ValueTypeDef;


extern myKey_ValueTypeDef myKey_Value;	//用于主函数使用

void KEY_Init(void) ;					//初始化函数
void keyValueReset(void);				//按键状态标志全复位函数


void TIM_CLock_Count(void);				//时钟计数器（用于设置设备时间）
void TIM_SetClock(uint32_t TIM_ClockStartPoint);//时钟起始电设置（用于设置起始时间）
uint32_t TIM_GetClock(void);			//获取时钟计数器的值，用于计算当前时间

#endif
