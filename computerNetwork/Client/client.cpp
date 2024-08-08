#include <iostream>
using namespace std;
#include "stdTcpServer.h"
#include "stdShared.h"
#include <unistd.h>
#include <string.h>

void interfaceMenu()
{
    cout << "1.注册" << endl;
    cout << "2.登陆" << endl;
    cout << "3.退出" << endl;
}

void registerfunc(StdTcpSocket &client )
{
    string username;
    cout << "请输入用户名";
    fflush(stdout);
    cin >>username;

    string passwd;
    cout << "请输入密码";
    fflush(stdout);
    cin >>passwd;

    Msg msg;
    memset(&msg, 0, sizeof(msg));

    msg.type = REGISTER;
    strncpy(msg.name, username.c_str(), sizeof(msg.name)- 1);
    strncpy(msg.passwd, passwd.c_str(), sizeof(msg.passwd)- 1);      
    //用户名密码发送给服务器
    client.sendMessage(&msg, sizeof(msg));

    char buffer[128] = {0};
    client.recvMessage(buffer,sizeof(buffer));
    cout<<"buffer:"<<buffer<<endl;
    sleep(1);
}
void loginfunc(StdTcpSocket &client)
{
    string username;
    cout << "请输入用户名";
    fflush(stdout);
    cin >>username;

    string passwd;
    cout << "请输入密码";
    fflush(stdout);
    cin >>passwd;
    Msg msg;
    memset(&msg, 0, sizeof(msg));

    msg.type = LOGIN;
    strncpy(msg.name, username.c_str(), sizeof(msg.name)- 1);
    strncpy(msg.passwd, passwd.c_str(), sizeof(msg.passwd)- 1);      
    //用户名密码发送给服务器
    client.sendMessage(&msg, sizeof(msg));
    sleep(1);
}

int main()
{
    StdTcpSocket client;
    // 连接服务器
    const char *serverIp = "192.168.159.131";
    int ret = client.connectToServer(serverIp, 8080);
    if (ret != 0)
    {
        perror("connect failed");
        _exit(-1);
    }
    cout << "连接成功" << endl;
    sleep(1);
    system("clear");
    int choice;
    while (1)
    {
        interfaceMenu();
        cout << "请输入选项";
        cin >> choice;
        switch (choice)
        {
        case (REGISTER):
            registerfunc(client);
            break;
        case (LOGIN):     
            loginfunc(client);
            break;
        case (EXIT):
            exit(0);
            break;
        default:
            break;
        }
    }
    return 0;
}
