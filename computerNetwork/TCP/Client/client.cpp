#include <iostream>
using namespace std;
#include "stdTcpServer.h"
#include "stdShared.h"
#include <unistd.h>
#include <string.h>
#include "chatroom.h"
void interfaceMenu()
{
    cout << "1.注册" << endl;
    cout << "2.登陆" << endl;
    cout << "3.退出" << endl;
}

void registerfunc(StdTcpSocketPtr &client )
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
    client->sendMessage(&msg, sizeof(msg));

    ReplyMsg recvMsg;
    client->recvMessage((void*)&recvMsg,sizeof(recvMsg));
    if(recvMsg.ststic_code == REGISTER_SUCCESS)
    {
        cout << "注册成功"<< endl;
    }
    else if(recvMsg.ststic_code == REGISTER_USEREXIST)
    {
        cout << "重复注册(已经存在用户)"<< endl;
    }

    sleep(1);
}
void loginfunc(StdTcpSocketPtr &client)
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
    client->sendMessage(&msg, sizeof(msg));

    ReplyMsg recvMsg;
    client->recvMessage((void*)&recvMsg,sizeof(recvMsg));
    if(recvMsg.ststic_code == LOGIN_SUCCESS)
    {
        cout << "登陆成功"<< endl;
        sleep(2);
        ChatRoom chatroom(client);
        chatroom.ChatEnter();
    }
    else if(recvMsg.ststic_code == LOGIN_PASSWD_ERROR)
    {
        cout << "密码错误"<< endl;
    }
    else if(recvMsg.ststic_code == LOGIN_NOUSER)
    {
        cout << "无用户"<< endl;
    }
    sleep(1);
}     

int main()
{
    StdTcpSocketPtr client = std::make_shared<StdTcpSocket>();
    // 连接服务器
    const char *serverIp = "192.168.159.131";
    int ret = client->connectToServer(serverIp, 8080);
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
