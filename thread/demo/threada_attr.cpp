#include <iostream>
using namespace std;
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
//线程依托于进程
void* thread_func(void* arg)
{
    int cnt =10;
    while(cnt--)
    {
        cout<<"hello world"<<endl;
        sleep(1);
    }
    pthread_exit(NULL);//退出线程
}

int main()
{
    //初始化线程状态
    pthread_attr_t threadAattr;
    pthread_attr_init(&threadAattr);
    //设置线程的分离状态
    int ret = pthread_attr_setdetachstate(&threadAattr,PTHREAD_CREATE_DETACHED);
    if(ret!=0)
    {
        perror("setdetachstate error");
        _exit(-1);
    }

    pthread_t threadId;
    ret = pthread_create(&threadId,&threadAattr,thread_func,NULL);
    if(ret!=0)
    {
        perror("create thread error");
        _exit(-1);
    }

    pthread_attr_destroy(&threadAattr);
    while(1)
    {
        sleep(1);
    }
    return 0;
}