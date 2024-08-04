#include<iostream>
using namespace std;
#include "stdTcpServer.h"
#include "stdShared.h"
#include <unistd.h>
#include <string.h>

void interfaceMenu()
{
    cout<<"1.注册"<<endl;
    cout<<"2.登陆"<<endl;
    cout<<"3.退出"<<endl;
}
void registerFunc(StdTcpSocket &client)
{
    string message;
    string username;
    cout<<"用户名"<<endl;
    fflush(stdout);
    cin >>username;

    string passwd;
    cout<<"密码"<<endl;
    fflush(stdout);
    cin >>passwd;

    Msg msg;
    memset(&msg,0,sizeof(msg));

    msg.type = REGISTER;
    strncpy(msg.name,username.c_str(),sizeof(msg.name)-1);
    strncpy(msg.passwd,passwd.c_str(),sizeof(msg.passwd)-1);
    //用户名密码发送给服务器
    client.sendMessage(&msg,sizeof(msg));
}
void loginFunc(StdTcpSocket &client)
{
    string message;
    string username;
    cout<<"用户名"<<endl;
    fflush(stdout);
    cin >>username;

    string passwd;
    cout<<"密码"<<endl;
    fflush(stdout);
    cin >>passwd;
    Msg msg;
    memset(&msg,0,sizeof(msg));

    msg.type = LOGIN;
    strncpy(msg.name,username.c_str(),sizeof(msg.name)-1);
    strncpy(msg.passwd,passwd.c_str(),sizeof(msg.passwd)-1);
    //用户名密码发送给服务器
    client.sendMessage(&msg,sizeof(msg));
}

int main()
{
    StdTcpSocket client;
    //连接服务器
    const char* serverIp = "192.168.159.131";
    int ret = client.connectToServer(serverIp,8080);
    if(ret!=0)
    {
        cout<<"connect error"<<endl;
        _exit(-1);
    }
    cout <<"连接成功"<<endl;
    sleep(1);
    system("clear");
    int choice;
    while(1)
    {
        interfaceMenu();
        cout<<"请输入选项";
        cin >>choice;
        switch (choice)
        {
        case REGISTER:
            registerFunc(client);
            break;
        case LOGIN:
            loginFunc(client);
            break;
        case EXIT:
        exit(0);
            break;
        default:

            break;

        }
    }
    
    return 0;
}