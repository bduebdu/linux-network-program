#include <iostream>
using namespace std;
#include "stdTcpServer.h"
#include <unistd.h>
#include <pthread.h>
#include <cstring>
#include "stdShared.h"
#include "function.h"
#include "messageHandle.h"
#include "threadpool.h"
#include "sqliteDataBase.h"
#include <json-c/json.h>

#define BUFFER_SIZE 1024

void * handleClientInfo(void * arg)
{
    /* 线程分离 */
    pthread_detach(pthread_self());

    StdTcpSocketPtr clientInfo = *static_cast<StdTcpSocketPtr*>(arg);

    int readBytes = 0;
    
    MessageHandle handles(clientInfo);

    string buffer;
    while (1)
    {
        readBytes = clientInfo->recvMessage(buffer);
        if (readBytes <= 0)
        {
            cout << "readBytes <= 0" <<endl;
            break;
        }
        else
        {
            /* 客户端有数据过来 */
            handles.handleMessage(buffer);//执行注册方法
        }
        buffer.clear();//清空缓冲区
    }
 
    /* 资源回收 */

    /* 线程退出 */
    return(NULL);
}

int main()
{
    /* 创建线程池对象 */
    ThreadPool pool(3, 8, 20);

    /* 创建服务器对象 */
    StdTcpServer server;

    /* 设置监听 */
    bool res = server.setListen(8080);
    if (res == false)
    {
        cout << "listen error" << endl;
        _exit(-1);
    }
    cout << "server listening..." << endl;

    int ret = 0;
    while (1)
    {
        StdTcpSocketPtr clientInfo = server.getClientSock();

#if 0
        pthread_t tid;
        ret = pthread_create(&tid, NULL, handleClientInfo, &clientInfo);
        if (ret != 0)
        {
            perror("thread create error:");
            _exit(-1);
        }
#else
        pool.addTask(handleClientInfo, new StdTcpSocketPtr(clientInfo));
#endif

        /* 休眠一下 */
        sleep(1);
    }
}