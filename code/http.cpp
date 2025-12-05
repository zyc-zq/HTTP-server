#include "http.h"
#include "custom_handle.h"
#include "myhead.h"
int init_server(int _port)
{
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock==-1)
    {
        perror("socket error");
        return -1;
    }
    int reuse=1;
    if(setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse))==-1)
    {
        perror("setsockopt error");
        return -1;
    }
    struct sockaddr_in local;
    local.sin_family=AF_INET;
    local.sin_port=htons(_port);
    local.sin_addr.s_addr=INADDR_ANY;
    if(bind(sock,(struct sockaddr*)&local,sizeof(local))==-1)
    {
        perror("bind error");
        return -1;
    }
    if(listen(sock,128)==-1)
    {
        perror("listen error");
        return -1;
    }
    return sock;
}
int get_line(int sock,char* buf)
{
    char ch='\0';
    int i=0;
    int ret=0;
    while(i<SIZE && ch!='\n')
    {
        ret=recv(sock,&ch,1,0);
        if(ret>0&& ch=='\r')
        {
            int s=recv(sock,&ch,1,MSG_PEEK);
            if(s>0&&ch=='\n')
            {
                recv(sock,&ch,1,0);
            }
            else
            {
                ch='\n';
            }
        }
        buf[i]=ch;
        i++;
    }
    buf[i]='\0';
    return i;
}
static void clear_header(int sock)
{
    char buf[4096]="";
    int ret=0;
    do{
        ret=get_line(sock,buf);
    }while(ret!=1&&strcmp(buf,"\n")!=0);
}

static void show_404(int sock)
{
    clear_header(sock);
    const char*msg="HTTP/1.1 404 NOT FOUND\r\n";
    send(sock,msg,strlen(msg),0);
    send(sock,"\r\n",strlen("\r\n"),0);
    struct stat st;
    stat("./wwwroot/404.html",&st);
    int fd=open("./wwwroot/404.html",O_RDONLY);
    if(fd==-1)
    {
        perror("open 404 error");
        return;
    }
    sendfile(sock,fd,NULL,st.st_size);
    close(fd);
}
void echo_error(int sock,int err_code)
{
    switch(err_code)
    {
        case 403:
            break;
        case 404:
            show_404(sock);
            break;
        case 405:
            break;
        case 500:
            break;
        default:
            break;
    }
}
static int echo_www(int sock,const char* path,size_t s)
{
    int fd=open(path,O_RDONLY);
    if(fd==-1)
    {
        perror("open error");
        return -1;
    }
    const char* msg="HTTP/1.1 200 OK\r\n";
    send(sock,msg,strlen(msg),0);
    send(sock,"\r\n",strlen("\r\n"),0);
    if(sendfile(sock,fd,NULL,s)==-1)
    {
        echo_error(sock,500);
        return -1;
    }
    close(fd);
    return 0;
}
static int handle_request(int sock,const char* method,const char*path,const char* query_string)
{
    char line[SIZE]="";
    int ret=0;
    int content_len=-1;
    if(strcasecmp(method,"GET")==0)
    {
        clear_header(sock);
    }else{
        do{
            ret=get_line(sock,line);
            if(strncasecmp(line,"content-length",14)==0)
            {
                content_len=atoi(line+16);
            }
        }while(ret!=1&&(strcmp(line,"\n"))!=0);
    }
    printf("method=%s\n",method);
    printf("query_string=%s\n",query_string);
    printf("content_len=%d\n",content_len);
    char req_buf[4096]="";
    if(strcasecmp(method,"POST")==0)
    {
        int len=recv(sock,req_buf,content_len,0);
        printf("len=%d\n",len);
        printf("req_buf=%s\n",req_buf);
    }
    // 原错误：只发"HTTP/1.1 200 OK\r\n"，无Content-Type和空行
    const char* msg="HTTP/1.1 404 NOT FOUND\r\nContent-Type: text/html; charset=utf-8\r\n\r\n";
    send(sock,msg,strlen(msg),0);
    parse_and_process(sock,query_string,req_buf);
}

int handler_msg(int sock)
{
    char del_buf[SIZE]="";
    recv(sock, del_buf, SIZE, MSG_PEEK);
    #if 1
    printf("-------------------------------------------------\n");
    printf("%s\n", del_buf);
    printf("-------------------------------------------------\n");
    #endif
    char buf[SIZE]="";
    int count=get_line(sock,buf);
    char method[32]="";
    int k=0;
    int i=0;
    for(i;i<count;i++)
    {
        if(isspace(buf[i]))
        {
            break;
        }
        method[k++]=buf[i];
    }
    method[k]='\0';
    while(isspace(buf[i])&&i<SIZE)
    {
        i++;
    }
    if(strcasecmp(method,"GET")!=0&&strcasecmp(method,"POST")!=0)
    {
        printf("method error\n");
        close(sock);
        return -1;
    }
    int need_handle=0;
    if(strcasecmp(method,"POST")==0)
    {
        need_handle=1;
    }
    char url[SIZE]="";
    int t=0;
    char *query_string=NULL;
    for(i;i<SIZE;i++)
    {
        if(isspace(buf[i]))
        {
            break;
        }
        if(buf[i]=='?')
        {
            query_string=&url[t];
            query_string++;
            url[t]='\0';
        }else{
            url[t]=buf[i];
        }
        t++;
    }
    url[t]='\0';
    printf("url = %s\n", url);
    printf("query_string = %s\n", query_string);
    if(strcasecmp(method, "GET")==0 && query_string!=NULL)
    {
        need_handle = 1; 
    }
    char path[SIZE]="";
    sprintf(path,"../wwwroot%s",url);
    if(path[strlen(path)-1]=='/')
    {
        strcat(path,"index.html");
    }
    printf("path=%s\n",path);
    struct stat st;
    if(stat(path,&st)==-1)
    {
        printf("can not find file\n");
        echo_error(sock,404);
        close(sock);
        return -1;
    }
    if(need_handle==1)
    {
        handle_request(sock,method,path,query_string);
    }else{
        clear_header(sock);
        echo_www(sock,path,st.st_size);
    }
    close(sock);
    return 0;
}

