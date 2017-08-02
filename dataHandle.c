#include <stdio.h>
#include <stdlib.h>
#include <string.h>    
#include <unistd.h> 
//#include <stdbool.h>
//#include <sys/time.h> 
#include <time.h> 
#include "cJSON.h"
#include "http.h"
#include "md5.h"
#include "fileHandle.h"
#include "dataHandle.h"
#include "dataReport.h"

#define APP_ID "1004"
#define APP_OS "Linux"
#define APP_NAME "null"
#define APP_VERSION "1.0"



//#define POSTURL    "http://www.xiaomi.com/member/login"

//数据上传测试版服务器接口
//private static String DATAUPDATE_TEST_ROOT_URL = "http://120.77.23.79:8848";
//数据上传发布版服务器接口
//private static String DATAUPDATE_RELEASE_ROOT_URL = "http://data.hpplay.com.cn:8848";

#define POSTURL "http://120.77.23.79"
#define CPUINFO "CPUInfo.txt"
#define SERIAL "Serial"

extern int g_login_time;

void build_head_info(cJSON *head_info)	
{
	if(head_info==NULL)
	{
		return;
	}
	printf("app_id=%s\n",APP_ID);
	printf("uid=%s\n",g_uid);
	printf("session=%s\n",g_session_id);

	cJSON_AddStringToObject(head_info,"app_id",APP_ID);
	cJSON_AddStringToObject(head_info,"uid",g_uid);
	cJSON_AddStringToObject(head_info,"session",g_session_id);
	cJSON_AddStringToObject(head_info,"os",APP_OS);
	cJSON_AddStringToObject(head_info,"app_name",APP_NAME);
	cJSON_AddStringToObject(head_info,"app_ver",APP_VERSION);
	
}


void build_even_info(cJSON *even_info, DATA_INFO *data_info)
{
	if(even_info==NULL||data_info==NULL)
	{
		return;
	}
	printf("data_info->type=%d\n",data_info->type);
	if(data_info->type==STARTCAST)
	{
		cJSON_AddStringToObject(even_info,"type","startcast");
		cJSON_AddStringToObject(even_info,"cast_info",data_info->cast_info);
	}
	else if(data_info->type==ENDCAST)
	{
		cJSON_AddStringToObject(even_info,"type","endcast");
		cJSON_AddBoolToObject(even_info,"status",data_info->status);
	}
	else
	{
		printf("REPORT_DATA_TYPE error!\n");
		even_info = NULL;
	}
	
	if(data_info->cast_type==AIRPLAY)
	{
		cJSON_AddStringToObject(even_info,"cast_type","airplay");
	}
	else if(data_info->cast_type==MIRROR)
	{
		cJSON_AddStringToObject(even_info,"cast_type","mirror");
	}
	char time_info[TIME_INFO_LEN];
	get_current_time(time_info);
	cJSON_AddStringToObject(even_info,"time",time_info);
}

void build_login_logout_info(cJSON *login_logout_info, enum REPORT_DATA_TYPE type)
{
	if(login_logout_info==NULL)
	{
		return;
	}
	
	char time_info[TIME_INFO_LEN];
	get_current_time(time_info);
	if(type==LOGIN)
	{
		g_login_time = time((time_t*)NULL);
		printf("g_login_time=%d\n", g_login_time);
		cJSON_AddStringToObject(login_logout_info,"type","login");
		cJSON_AddStringToObject(login_logout_info,"time",time_info);
	}
	else if(type==LOGOUT)
	{
		int logout_time = time((time_t*)NULL);
		printf("logout_time=%d\n", logout_time);
		printf("g_login_time=%d\n", g_login_time);
		int duration = logout_time - g_login_time;
		printf("duration=%d\n", duration);
		cJSON_AddStringToObject(login_logout_info,"type","logout");
		cJSON_AddStringToObject(login_logout_info,"time",time_info);
		cJSON_AddNumberToObject(login_logout_info,"duration",duration);
	}
	else
	{
		printf("REPORT_DATA_TYPE error!\n");
		login_logout_info = NULL;
	}
}

/*
void build_login_info(cJSON *login_info)
{
	if(login_info==NULL)
	{
		return;
	}
	g_login_time = time((time_t*)NULL);
    printf("g_login_time=%d\n", g_login_time);
	char time_info[TIME_INFO_LEN];
	get_current_time(time_info);
	printf("time_info=%s",time_info);
	cJSON_AddStringToObject(login_info,"type","login");
	cJSON_AddStringToObject(login_info,"time",time_info);
	return;
}

void build_logout_info(cJSON *logout_info)
{
	if(logout_info==NULL)
	{
		return;
	}
	int logout_time = time((time_t*)NULL);
    printf("logout_time=%d\n", logout_time);
	printf("g_login_time=%d\n", g_login_time);
	int duration = logout_time - g_login_time;
	printf("duration=%d\n", duration);
	
	char time_info[TIME_INFO_LEN];
	get_current_time(time_info);
	printf("time_info=%s\n",time_info);
	cJSON_AddStringToObject(logout_info,"type","logout");
	cJSON_AddStringToObject(logout_info,"time",time_info);
	cJSON_AddNumberToObject(logout_info,"duration",duration);
	return;
}*/

