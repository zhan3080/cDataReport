#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
//#include <time.h>
#include <pthread.h>
#include "dataReport.h"
#include "dataHandle.h"
#include "http.h"

#define POSTURL "http://120.77.23.79"
//全局变量初始化
static void global_data_init()
{
	//生成全局uid
	char string[64];
	get_device_uid(string);
	generate_string_md5(string, g_uid, UID_LEN);
	//生成全局session_id
	generate_session_id(g_session_id, SESSION_ID_LEN-1);
	g_report_frequency = DEFAULT_REPORT_FREQUENCY;
}

//保存程序启动信息
void set_login_data()
{
	cJSON *login_logout_info = cJSON_CreateObject();
	build_login_logout_info(login_logout_info, LOGIN);
	write_data_to_file(FILE_PATH, cJSON_Print(login_logout_info));
	if(true==copy_file(FILE_PATH,REPORT_FILE_PATH))
	{
		clean_file(FILE_PATH);
	}
}

//保存程序退出信息
void set_logout_data()
{
	cJSON *login_logout_info = cJSON_CreateObject();
	build_login_logout_info(login_logout_info, LOGOUT);
	write_data_to_file(FILE_PATH, cJSON_Print(login_logout_info));
	if(true==copy_file(FILE_PATH,REPORT_FILE_PATH))
	{
		clean_file(FILE_PATH);
	}
}


//上报数据线程
void *report_data_thread(void *param)
{
	global_data_init();
	set_login_data();
	DATA_INFO data_info;
	data_info.type = STARTCAST;
	strcpy(data_info.cast_info,"http://www.leto.psde/infos/drr.mp4");
	data_info.cast_type = AIRPLAY;
	cJSON *even_info = cJSON_CreateObject();
	build_even_info(even_info, &data_info);
	write_data_to_file(FILE_PATH, cJSON_Print(even_info));
	
	data_info.type = ENDCAST;
	data_info.status = true;
	data_info.cast_type = AIRPLAY;
	cJSON *even_info1 = cJSON_CreateObject();
	build_even_info(even_info1, &data_info);
	write_data_to_file(FILE_PATH, cJSON_Print(even_info1));
	
	if(true==copy_file(FILE_PATH,REPORT_FILE_PATH))
	{
		clean_file(FILE_PATH);
	}
	reprot_data(REPORT_FILE_PATH);
	printf("report_data_thread return\n");
	/*while(true)
	{
		printf("g_report_frequency=%d\n",g_report_frequency);
		sleep(g_report_frequency);
		reprot_data(REPORT_FILE_PATH);
	}*/
}

void start_report_data_service()
{
   printf("start_report_data_service\n");
   pthread_t REPORT_DATA_THREAD_ID = -1;
   pthread_create(&REPORT_DATA_THREAD_ID, NULL, report_data_thread,NULL);
   printf("start_report_data_service end\n");
   return ;
}


int main(int argc,char *argv[])
{
	printf("main\n");
	start_report_data_service();
	sleep(10);
	/*
	char s[] = "hzztest1";
	if(!http_post(POSTURL,s,strlen(s)))
	{
		printf("return false\n");
		return false;
	}
	global_data_init();
	unsigned long int len = get_content_length(REPORT_FILE_PATH);
	
	cJSON *login_logout_info = cJSON_CreateObject();
	build_login_logout_info(login_logout_info, LOGIN);
	int i = 0;
	for(i=0;i<3;i++)
	{
		write_data_to_file(FILE_PATH, cJSON_Print(login_logout_info));
	}
	
	if(true==copy_file(FILE_PATH,REPORT_FILE_PATH))
	{
		clean_file(FILE_PATH);
	}
	
	//len = get_content_length(REPORT_FILE_PATH);
	if(true==reprot_data(REPORT_FILE_PATH))
	{
		clean_file(REPORT_FILE_PATH);
	}*/
	return 0;
}




