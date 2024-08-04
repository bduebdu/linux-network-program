#include <iostream>
using namespace std;
#include <unistd.h>
#include <pthread.h>
#include "stdTcpServer.h"
#include <cstring>
#include "stdShared.h"
#include "function.h"
#define BUFFER_SIZE 1024

void *handleClientInfo(void *arg)
{
    // 线程分离
    pthread_detach(pthread_self());
    StdTcpSocketPtr clientInfo = *(StdTcpSocketPtr *)arg;
    int readBytes =0;
    Msg msg;
    memset(&msg,0,sizeof(msg));
    while (1)
    {
        int readbytes = clientInfo->recvMessage(&msg, sizeof(msg));
        if (readbytes <= 0)
        {
            cout << "readBytes <=0" << endl;
            break;
        }
        else
        {
            cout<<"msg.type:"<<msg.type<<endl;
            if(msg.type==REGISTER)
            {
                Funtion::handleRegisterInfo(msg.name,msg.passwd);
            }
            else if (msg.type==LOGIN)
            {
                Funtion::handleLoginInfo(msg.name,msg.passwd);
            }
            
            cout<<"msg.name:"<<msg.name<<endl;
            cout<<"msg.passwd:"<<msg.passwd<<endl;
        }
        memset(&msg,0,sizeof(msg));
    }
    
    pthread_exit(NULL);
}

int main()
{
    StdTcpServer server;

    bool res = server.setListen(8080);

    if (res == false)
    {
        cout << "listen error" << endl;
        _exit(-1);
    }

    while (1)
    {
        StdTcpSocketPtr clientInfo = server.getClientSock();
        pthread_t tid;
        int ret =0;
        ret = pthread_create(&tid, NULL, handleClientInfo, &clientInfo);
        if(ret!=0)
        {
            perror("listen error");
            _exit(-1);
        }
        sleep(1);
    }
}