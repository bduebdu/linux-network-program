#include <iostream>
using namespace std;
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>


pthread_rwlock_t g_rwlock;//初始化读写锁
int g_num = 100;
void*threadFunc1(void*arg)//线程函数1
{
    sleep(1);
    pthread_rwlock_rdlock(&g_rwlock);//读锁
    for(int idx = 0;idx<10;idx++)
    {
        cout<<"threadFunc1 g_num:"<<g_num<<endl;
        usleep(1000);
    }

    //解锁
    pthread_rwlock_unlock(&g_rwlock);
    pthread_exit(NULL);
}

void*threadFunc2(void*arg)//线程函数2
{
    sleep(1);
    pthread_rwlock_rdlock(&g_rwlock);//读锁
    for(int idx = 0;idx<10;idx++)
    {
        cout<<"threadFunc2 g_num:"<<g_num<<endl;
        usleep(1000);
    }

    //解锁
    pthread_rwlock_unlock(&g_rwlock);
    pthread_exit(NULL);
}

void*threadFunc3(void*arg)//线程函数3
{
    //写锁(其他线程不能进行读写操作，即拿不到读写锁)
    pthread_rwlock_wrlock(&g_rwlock);
    sleep(10);
    g_num+=666;

    pthread_rwlock_unlock(&g_rwlock);
    pthread_exit(NULL);
}

void*threadFunc4(void*arg)//线程函数4
{


    pthread_exit(NULL);
}

int main()
{
    
    pthread_rwlock_init(&g_rwlock,NULL);

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
    pthread_t threadId3;
    ret = pthread_create(&threadId3,NULL,threadFunc3,NULL);
    if(ret!=0)
    {
        perror("setdetachstate error");
        _exit(-1);
    }
    pthread_t threadId4;
    ret = pthread_create(&threadId4,NULL,threadFunc4,NULL);
    if(ret!=0)
    {
        perror("setdetachstate error");
        _exit(-1);
    }
    //回收资源
    pthread_join(threadId1,NULL);
    pthread_join(threadId2,NULL);
    pthread_join(threadId3,NULL);
    pthread_join(threadId4,NULL);


    //释放读写锁
    pthread_rwlock_destroy(&g_rwlock);

    return 0;
}