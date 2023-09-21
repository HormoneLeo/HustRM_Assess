#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "key.h"
#include "Timer.h"
#include "Serial.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//初始化
	Timer_Init();
	OLED_Init();
	KEY_Init();
	Serial_Init();
	
	
	while (1)
	{
	
	}
}
//中断执行函数写在main()里
