#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "key.h"
#include "led.h"
#include "Timer.h"
#include "Serial.h"
#include "string.h"

struct loginTime{
	uint8_t Hour,Min,Sec;
	uint16_t Year,Month,Day;
};



struct staff{
	unsigned char user_name[20];
	unsigned char password[20];
	struct loginTime login_TimeSave[20];
	uint8_t loginNum;
}staffs_Database[50];



uint8_t staffNum;
char strbuf[50];
uint16_t year,month,day;
static char cmd_SetTime[]="SETTIME";//设置时间的指令
static char cmd_StaffLogin[]="user:";
static char cmd_StaffSignup[]="user:,password:";

static char cmd_Exit[]="exit";




//外设模块初始化
void Project_InitAll()
{
	Timer_Init();
	OLED_Init();
	KEY_Init();
	LED_Init();
	Serial_Init();
}




//设备上电
void DeviceStart(void)
{
	printf("请长按按键打开设备\r\n\r\n");
	while(myKey_Value.longPressed == RESET);
	printf("设备打开成功，请用SETTIME命令设置当前系统时间,格式如下:\r\nSETTIME 19:30:21 2023/09/21\r\n\r\n");
}




//设置系统时间
void TimeSet(void)
{
	OLED_ShowString(1,1,"Current Time:");
	OLED_ShowString(2,1,"  00:00:00  ");
	unsigned char Hour,Minute,Second;
	while(1)
    {
	if(USART_GetString(strbuf)!=NULL)
		{
	    if(strncmp(strbuf,cmd_SetTime,strlen(cmd_SetTime))==0)
			{
				Hour = (strbuf[strlen(cmd_SetTime)+1]-'0')*10 +(strbuf[strlen(cmd_SetTime)+2]-'0');
				Minute = (strbuf[strlen(cmd_SetTime)+4]-'0')*10 +(strbuf[strlen(cmd_SetTime)+5]-'0');
				Second = (strbuf[strlen(cmd_SetTime)+7]-'0')*10 +(strbuf[strlen(cmd_SetTime)+8]-'0');
				year = (strbuf[strlen(cmd_SetTime)+10]-'0')*1000 +(strbuf[strlen(cmd_SetTime)+11]-'0')*100 + (strbuf[strlen(cmd_SetTime)+12]-'0')*10 + (strbuf[strlen(cmd_SetTime)+13]-'0');
				month = (strbuf[strlen(cmd_SetTime)+15]-'0')*10 +(strbuf[strlen(cmd_SetTime)+16]-'0');
				day = (strbuf[strlen(cmd_SetTime)+18]-'0')*10 +(strbuf[strlen(cmd_SetTime)+19]-'0');
				printf("设置系统时间成功!当前时间为:\r\n%02d:%02d:%02d %04d/%02d/%02d\r\n\r\n",Hour,Minute,Second,year,month,day);
				TIM_SetClock( (uint32_t) (Hour * 3600 + Minute * 60 + Second) );
				memset(strbuf,0,sizeof strbuf);
				break;
			}
		}
		
	}
}
	
void Print_MainMeun(uint8_t MainMeun_Option)
{
	printf("\r\nloading......\r\n\r\n");
	switch(MainMeun_Option){
		case 0:{
			printf("error commit!Send the right message again, please.\r\n");
		}break;
		case 1:{
			printf("**************Main Meun*************\r\n");
			printf("   --------signup:长按按键--------   \r\n");
			printf("   --------login :单击按键--------   \r\n");
			printf("   --------lookup:双击按键--------   \r\n");
			printf("*************************************\r\n");
		}break;
		case 2:{
			printf("*************login Meun**************\r\n");
			printf("   ------------user:***-----------   \r\n");
			printf("   --------------exit-------------   \r\n");
			printf("*************************************\r\n");
		}break;
		case 3:{
			printf("*************signup Meun**************\r\n");
			printf("   ----user:***,password:******----   \r\n");
			printf("   --------------exit-------------   \r\n");
			printf("*************************************\r\n");
		}break;
		case 4:{
			printf("*************lookup Meun**************\r\n");
			printf("   ------------user:***-----------   \r\n");
			printf("   --------------exit-------------   \r\n");
			printf("*************************************\r\n");
		}break;

		
	}
}

uint8_t KEY_Scan(void)
{
	while(1)
	{
		if(myKey_Value.shortPressed == SET)
			return 0;
		else if(myKey_Value.doublePressed == SET)
			return 1;
		else if(myKey_Value.longPressed == SET)
			return 2;
	}
}

