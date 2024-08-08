#include<iostream>
using namespace std;
#include <sys/types.h>          
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <pthread.h>
#define SERVER_PORT 8080

void*handleClientInfo(void *arg)
{
    pthread_detach(pthread_self());
    //通信的文件描述符
    int connfd = *(int*)arg;
    char buffer[1024] = {0};
    int readBytes = 0;
    while(1)
    {
        readBytes = read(connfd, buffer, sizeof(buffer));
        if(readBytes <0)
        {
            cout<<"readBytes <0"<<endl;
            break;
        }
        else if(readBytes==0)
        {
            cout<<"readBytes==0"<<endl;
            break;
        }
        else
        {
            cout<< "buffer:"<< buffer<< endl;  

            for(int idx = 0;idx< readBytes;idx++)
            {
                buffer[idx]= toupper(buffer[idx]);
            }
            write(connfd,buffer,readBytes);
        }
    }
    //回收资源并退出线程
    close(connfd);
    pthread_exit(NULL);
}


int main()
{
    //创建套接字
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1)
    {
        perror("socket error");
        _exit(-1);
    }

    cout<<"sockfd:"<<sockfd<<endl;
    //设置端口复用
    int optVal = 1;
    int ret = setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&optVal,sizeof(optVal));
    if(ret == -1)
    {
        perror("bind error");
        _exit(-1);
    }
    //绑定IP和端口
    struct sockaddr_in localAddress;
    memset(&localAddress,0,sizeof(localAddress));

    localAddress.sin_family = AF_INET;
    localAddress.sin_port = htons(SERVER_PORT);
    localAddress.sin_addr.s_addr =htonl(INADDR_ANY);
    ret = bind(sockfd,reinterpret_cast<const sockaddr *>(&localAddress),sizeof(localAddress));
    if(ret!=0)
    {
        perror("bind error");
        _exit(-1);
    }
    ret = listen(sockfd,10);
    if(ret!=0)
    {
        perror("listen error");
        _exit(-1);
    }
        
    int connfd = 0;

    while (1)
     {
        //接受到新客户端连接就解除阻塞
        int connfd = accept(sockfd,NULL,NULL);
        if(connfd==-1)
        {
            perror("acctpt error");
            _exit(-1);
        }
        //程序在此开辟线程处理客户端数据通信
        pthread_t tid = 0;
        ret = pthread_create(&tid,NULL,handleClientInfo,(void*)&connfd);
        if(ret!=0)
        {
            perror("thread create error");
            _exit(-1);
        }
        sleep(1);
     }
    

    //释放资源
    close(connfd);
    close(sockfd);//关闭文件描述符
    cout<<"hello world"<<endl;
    

    return 0;
}