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
static char cmd_SetTime[]="SETTIME";//����ʱ���ָ��

void DeviceStart(void)
{
	printf("�볤���������豸\r\n\r\n");
	while(myKey_Value.longPressed == RESET);
	printf("�豸�򿪳ɹ�������SETTIME�������õ�ǰϵͳʱ��,��ʽ����:\r\nSETTIME 19:30:21 2023/09/21\r\n\r\n");
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
		printf("����ϵͳʱ��ɹ�!��ǰʱ��Ϊ:\r\n%d:%d:%d %d/%d/%d\r\n\r\n",Hour,Minute,Second,year,month,day);


	    }
	}
}

}
int main(void)
{
	//�����жϷ���
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//��ʼ��
	Timer_Init();
	OLED_Init();
	KEY_Init();
	Serial_Init();
	
	//�豸��������
	DeviceStart();
	//ϵͳʱ�����ó���
	TimeSet();
	while (1)
	{
	
	}
}

