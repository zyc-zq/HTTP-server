#include"custom_handle.h"
#include"myhead.h"
#define KB 1024
#define HTML_SIZE (64*1024)
static int handle_add(int sock,const char* req_buf)
{
    int num1,num2;
    sscanf(req_buf,"\"data1=%ddata2=%d\"",&num1,&num2);
    printf("num1=%d,num2=%d\n",num1,num2);
    char reply_buf[HTML_SIZE]="";
    sprintf(reply_buf,"%d",num1+num2);
    printf("reply_buf=%s\n",reply_buf);
    send(sock,reply_buf,strlen(reply_buf),0);
    return 0;
}
int handle_login(int sock,char *req_buf)
{
    char reply_buf[HTML_SIZE]="";
    char *uname=strstr(req_buf,"username=");
    uname+=strlen("username=");
    char *ptr=strstr(req_buf,"password=");
    *(ptr-1)='\0';
    char* passwd=ptr+strlen("password=");
    printf("账号：%s    密码：%s",uname,passwd);
    if(strcmp(uname,passwd)==0)
    {
        sprintf(reply_buf,"<script>localStorage.setItem('usr_user_name','%s');</script>",uname);
        strcat(reply_buf,"<script>window.location.href='/index.html';</script>");
        send(sock,reply_buf,strlen(reply_buf),0);
    }
}
int parse_and_process(int sock, const char *querry_string, char * req_buf)
{
    if(strstr(req_buf, "data1=") && strstr(req_buf, "data2="))
    {
        return handle_add(sock, req_buf);
    }else if(strstr(req_buf,"username=") && strstr(req_buf,"password=")) 
    {
        return handle_login(sock, req_buf);
    }else
    {
    }
}