void package_data(char *head_info, char *body, char *result)  
{  
    //char *result = malloc(strlen(head_info)+strlen(body)+1);//+1 for the zero-terminator  
    //in real code you would check for errors in malloc here  
    if (head_info == NULL || body == NULL || result == NULL) 
	{
		printf("package data fail");
		return;  
	}

	strcpy(result,"{\n\"headinfo\":"); //13
	strcat(result,head_info);
	strcat(result,",\n\"items\":[\n"); //12
	strcat(result,body);
	strcat(result,"\n]\n}"); //4
    //strcpy(result, head_info);  
    //strcat(result, body);  
  
    return;  
}  


bool reprot_data(char *file)  
{  
	if(NULL==file)
	{
		return false;
	}
	int data_len = get_content_length(file);
	
	if(data_len<=0)
	{
		printf("data_len=%d\n",data_len);
		return;
	}
	
	
	//生成数据报头
	cJSON *head_info = cJSON_CreateObject();
	build_head_info(head_info);
	char *s1 = (char*)cJSON_Print(head_info);
	//printf("head_info=%s\n",s1);
	
	//包装上报数据体
	int head_len = strlen(s1);
	printf("head_len=%d\n",head_len);
	
	char data[data_len];
	char result[data_len+head_len+EXTENAL_LEN+1];
	read_data_from_file(file, data,data_len);
	//printf("data=%s\n",data);
	package_data(s1, data,result);
	//printf("package_data result=%s\n",result);
    if(!http_post(POSTURL,result,data_len+head_len+EXTENAL_LEN+1))
	{
		printf("return false\n");
		return false;
	}
	
	//上传成功，则把文件清空
	clean_file(file);
    return true;  
}

void get_current_time(char *time_info){
	time_t timep;  
    struct tm *p;  
    time(&timep);  
	p = localtime(&timep); //此函数获得的tm结构体的时间，是已经进行过时区转化为本地时间  
    //p = gmtime(&timep); //把日期和时间转换为格林威治(GMT)时间的函数   
	sprintf(time_info,"%d-%02d-%02d %02d:%02d:%02d",1900+p->tm_year,1+p->tm_mon,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
	printf("time_info:  %s\n",time_info); 
	
	/*printf("Year:  %d\n", 1900+p->tm_year);  
    printf("Month:  %d\n", 1+p->tm_mon);  
    printf("Day:  %d\n", p->tm_mday);  
    printf("Hour:  %d\n", p->tm_hour);  
    printf("Minute:  %d\n", p->tm_min);  
    printf("Second:  %d\n",  p->tm_sec);
    printf("Weekday:  %d\n", p->tm_wday);  
    printf("Days:  %d\n", p->tm_yday);  
    printf("Isdst:  %d\n", p->tm_isdst);
	printf("当前的本地时间和日期：%s", asctime(p));
	*/
	/*struct	timeval	tv;
    struct	timezone tz;
    gettimeofday(&tv,&tz);
    printf("tv_sec:%d\n",tv.tv_sec);
	printf("tv_usec:%d\n",tv.tv_usec);
	printf("tz_minuteswest:%d\n",tz.tz_minuteswest);
	printf("tz_dsttime:%d\n",tz.tz_dsttime);
	*/
}

void generate_string_md5(char *string, unsigned char *md5_string, int md5_len)
{
	int index = 0;
	unsigned char md5_tmp[md5_len/2];
	MD5_CTX md5c; 
	MD5Init(&md5c); //初始化 
	MD5Update(&md5c, string, strlen(string));
	MD5Final(&md5c,md5_tmp);
	for(index=0;index<md5_len/2;index++)  
    {   
		sprintf(&md5_string[index*2],"%02x",md5_tmp[index]);
    }
	printf("md5_string=%s\n",md5_string);
	md5_string[md5_len-1] = '\0';
	
	/*int index = 0;
	printf("md5_tmp len=%d\n",strlen(md5_tmp));
	printf("\n加密前:%s\n加密后:",string);  
    for(index=0;index<16;index++)  
    {  
        printf("%02x",md5_tmp[index]);  
    }*/
}

//去掉strIn里前后空格，最后把字符串保存在strOut里
void trim(char *strIn, char *strOut)
{
    int i, j ;
    i = 0;
    j = strlen(strIn) - 1;
    while(strIn[i] == ' ')
        ++i;
    while(strIn[j] == ' ')
        --j;
    strncpy(strOut, strIn + i , j - i + 1);
    strOut[j - i + 1] = '\0';
}

bool get_device_uid(char *string)
{
	char cmd[30];
	sprintf(cmd,"cat %s",CPUINFO);
	FILE *pp = popen(cmd, "r"); //建立管道
    if (!pp) {
        return false;
    }
    char tmp[512]; //设置一个合适的长度，以存储每一行输出
    while (fgets(tmp, sizeof(tmp), pp) != NULL) {
		if(strstr(tmp,SERIAL))
        {	
			printf("tmp:%s\n",tmp);
			char *token  = strtok(tmp,":"); 
			if (token == NULL)  
			{  
				break; 
			}  
			token = strtok(NULL, ":");
			if (token == NULL)  
			{  
				break;
			}  
			trim(token,string);
			printf("uid=%s\n",string);
			pclose(pp); //关闭管道
			return true;
		}
    }
	printf("Serial is NULL\n");
	string = NULL;
    pclose(pp); //关闭管道
    return false;
}

void generate_session_id(char *session_id, int len)
{
    int i;
    srand(time(NULL));//通过时间函数设置随机数种子，使得每次运行结果随机。
    for(i = 0; i < len; i ++)
    {
        session_id[i] = rand()%('9'-'0' + 1) + '0'; //生成要求范围内的随机数。
    }
	session_id[len]='\0';
    printf("session_id：%s\n", session_id);//输出生成的随机数。
}


