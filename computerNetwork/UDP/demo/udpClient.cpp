#include<iostream>
using namespace std;
#include<sys/types.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include<unistd.h>
#include<cstring>
#include <arpa/inet.h>
#define UDP_SERVER_PORT 999
int main()
{
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd ==-1)
    {
        perror("socket error");
        _exit(-1);
    }
    //连接服务器的ip和端口
    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(UDP_SERVER_PORT);
    const char *ip = "";
    inet_pton(AF_INET,ip,&(serveraddr.sin_addr.s_addr));

    char buffer[128] ="hello";
    int writeBytes = sendto(sockfd,buffer,sizeof(buffer),0,reinterpret_cast<const sockaddr *>(&serveraddr),sizeof(serveraddr));
    cout<<"writeBytes = "<<writeBytes<<endl;

    return 0;
}