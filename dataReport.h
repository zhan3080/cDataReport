#ifndef dataReport__h
#define dataReport__h
#include <stdbool.h>
//uuid经过MD5加密后的长度，固定16
#define UID_LEN 33
//session_id数组长度，实际session_id位数=数组长度-1
#define SESSION_ID_LEN 11
//加上数据包头后，还需要增加的字符长度
#define EXTENAL_LEN 29 
//日期时间字符串长度
#define TIME_INFO_LEN 20
//默认上报时间间隔，单位秒
#define DEFAULT_REPORT_FREQUENCY 10

#define FILE_PATH "save.txt"
#define REPORT_FILE_PATH "report.txt"

//全局变量，初始化时赋值
unsigned char g_uid[UID_LEN];
char g_session_id[SESSION_ID_LEN];
int g_login_time;
int g_report_frequency;


//保存程序启动信息
void set_logout_data();

//保存程序退出信息
void set_login_data();

#endif