/************************头文件包含*************************/
#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "key.h"
#include "led.h"
#include "Timer.h"
#include "Serial.h"
#include "string.h"
/**********************************************************/






/*******************************数据库查找模式枚举************************************/
enum{
	searchMode_OnlyName = 0,
	searchMode_PasswordCheck
}searchMode;
/************************************************************************************/












/*******************************数据库（结构体封装）********************************/
//年登记日志信息：月日、时分秒
struct loginTime{
	
	uint8_t Hour,Min,Sec;
	
	uint16_t Year,Month,Day;
};

//数据库：用户名、密码和登记日志信息
struct staff{
	
	unsigned char user_name[20];
	
	unsigned char password[20];
	
	struct loginTime login_TimeSave[20];
	
	uint8_t loginNum;
	
}staffs_Database[50];
/*********************************************************************************/










/*******************************全局变量************************************/
char 		strbuf[50];			//串口接收缓存寄存器

uint8_t 	staffNum;			//员工已注册数目

uint16_t 	year, month, day;	//起始时间年月日
/**************************************************************************/








/*******************************串口指令定义************************************/
static char cmd_SetTime[]="SETTIME";//设置时间的指令

static char cmd_StaffLogin[]="user:";

static char cmd_StaffSignup[]="user:,password:";

static char cmd_Exit[]="exit";
/*******************************************************************************/









/*******************************模拟加载中串口输出函数*********************************/

void Printf_Loading(uint16_t delay_time)
{
	printf("\r\nloading");
	Delay_ms(delay_time);
	printf(".");
	Delay_ms(delay_time);
	printf(".");
	Delay_ms(delay_time);
	printf(".");
	Delay_ms(delay_time);
	printf(".");
	Delay_ms(delay_time);
	printf(".\r\n\r\n");
	
}
/************************************************************************************/









/*******************************外设模块初始化************************************/
void Project_InitAll()
{
	Timer_Init();	//定时器
	OLED_Init();	//OLED显示器
	KEY_Init();		//按键
	LED_Init();		//LED
	Serial_Init();	//串口
}
/*********************************************************************************/









/**********************************************设备上电**************************************************/
void DeviceStart(void)
{
	printf("请长按按键打开设备\r\n\r\n");
	while(myKey_Value.longPressed == RESET);
	Printf_Loading(200);
	printf("设备打开成功，请用SETTIME命令设置当前系统时间,格式如下:\r\nSETTIME 19:30:21 2023/09/21\r\n\r\n");
}
/*******************************************************************************************************/









/*********************************************************************************设置系统初始时间*****************************************************************************************/

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
/*******************************************************************************************************************************************************************************************/








/*******************************串口显示菜单输出************************************/

void Printf_MainMeun(uint8_t MainMeun_Option)
{
	Printf_Loading(500);
	switch(MainMeun_Option){
		case 0:{
			printf("*************** error ***************\r\n");
		}break;
		case 1:{
			printf("**************Main Meun**************\r\n");
			printf("*                                   *\r\n");
			printf("*  --------signup:长按按键--------  *\r\n");
			printf("*                                   *\r\n");
			printf("*  --------login :单击按键--------  *\r\n");
			printf("*                                   *\r\n");
			printf("*  --------lookup:双击按键--------  *\r\n");
			printf("*                                   *\r\n");
			printf("*************************************\r\n");
		}break;
		case 2:{
			printf("*************login Meun**************\r\n");
			printf("*                                   *\r\n");
			printf("*  ------------user:***-----------  *\r\n");
			printf("*                                   *\r\n");
			printf("*  --------------exit-------------  *\r\n");
			printf("*                                   *\r\n");
			printf("*************************************\r\n");
		}break;
		case 3:{
			printf("*************lookup Meun**************\r\n");
			printf("*                                    *\r\n");
			printf("*  ----user:***,password:******----  *\r\n");
			printf("*                                    *\r\n");
			printf("*  --------------exit-------------   *\r\n");
			printf("*                                    *\r\n");
			printf("**************************************\r\n");
			
		}break;
		case 4:{
			printf("*************signup Meun**************\r\n");
			printf("*                                    *\r\n");
			printf("*  ----user:***,password:******----  *\r\n");
			printf("*                                    *\r\n");
			printf("*   -------------exit-------------   *\r\n");
			printf("*                                    *\r\n");
			printf("**************************************\r\n");
		}break;
		default:printf("\r\nerror commit!\r\n");
	}
}

