#include "myhead.h"
#include "http.h"
//定义线程体函数
void* msg_request(void* arg)
{
    int sock=*(int*)arg;
    handler_msg(sock);
}
int main(int argc,const char* argv[])
{
    int port=80;
    if(argc>1)
    {
        port=atoi(argv[1]);
    }
    int lis_socket=init_server(port);
    while(1)
    {
        struct sockaddr_in peer;
        socklen_t len=sizeof(peer);
        int sock=accept(lis_socket,(struct sockaddr*)&peer,&len);
        if(sock==-1)
        {
            perror("accept error");
            return -1;
        }
        printf("您有新客户端发来连接请求\n");
        pthread_t tid=-1;
        if(pthread_create(&tid,NULL,msg_request,&sock)>0)
        {
            printf("pthread_create error\n");
            return -1;
        }
        pthread_detach(tid);
    }
    close(lis_socket);
    return 0;
}

