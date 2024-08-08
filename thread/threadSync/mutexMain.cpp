#include <iostream>
using namespace std;
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>

//初始化锁
pthread_mutex_t mutex;

void painter(const char *str)
{
    if(str==NULL)
    {
        return;
    }
    while(*str!='\0')
    {
        cout<<*str;
        fflush(stdout);
        sleep(1);
        str++;
    }
    cout<<endl;
}

void*threadFunc1(void*arg)//线程函数1
{
    const char *str1 = "hello";
    pthread_mutex_lock(&mutex);//加锁
    painter(str1);
    pthread_mutex_unlock(&mutex);//解锁
    pthread_exit(NULL);
}

void*threadFunc2(void*arg)//线程函数2
{
    const char *str2 = "world";
    pthread_mutex_lock(&mutex);//加锁
    painter(str2);
    pthread_mutex_unlock(&mutex);//解锁
    pthread_exit(NULL);
}

int main()
{
    pthread_mutex_init(&mutex,NULL);
    //创建线程1
    pthread_t threadId1;
    int ret = pthread_create(&threadId1,NULL,threadFunc1,NULL);
    if(ret!=0)
    {
        perror("setdetachstate error");
        _exit(-1);
    }

    //创建线程2
    pthread_t threadId2;
    ret = pthread_create(&threadId2,NULL,threadFunc2,NULL);
    if(ret!=0)
    {
        perror("setdetachstate error");
        _exit(-1);
    }
    //回收资源
    pthread_join(threadId1,NULL);
    pthread_join(threadId2,NULL);
    cout<<"i am main thread"<<endl;

    pthread_mutex_destroy(&mutex);
    return 0;
}