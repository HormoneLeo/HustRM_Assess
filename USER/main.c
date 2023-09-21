#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "key.h"
#include "Timer.h"
#include "Serial.h"
#include "string.h"


char strbuf[50];
unsigned char Hour,Minute,Second;
uint16_t year,month,day;
static char cmd_SetTime[]="SETTIME";//设置时间的指令

void DeviceStart(void)
{
	printf("请长按按键打开设备\r\n\r\n");
	while(myKey_Value.longPressed == RESET);
	printf("设备打开成功，请用SETTIME命令设置当前系统时间,格式如下:\r\nSETTIME 19:30:21 2023/09/21\r\n\r\n");
}

void TimeSet(void)
{
	while(1)
    {
	if(USART_GetString(strbuf)!=NULL)
		{
	    if(strncmp(strbuf,cmd_SetTime,strlen(cmd_SetTime))==0){
		Hour = (strbuf[strlen(cmd_SetTime)+1]-'0')*10 +(strbuf[strlen(cmd_SetTime)+2]-'0');
		Minute = (strbuf[strlen(cmd_SetTime)+4]-'0')*10 +(strbuf[strlen(cmd_SetTime)+5]-'0');
		Second = (strbuf[strlen(cmd_SetTime)+7]-'0')*10 +(strbuf[strlen(cmd_SetTime)+8]-'0');
		year = (strbuf[strlen(cmd_SetTime)+10]-'0')*1000 +(strbuf[strlen(cmd_SetTime)+11]-'0')*100 + (strbuf[strlen(cmd_SetTime)+12]-'0')*10 + (strbuf[strlen(cmd_SetTime)+13]-'0');
		month = (strbuf[strlen(cmd_SetTime)+15]-'0')*10 +(strbuf[strlen(cmd_SetTime)+16]-'0');
		day = (strbuf[strlen(cmd_SetTime)+18]-'0')*10 +(strbuf[strlen(cmd_SetTime)+19]-'0');
		printf("设置系统时间成功!当前时间为:\r\n%d:%d:%d %d/%d/%d\r\n\r\n",Hour,Minute,Second,year,month,day);


	    }
	}
}

}
int main(void)
{
	//设置中断分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//初始化
	Timer_Init();
	OLED_Init();
	KEY_Init();
	Serial_Init();
	
	//设备启动程序
	DeviceStart();
	//系统时间设置程序
	TimeSet();
	while (1)
	{
	
	}
}

