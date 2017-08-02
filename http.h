#include <stdio.h>
#include <stdlib.h>
#include <string.h>    
#include <unistd.h> 
#include <stdbool.h>
 
char* http_get(const char *url);

char* http_post(const char *url, const char *post_str, const int str_len);

  