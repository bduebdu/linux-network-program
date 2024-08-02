#include<iostream>
using namespace std;
#include <sys/types.h>          
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#define SERVER_PORT 8080

int main()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1)
    {
        perror("socket error");
        _exit(-1);
    }
    cout << "sockfd:"<< sockfd<<endl;
    //连接服务器
    struct sockaddr_in serverAddress;
    memset(&serverAddress,0,sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    const char * ip = "192.168.159.131";
    inet_pton(AF_INET,ip,&(serverAddress.sin_addr.s_addr));
    //连接
    int ret = connect(sockfd,reinterpret_cast<const sockaddr*>(&serverAddress),sizeof(serverAddress));
    if(ret!=0)
    {
        perror("connect error");
        _exit(-1);
    }

char writebuffer[128]={0};
char readbuffer[128]={0};
while(1)
{
        cout<<"输入发送信息"<<endl;
        cin>>writebuffer;
        write(sockfd,writebuffer,strlen(writebuffer)+1);
        sleep(1);//休眠

        read(sockfd,readbuffer,sizeof(readbuffer));
        cout<<"server:"<<readbuffer<<endl;


}


    return 0;
}