/***************************************************************************************/











/*********************************************************************显示用户的所有登记日志***********************************************************************/
void printf_logsAll(uint8_t serialNum)
{
	
	printf("password correctly!\r\n");
	Printf_Loading(1000);
	uint8_t i;
	printf("***************************************\r\n");
	printf("*          user name:%-8s         *\r\n", staffs_Database[serialNum].user_name);
	printf("***************************************\r\n");
	for(i=0; i<staffs_Database[serialNum].loginNum; i++)
	{
		printf("*         %02d:%02d:%02d %04d/%02d/%02d         *\r\n",\
		 staffs_Database[serialNum].login_TimeSave[i].Hour, staffs_Database[serialNum].login_TimeSave[i].Min, staffs_Database[serialNum].login_TimeSave[i].Sec, \
		staffs_Database[serialNum].login_TimeSave[i].Year, staffs_Database[serialNum].login_TimeSave[i].Month, staffs_Database[serialNum].login_TimeSave[i].Day );
	}
	printf("***************************************\r\n");
}

/*****************************************************************************************************************************************************************/











/*******************************按键状态扫描函数************************************/
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

/**********************************************************************************/











/************************************************新用户信息录入函数***********************************************/


void staffSignupNew()
{
	printf("\r\nTyping into StaffDatabase!\r\n");
	Printf_Loading(1000);
	uint8_t i,j;
	for(i=0; strbuf[i + strlen(cmd_StaffLogin)] != ','; i++)
	{
		staffs_Database[staffNum].user_name[i] = strbuf[i + strlen(cmd_StaffLogin)];
	}
	staffs_Database[staffNum].user_name[i] = 0;


	for(j=0; strbuf[j + i + strlen(cmd_StaffSignup)] != 0; j++)
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

/****************************************************************************************************************/













/*********************************************用户打卡录入日志库**************************************************/
void staffLoginNew_intoDatabase(uint8_t serialNum)
{
	printf("\r\nTyping into LogDatabase!\r\n");
	Printf_Loading(200);
	uint32_t TIM_NowCount = TIM_GetClock();
	staffs_Database[serialNum].login_TimeSave[staffs_Database[serialNum].loginNum].Year=year ;
	staffs_Database[serialNum].login_TimeSave[staffs_Database[serialNum].loginNum].Month=month ;
	staffs_Database[serialNum].login_TimeSave[staffs_Database[serialNum].loginNum].Day=day ;
	staffs_Database[serialNum].login_TimeSave[staffs_Database[serialNum].loginNum].Hour=TIM_NowCount / 3600 ;
	staffs_Database[serialNum].login_TimeSave[staffs_Database[serialNum].loginNum].Min=(TIM_NowCount % 3600) / 60;
	staffs_Database[serialNum].login_TimeSave[staffs_Database[serialNum].loginNum].Sec=(TIM_NowCount % 3600) % 60;
	
	staffs_Database[serialNum].loginNum++;
	
	uint8_t i=4;
	while(i--)
	{
	LED0_Turn();
	Delay_ms(500);
	}
	printf("login successfully!\r\n");
	printf("%s:%02d:%02d:%02d %04d/%02d/%02d\r\n\r\n",staffs_Database[serialNum].user_name,\
	TIM_NowCount / 3600,(TIM_NowCount % 3600) / 60,(TIM_NowCount % 3600) % 60,year,month,day);
}
/******************************************************************************************************************/












/*******************************用户名未找到输出提示************************************/
void staff_NotFound()
{
	Printf_Loading(200);
	uint8_t i=10;
	while(i--)
	{
	LED0_Turn();
	Delay_ms(1000);
	}
	printf("error:User Name Not Found!\r\n");
}
/************************************************************************************/












/*************************************************按键初始化函数***************************************************/
void staff_searchName(uint8_t searchMode)
{
	Printf_Loading(200);
	uint8_t i,j,k;
	FlagStatus searchNameFlag;
	unsigned char staffName[20];
	for(i=0; strbuf[i+strlen(cmd_StaffLogin)] != 0 && strbuf[i+strlen(cmd_StaffLogin)] != ',';i++)
	{
		staffName[i] = strbuf[i+strlen(cmd_StaffLogin)];
	}
	staffName[i]=0;
	
	for(j=0; j<staffNum; j++)
	{
		if(strncmp(staffs_Database[j].user_name, staffName, strlen(staffName))==0)
		{
			searchNameFlag = SET;
			memset(staffName,0,sizeof staffName);
			if(searchMode ==searchMode_OnlyName )staffLoginNew_intoDatabase(j);
			
			else 
			{
				unsigned char staffPassword[20];
				for(k=0; strbuf[k + i + strlen(cmd_StaffSignup)] != 0; k++)
				{
					staffPassword[k] = strbuf[k+i+strlen(cmd_StaffSignup)];
				}
					staffPassword[k] = 0;
				if( strncmp(staffPassword, staffs_Database[j].password, strlen(staffs_Database[j].password)) == 0 )
					printf_logsAll(j);
				else
					printf("Password Error!\r\n");
			}
		break;
		}

	}
	if(searchNameFlag == RESET)
	{
		if(searchMode == searchMode_OnlyName) 
			staff_NotFound();
		else 
			printf("error:User Name Not Found!\r\n");
	}
}
/**********************************************************************************************************************/











/*******************************按键初始化函数************************************/


int main(void)
{

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	Project_InitAll();

	DeviceStart();

	TimeSet();
	
	while (1)
	{
		Printf_MainMeun(1);
		switch(KEY_Scan())
			{
			case 0:
			{
				Printf_MainMeun(2);
				while(1)
				{
					if(USART_GetString(strbuf)!=NULL && strbuf[0] != 0)
					{
						if(strncmp(strbuf,cmd_StaffLogin,strlen(cmd_StaffLogin))==0)
						{
							staff_searchName(searchMode_OnlyName);
							Printf_MainMeun(2);
						}
						else if(strncmp(strbuf,cmd_Exit,strlen(cmd_Exit))==0)
						{
							memset(strbuf,0,sizeof strbuf);
							break;
						}
						else if(*strbuf != 0){
							
							Printf_MainMeun(0);
							Printf_MainMeun(2);
						}
						memset(strbuf,0,sizeof strbuf);
					}
				}
			}break;
		
			case 1:
			{
				Printf_MainMeun(3);
				while(1)
				{
					if(USART_GetString(strbuf)!=NULL && strbuf[0] != 0)
					{
						if(strncmp(strbuf,cmd_StaffLogin,strlen(cmd_StaffLogin))==0)
						{
							staff_searchName(searchMode_PasswordCheck);
							Printf_MainMeun(3);
						}
						else if(strncmp(strbuf, cmd_Exit, strlen(cmd_Exit))==0)
						{
							memset(strbuf,0,sizeof strbuf);
							break;
						}
						else if(*strbuf != 0)
						{
							Printf_MainMeun(0);
							Printf_MainMeun(3);
						}
						memset(strbuf,0,sizeof strbuf);
					}
				}
				
			}break;
			
			
			case 2:
			{
					Printf_MainMeun(4);
				while(1)
				{
					if(USART_GetString(strbuf)!=NULL && strbuf[0] != 0)
					{
						if(strncmp(strbuf,cmd_StaffLogin,strlen(cmd_StaffLogin))==0)
						{
							staffSignupNew();
							Printf_MainMeun(4);
							
						}
						else if(strncmp(strbuf,cmd_Exit,strlen(cmd_Exit))==0)
						{
							memset(strbuf,0,sizeof strbuf);
							break;
						}
						else if(*strbuf != 0){
							Printf_MainMeun(0);
							Printf_MainMeun(4);
						}
						
						
						memset(strbuf,0,sizeof strbuf);
					}
				}
			}break;
			default:printf("\r\nerror commit!\r\n");
		}
	
	}
}
/************************************************************************************/
