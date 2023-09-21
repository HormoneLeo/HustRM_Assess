#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "key.h"
#include "Timer.h"
#include "Serial.h"
#include "string.h"


char strbuf[50];
unsigned char HH,MM,SS;
static char cmd_SetTime[]="AT+SETTIME";//����ʱ���ָ��



void TimeSet()
{
	while(1)
    {
	if(USART_GetString(strbuf)!=NULL){
	    if(strncmp(strbuf,cmd_SetTime,strlen(cmd_SetTime))==0){
		HH = (strbuf[strlen(cmd_SetTime)+1]-'0')*10 +(strbuf[strlen(cmd_SetTime)+2]-'0');
		MM = (strbuf[strlen(cmd_SetTime)+4]-'0')*10 +(strbuf[strlen(cmd_SetTime)+5]-'0');
		SS = (strbuf[strlen(cmd_SetTime)+7]-'0')*10 +(strbuf[strlen(cmd_SetTime)+8]-'0');
		printf("����RTCʱ��Ϊ %d:%d:%d\r\n",HH,MM,SS);
		RTC_WaitForLastTask();
		RTC_SetCounter(HH*3600+MM*60+SS);//����RTC��ǰ�����Ĵ�����ֵ
		RTC_WaitForLastTask();
		printf("����RTCʱ��ɹ�!\r\n");
	    }
	}
}

}
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//��ʼ��
	Timer_Init();
	OLED_Init();
	KEY_Init();
	Serial_Init();
	TimeSet();
	
//	printf("Press and hold the key to set the current time of the system\r\n");
//	while(myKey_Value.longPressed == RESET);
//	printf("Send the message like Set_Time:20230921 19:30:21\r\n");

	while (1)
	{
	
	}
}