void staffSignupNew()
{
	printf("\r\nwait for a second...\r\n\r\n");
	uint8_t i,j;
	for(i=0; strbuf[i + strlen(cmd_StaffLogin)] != ','; i++)
	{
		staffs_Database[staffNum].user_name[i] = strbuf[i + strlen(cmd_StaffLogin)];
	}
	staffs_Database[staffNum].user_name[i] = 0;


	for(j=0; strbuf[j + strlen(cmd_StaffSignup)] != 0; j++)
	{
		staffs_Database[staffNum].password[j] = strbuf[j + i + strlen(cmd_StaffSignup)];
	}
	staffs_Database[staffNum].password[j] = 0;

	
	uint32_t TIM_NowCount = TIM_GetClock();
	staffs_Database[staffNum].login_TimeSave[staffs_Database[staffNum].loginNum].Year=year ;
	staffs_Database[staffNum].login_TimeSave[staffs_Database[staffNum].loginNum].Month=month ;
	staffs_Database[staffNum].login_TimeSave[staffs_Database[staffNum].loginNum].Day=day ;
	staffs_Database[staffNum].login_TimeSave[staffs_Database[staffNum].loginNum].Hour=TIM_NowCount / 3600 ;
	staffs_Database[staffNum].login_TimeSave[staffs_Database[staffNum].loginNum].Min=(TIM_NowCount % 3600) / 60;
	staffs_Database[staffNum].login_TimeSave[staffs_Database[staffNum].loginNum].Sec=(TIM_NowCount % 3600) % 60;
	
	staffs_Database[staffNum].loginNum++;
	
	printf("signup successfully!\r\n");
	printf("%s:%02d:%02d:%02d %04d/%02d/%02d\r\n\r\n",staffs_Database[staffNum++].user_name,\
	TIM_NowCount / 3600,(TIM_NowCount % 3600) / 60,(TIM_NowCount % 3600) % 60,year,month,day);
}




void staffLoginNew_intoDatabase(uint8_t serialNum)
{
	printf("\r\nwait for a second...\r\n\r\n");
	uint32_t TIM_NowCount = TIM_GetClock();
	staffs_Database[serialNum].login_TimeSave[staffs_Database[serialNum].loginNum].Year=year ;
	staffs_Database[serialNum].login_TimeSave[staffs_Database[serialNum].loginNum].Month=month ;
	staffs_Database[serialNum].login_TimeSave[staffs_Database[serialNum].loginNum].Day=day ;
	staffs_Database[serialNum].login_TimeSave[staffs_Database[serialNum].loginNum].Hour=TIM_NowCount / 3600 ;
	staffs_Database[serialNum].login_TimeSave[staffs_Database[serialNum].loginNum].Min=(TIM_NowCount % 3600) / 60;
	staffs_Database[serialNum].login_TimeSave[staffs_Database[serialNum].loginNum].Sec=(TIM_NowCount % 3600) % 60;
	
	staffs_Database[serialNum].loginNum++;
	
	uint8_t i=4;
	while(--i)
	{
	LED0_Turn();
	Delay_ms(500);
	}
	printf("login successfully!\r\n");
	printf("%s:%02d:%02d:%02d %04d/%02d/%02d\r\n\r\n",staffs_Database[serialNum].user_name,\
	TIM_NowCount / 3600,(TIM_NowCount % 3600) / 60,(TIM_NowCount % 3600) % 60,year,month,day);
}


void staff_NotFound()
{
	printf("\r\nwait for a second...\r\n");
	uint8_t i=10;
	while(--i)
	{
	LED0_Turn();
	Delay_ms(1000);
	}
	printf("\r\nerror:User Name Not Found!\r\n");
}

int main(void)
{
	//设置中断分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//初始化(自定义)
	Project_InitAll();
	//设备启动(自定义)
	DeviceStart();
	//系统时间设置(自定义)
	TimeSet();
	while (1)
	{
		Print_MainMeun(1);
		switch(KEY_Scan())
			{
			case 0:
			{
				Print_MainMeun(2);
				while(1)
				{
					if(USART_GetString(strbuf)!=NULL)
					{
						if(strncmp(strbuf,cmd_StaffLogin,strlen(cmd_StaffLogin))==0)
						{
							uint8_t i;
							FlagStatus staffSearchFlag;
							unsigned char staffName[20];
							
							for(i=0; strbuf[i+strlen(cmd_StaffLogin)] != 0;i++)
							{
								staffName[i] = strbuf[i+strlen(cmd_StaffLogin)];
							}
							staffName[i]=0;
							
							for(i=0; i<staffNum; i++)
							{
								if(strncmp(staffs_Database[i].user_name, staffName, strlen(staffName))==0)
								{
									staffLoginNew_intoDatabase(i);
									staffSearchFlag = SET;
								}
							}
							
							if(staffSearchFlag != SET)
								staff_NotFound();
								
							
						}
						else if(strncmp(strbuf,cmd_Exit,strlen(cmd_Exit))==0)
						{
							memset(strbuf,0,sizeof strbuf);
							break;
						}
						else if(*strbuf != 0){
							
							Print_MainMeun(0);
						}
						memset(strbuf,0,sizeof strbuf);
					}
				}
			}break;
		
			case 1:
			{

			}break;
			
			
			case 2:
			{
								Print_MainMeun(3);
				while(1)
				{
					if(USART_GetString(strbuf)!=NULL)
					{
						if(strncmp(strbuf,cmd_StaffLogin,strlen(cmd_StaffLogin))==0)
						{
							staffSignupNew(strbuf);
							
						}
						else if(strncmp(strbuf,cmd_Exit,strlen(cmd_Exit))==0)
						{
							memset(strbuf,0,sizeof strbuf);
							break;
						}
						else if(*strbuf != 0){
							Print_MainMeun(0);
						}
						
						
						memset(strbuf,0,sizeof strbuf);
					}
				}
			}break;
		}
	
	}
}

