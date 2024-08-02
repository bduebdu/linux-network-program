#include<iostream>
using namespace std;
#include <sys/types.h>          
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>

#define SERVER_PORT 8080

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
    //绑定IP和端口
    struct sockaddr_in localAddress;
    memset(&localAddress,0,sizeof(localAddress));

    localAddress.sin_family = AF_INET;
    localAddress.sin_port = htons(SERVER_PORT);
    localAddress.sin_addr.s_addr =htonl(INADDR_ANY);
    int ret = bind(sockfd,reinterpret_cast<const sockaddr *>(&localAddress),sizeof(localAddress));
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

    int connfd = accept(sockfd,NULL,NULL);
    char buffer[1024] = {0};
    
    int readBytes = 0;
    while (1)
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
    

    //释放资源
    close(connfd);
    close(sockfd);//关闭文件描述符
    cout<<"hello world"<<endl;
    

    return 0;
}