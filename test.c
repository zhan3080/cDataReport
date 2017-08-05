#include <stdio.h>
#include <stdlib.h>
#include <string.h>    
#include <unistd.h> 
#include "cJSON.h"
#include "fileHandle.h"
#include "dataHandle.h"
#include "md5.h"



#include <sys/time.h> 
#include <time.h> 
int main (int argc, const char * argv[]) {
	
	//生成json格式数据，并写到文件
	/*cJSON *login_info = cJSON_CreateObject();
	build_login_info(login_info);
	int i = 0;
	for(i=0;i<3;i++)
	{
		write_data_to_file(FILE_PATH, cJSON_Print(login_info));
	}
	
	if(true==copy_file(FILE_PATH,REPORT_FILE_PATH))
	{
		clean_file(FILE_PATH);
	}
	if(true==reprot_data(REPORT_FILE_PATH))
	{
		clean_file(REPORT_FILE_PATH);
	}*/
	//time_t timep;  
    //struct tm *p;  
    //time(&timep);  
	//p = localtime(&timep); //此函数获得的tm结构体的时间，是已经进行过时区转化为本地时间  
    //p = gmtime(&timep); //把日期和时间转换为格林威治(GMT)时间的函数  
  
    /*printf("Year:  %d\n", 1900+p->tm_year);  
    printf("Month:  %d\n", 1+p->tm_mon);  
    printf("Day:  %d\n", p->tm_mday);  
    printf("Hour:  %d\n", p->tm_hour);  
    printf("Minute:  %d\n", p->tm_min);  
    printf("Second:  %d\n",  p->tm_sec);  
    printf("Weekday:  %d\n", p->tm_wday);  
    printf("Days:  %d\n", p->tm_yday);  
    printf("Isdst:  %d\n", p->tm_isdst);
	
	time_t rawtime;
	struct tm *info;
	char buffer[80];
	time( &rawtime );
	info = localtime( &rawtime );*/
	//printf("当前的本地时间和日期：%s", asctime(p));

	/*struct	timeval	tv;
    struct	timezone tz;
    gettimeofday(&tv,&tz);
    printf("tv_sec:%d\n",tv.tv_sec);
	printf("tv_usec:%d\n",tv.tv_usec);
	printf("tz_minuteswest:%d\n",tz.tz_minuteswest);
	printf("tz_dsttime:%d\n",tz.tz_dsttime);
	
	char time_info[TIME_INFO_LEN];
	get_current_time(time_info);*/
		
	/*char encrypt[] = "12345";
	unsigned char decrypt[16]; //存放结果 
	MD5_CTX md5c; 
	MD5Init(&md5c); //初始化 
	MD5Update(&md5c, encrypt, strlen((char *)encrypt));
	MD5Final(&md5c,decrypt);
	printf("\n加密前:%s\n加密后:",encrypt);  
    for(index=0;index<16;index++)  
    {  
        printf("%02x",decrypt[index]);  
    }*/
	
	
	/*char string[64];
	get_device_uuid(string);
	printf("uuid=%s\n",string);
	printf("uuid_len=%d\n",strlen(string));
	
	int index = 0;
	unsigned char decrypt[16]; //存放结果 
	//char encrypt[] = "6c:0b:84:07:3a:32"; //"DC:53:60:89:15:24";
	generate_string_md5(string, decrypt);
	printf("len=%d",strlen(decrypt)); 
	printf("\n加密前encrypt:%s\n加密后:",string);  
    for(index=0;index<16;index++)  
    {  
        printf("%02x",decrypt[index]);  
    }*/
	char session_id[11];
	generate_session_id(session_id, 10);
	printf("sesion_id=%s\n",session_id);
	printf("\n");
	return 0;
}
