#include <stdio.h>  
#include <stdlib.h>  
#include <arpa/inet.h>  
#include <netdb.h>  
#include <string.h>  
#include "http.h"  
  
#define BUFFER_SIZE 1024  
#define HTTP_POST "POST /%s HTTP/1.1\r\nHOST: %s:%d\r\nAccept: */*\r\n"\  
	"Content-Type:application/x-www-form-urlencoded\r\nContent-Length: %d\r\n\r\n%s"  
#define HTTP_GET "GET /%s HTTP/1.1\r\nHOST: %s:%d\r\nAccept: */*\r\n\r\n"  

#define MY_HTTP_DEFAULT_PORT 8848   
  
static int http_tcpclient_create(const char *host, int port){  
	struct hostent *he;  
	struct sockaddr_in server_addr;   
	int socket_fd;  
  
	if((he = gethostbyname(host))==NULL){  
		return -1;  
	}  
  
   server_addr.sin_family = AF_INET;  
   server_addr.sin_port = htons(port);  
   server_addr.sin_addr = *((struct in_addr *)he->h_addr);  
  
	if((socket_fd = socket(AF_INET,SOCK_STREAM,0))==-1){  
		return -1;  
	}  
  
	if(connect(socket_fd, (struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1){  
		return -1;  
	}  
	//设置超时 ret==0设置成功
	struct timeval timeout={5,0};//3s
    int ret = setsockopt(socket_fd,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
    ret = setsockopt(socket_fd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
	printf("set time_out = %d, ret=%d\n",timeout.tv_sec,ret);
	return socket_fd;  
}  
  
static void http_tcpclient_close(int socket){  
	close(socket);  
}  
  
static int http_parse_url(const char *url,char *host,char *file,int *port)  
{  
	char *ptr1,*ptr2;  
	int len = 0;  
	if(!url || !host || !file || !port){  
		return -1;  
	}  
  
	ptr1 = (char *)url;  
  
	if(!strncmp(ptr1,"http://",strlen("http://"))){  
		ptr1 += strlen("http://");  
	}else{  
		return -1;  
	}  
  
	ptr2 = strchr(ptr1,'/');  
	if(ptr2){  
		len = strlen(ptr1) - strlen(ptr2);  
		memcpy(host,ptr1,len);  
		host[len] = '\0';  
		if(*(ptr2 + 1)){  
			memcpy(file,ptr2 + 1,strlen(ptr2) - 1 );  
			file[strlen(ptr2) - 1] = '\0';  
		}  
	}else{  
		memcpy(host,ptr1,strlen(ptr1));  
		host[strlen(ptr1)] = '\0';  
	}  
	//get host and ip  
	ptr1 = strchr(host,':');  
	if(ptr1){  
		*ptr1++ = '\0';  
		*port = atoi(ptr1);  
	}else{  
		*port = MY_HTTP_DEFAULT_PORT;  
	}  
  
	return 0;  
}  
  
  
static int http_tcpclient_recv(int socket,char *lpbuff){  
	int recvnum = 0;  
  
	recvnum = recv(socket, lpbuff,BUFFER_SIZE*4,0);  
  
	return recvnum;  
}  
  
static int http_tcpclient_send(int socket,char *buff,int size){  
	int sent=0,tmpres=0;  
  
	while(sent < size){  
		tmpres = send(socket,buff+sent,size-sent,0);  
		if(tmpres == -1){  
			return -1;  
		}  
		sent += tmpres;  
	}  
	return sent;  
}  
  
static char *http_parse_result(const char*lpbuf)  
{  
	char *ptmp = NULL;   
	char *response = NULL;  
	ptmp = (char*)strstr(lpbuf,"HTTP/1.1");  
	if(!ptmp){  
		printf("http/1.1 not faind\n");  
		return NULL;  
	}  
	if(atoi(ptmp + 9)!=200){  
		printf("result:\n%s\n",lpbuf);  
		return NULL;  
	}  
  
	ptmp = (char*)strstr(lpbuf,"\r\n\r\n");  
	if(!ptmp){  
		printf("ptmp is NULL\n");  
		return NULL;  
	}  
	response = (char *)malloc(strlen(ptmp)+1);  
	if(!response){  
		printf("malloc failed \n");  
		return NULL;  
	}  
	strcpy(response,ptmp+4);  
	printf("response:\n%s\n",response); 
	return response;  
}  
  
char * http_post(const char *url,const char *post_str,const int str_len){  
  
	char post[BUFFER_SIZE] = {'\0'};  
	int socket_fd = -1;  
	char lpbuf[BUFFER_SIZE+str_len];// = {'\0'};  
	char *ptmp;  
	char host_addr[BUFFER_SIZE] = {'\0'};  
	char file[BUFFER_SIZE] = {'\0'};  
	int port = 0;  
	int len=0;  
	char *response = NULL;  
  
	if(!url || !post_str){  
		printf("failed!\n");  
		return NULL;  
	}  
  
	if(http_parse_url(url,host_addr,file,&port)){  
		printf("http_parse_url failed!\n");  
		return NULL;  
	}  
	printf("host_addr : %s\tfile:%s\t,%d\n",host_addr,file,port);  
  
	socket_fd = http_tcpclient_create(host_addr,port);  
	if(socket_fd < 0){  
		printf("http_tcpclient_create failed\n");  
		return NULL;  
	}  
	   
	sprintf(lpbuf,HTTP_POST,file,host_addr,port,strlen(post_str),post_str);  
	//sprintf(lpbuf,HTTP_POST,file,host_addr,port,str_len,post_str); 
	int send_data_len = http_tcpclient_send(socket_fd,lpbuf,strlen(lpbuf));
	printf("发送数据:\nsend_data_len=%d\n",send_data_len);    
	if(send_data_len < 0){  
		printf("http_tcpclient_send failed..\n");  
		return NULL;  
	}  
	printf("发送请求:\n%s\n",lpbuf);  
  
	/*it's time to recv from server*/  
	if(http_tcpclient_recv(socket_fd,lpbuf) <= 0){  
		printf("http_tcpclient_recv failed\n");  
		return NULL;  
	}  
  
	http_tcpclient_close(socket_fd);  
	printf("receive:\n%s\n",lpbuf); 
	return http_parse_result(lpbuf);  
}  
  
char * http_get(const char *url)  
{  
  
	char post[BUFFER_SIZE] = {'\0'};  
	int socket_fd = -1;  
	char lpbuf[BUFFER_SIZE*4] = {'\0'};  
	char *ptmp;  
	char host_addr[BUFFER_SIZE] = {'\0'};  
	char file[BUFFER_SIZE] = {'\0'};  
	int port = 0;  
	int len=0;  
  
	if(!url){  
		printf("      failed!\n");  
		return NULL;  
	}  
  
	if(http_parse_url(url,host_addr,file,&port)){  
		printf("http_parse_url failed!\n");  
		return NULL;  
	}  
	//printf("host_addr : %s\tfile:%s\t,%d\n",host_addr,file,port);  
  
	socket_fd =  http_tcpclient_create(host_addr,port);  
	if(socket_fd < 0){  
		printf("http_tcpclient_create failed\n");  
		return NULL;  
	}  
  
	sprintf(lpbuf,HTTP_GET,file,host_addr,port);  
  
	if(http_tcpclient_send(socket_fd,lpbuf,strlen(lpbuf)) < 0){  
		printf("http_tcpclient_send failed..\n");  
		return NULL;  
	}  
//  printf("发送请求:\n%s\n",lpbuf);  
  
	if(http_tcpclient_recv(socket_fd,lpbuf) <= 0){  
		printf("http_tcpclient_recv failed\n");  
		return NULL;  
	}  
	http_tcpclient_close(socket_fd);  
  
	return http_parse_result(lpbuf);  
}  