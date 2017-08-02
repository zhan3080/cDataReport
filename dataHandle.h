#include <stdio.h>
#include <stdlib.h>
#include <string.h>    
#include <unistd.h> 
#include <stdbool.h>
#include "cJSON.h"

#define APP_ID "1004"
#define APP_OS "Linux"
#define APP_NAME "null"
#define APP_VERSION "1.0"

#define TIME_INFO_LEN 20

/*typedef struct _data_head_info {
	int app_id;					
	char *app_name;			
	char *app_version;
	char *app_os;				
} data_head_info;*/


/* 定义事件枚举类型 */
enum REPORT_DATA_TYPE 
{
	LOGIN = 0, //程序启动
	LOGOUT,    //程序关闭
	STARTCAST, //开始投屏
	ENDCAST    //结束投屏
};
/* 定义投屏种类枚举类型 */
enum CAST_TYPE
{
	AIRPLAY = 0,
	MIRROR
};

typedef struct DATA_INFO 
{
	enum REPORT_DATA_TYPE type;					
	enum CAST_TYPE cast_type;
	char cast_info[1024];//url信息
	bool status;		 //投屏状态(0:success)
} DATA_INFO;



void build_head_info(cJSON *head_info);

//void build_login_info(cJSON *login_info);
void build_login_logout_info(cJSON *login_logout_info, enum REPORT_DATA_TYPE type);

void build_even_info(cJSON *even_info, DATA_INFO *data_info);

//把需要上传的数据封装网络包
void package_data(char *head_info, char *body, char *result);

//把指定文件的数据上传数据到服务器
bool reprot_data(char *file);

//获取当前时间，按格式返回 ”%04d-%02d-%02d %02d:%02d:%02d“
void get_current_time(char *time_info);

//生成string的MD5加密后的数据
void generate_string_md5(char *string, unsigned char *md5_string, int md5_len);

//获取Serial，作为uid
bool get_device_uid(char *string);

//生成随机数，作为session_id，随机数长度len
void generate_session_id(char *session_id, int len);
