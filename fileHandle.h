#include <stdio.h>
#include <stdlib.h>
#include <string.h>    
#include <unistd.h> 
#include <stdbool.h>

//#define path "save.txt"
//#define DATA_MAX_LENGTH (1*1024*1024) //文件最大范围

//把上报事件保存到文件里
void write_data_to_file(char* file, char *data);

//从文件里读取数据
int read_data_from_file(char* file, char *data, int flen);


//获取文件长度
unsigned long int get_content_length(char *file_name);

//清空文件内容
void clean_file(char *file_name);

//文件复制，把src的内容复制到dst，并清空src
bool copy_file(char *src, char *dst);


