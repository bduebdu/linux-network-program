#include<iostream>
using namespace std;
#include<sys/types.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include<unistd.h>
#include<cstring>
#define UDP_SERVER_PORT 999

int main()
{
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd ==-1)
    {
        perror("socket error");
        _exit(-1);
    }
    /* 设置端口复用 */
    int optVal = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
    if (ret != 0)
    {
        perror("bind error:");
        throw std::runtime_error("setsockopt error.");
    }

    /* 绑定IP和端口 */
    struct sockaddr_in localAddress;
    memset(&localAddress, 0, sizeof(localAddress));
    /* 地址族 */
    localAddress.sin_family = AF_INET;
    localAddress.sin_port = htons(UDP_SERVER_PORT);
    localAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    /* 绑定 */
    ret = bind(sockfd, reinterpret_cast<const sockaddr *>(&localAddress), sizeof(localAddress));
    if (ret != 0)
    {
        perror("bind error:");
        throw std::runtime_error("bind error");
    }
    char buffer[128] ={0};
    int readBytes = read(sockfd,buffer,sizeof(buffer));
    if(readBytes < 0)
    {
        perror("read error:");
        _exit(-1);
    }
    else if (readBytes == 0)
    {
        cout<<"readBytes == 0"<<endl;
    }
    else
    {
        cout<<"buffer:"<<buffer<<endl;
    }

    //关闭套接字
    close(sockfd);
    
    return 0;
}