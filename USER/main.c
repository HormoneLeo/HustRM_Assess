#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "key.h"
#include "Timer.h"
#include "Serial.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//≥ı ºªØ
	Timer_Init();
	OLED_Init();
	KEY_Init();
	Serial_Init();
	
	printf("Press and hold the key to set the current time of the system\r\n");
	while(myKey_Value.longPressed == RESET);
	printf("Send the message like Set_Time:20230921 19:30:21\r\n");

	while (1)
	{
	
	}
}

