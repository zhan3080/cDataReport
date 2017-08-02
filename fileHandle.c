#include <stdio.h>
#include <stdlib.h>
#include <string.h>    
#include <unistd.h> 
#include <stdbool.h>
#include "fileHandle.h"

//#define FILE_PATH "save.txt"
#define DATA_MAX_LENGTH (1024*1024) //文件最大范围

//把上报事件保存到文件里
void write_data_to_file(char* file, char *data)
{	
	unsigned long int file_len;
	FILE *fp;
	fp = fopen(file,"a+"); //如果存在，打开，不存在，新建该文件
	if(!fp)  //检查文件是否打开，若打开失败，返回一条信息后，结束程序。 
    {
        printf("open fail!\n");
        return; 
    }
	fseek(fp,0,SEEK_END);  //移动到文件尾 ,假设文件指针变量为fp
	file_len = ftell(fp);       //ftell函数返回当前文件指针相对于文件头的偏移量,文件指针位于文件尾的话，那返回值就相当于文件长度了
	if(file_len>=DATA_MAX_LENGTH)
	{
		printf("File(%s) to maximum file_len=%d\n",file,file_len);
		fclose(fp);  //关闭文件。 
		return;
	}
	else if(file_len>0)//文件为空，
	{
		if(!fputs(",\n", fp))  //调用fputs函数写入文件 
		{
			printf("fail!\n");
			fclose(fp);  //关闭文件。 
			fp = NULL;   //放空file指针。
			return;
		}
		else{
			//printf("success!\n");
		}
	}
	
	/*int ch = fgetc(fp);
	//判断文件是否为空，不为空要写一个逗号及换行符到上一条信息的后面
	if ( ch!=EOF )
	{
		if(!fputs(",\n", fp))  //调用fputs函数写入文件 
		{
			printf("fail!\n");
			fclose(fp);  //关闭文件。 
			fp = NULL;   //放空file指针。
			return;
		}
		else{
			//printf("success!\n");
		}
	}*/
	
    if(!fputs(data, fp))  //调用fputs函数写入文件，不管成功或失败都会返回一条信息。 
	{
        printf("fail!\n"); 
	}     
    fclose(fp);  //关闭文件。 
    fp = NULL;   //放空file指针。 
    return ;
}

//从文件里读取数据
int read_data_from_file(char* file, char *data, int flen)
{
	FILE *fp;
	fp = fopen(file,"rb"); // path文件名      
	if(!fp)  //检查文件是否打开，若打开失败，返回一条信息后，结束程序。 
	{
		printf("open fail!\n");
		return 0; 
	}
	fread(data,flen,1,fp);//一次性读完文件
	data[flen] = 0; //字符串结束标志
	if(fp){
		fclose(fp);
	}
	return flen;
}



unsigned long int get_content_length(char *file_name)
{
	FILE *fp;
	fp = fopen(file_name,"r"); //只读方式打开文件
	if(!fp)  //检查文件是否打开，若打开失败，返回一条信息后，结束程序。 
	{
		printf("open fail!\n");
		return 0; 
	}
	unsigned long int len; //用len存储文件长度
	fseek(fp,0,SEEK_END);  //移动到文件尾 ,假设文件指针变量为fp
	len = ftell(fp);       //ftell函数返回当前文件指针相对于文件头的偏移量,文件指针位于文件尾的话，那返回值就相当于文件长度了
	if(fp)
	{
		fclose(fp);
	}
	printf("return file(%s) len=%d\n",file_name,len);
	return len;
}


void clean_file(char *file_name)
{
	/*if(access(src,F_OK)==0)
	{
		//remove(src);//如果存在就删除
		if (remove(src) == 0) 
			printf("Removed %s.\n",src); 
		else 
			perror("remove");
	}*/
	FILE *fp;
	fp = fopen(file_name, "w");
	if(fp)fclose(fp);
	return;
}

bool copy_file(char *src, char *dst)
{
    int c;
	unsigned long int file_len;
    FILE *fds = NULL,*fdd=NULL;
    fds = fopen(src, "r");
    fdd = fopen(dst, "a+");//打开文件。
    if(fds == NULL || fdd == NULL)//打开文件失败。
    {
        if(fds)fclose(fds);
        if(fdd) fclose(fdd);
        return false;
    }
	
	fseek(fdd,0,SEEK_END);  //移动到文件尾 ,假设文件指针变量为fp
	file_len = ftell(fdd);       //ftell函数返回当前文件指针相对于文件头的偏移量,文件指针位于文件尾的话，那返回值就相当于文件长度了
	if(file_len>=DATA_MAX_LENGTH)
	{
		printf("File(%s) to maximum file_len=%d\n",dst,file_len);
		if(fds)fclose(fds);
        if(fdd) fclose(fdd); 
		return;
	}
	
	
    while((c = fgetc(fds))!=EOF)//循环读文件到文件尾。
	{
        fputc(c, fdd);//写目标文件。
	}
    fclose(fds);
    fclose(fdd);//关闭文件。
	return true;
}

/*
//从文件里读取数据
int read_data_from_file_bak(char *data, int len)
{
	FILE *fp;
	char *p=NULL;
	fp = fopen(FILE_PATH,"rb"); // path文件名      
	if(!fp)  //检查文件是否打开，若打开失败，返回一条信息后，结束程序。 
	{
		printf("open fail!\n");
		return 0; 
	}
	fseek(fp,0L,SEEK_END); // 定位到文件末尾 
	int flen = ftell(fp); // 得到文件大小 
	printf("flen = %d\n",flen);
	p = (char*)malloc(flen+1); //根据文件大小动态分配内存空间 
	if(p==NULL)
	{
		fclose(fp);
		return 0;
	}
	fseek(fp,0L,SEEK_SET);//定位到文件开头
	fread(p,flen,1,fp);//一次性读完文件
	p[flen] = 0; //字符串结束标志
	strncpy(data,p,flen);
	free(p);
	if(fp){
		fclose(fp);
	}
	return flen;
}
*/
