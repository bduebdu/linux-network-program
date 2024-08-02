#include<iostream>
using namespace std;
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t g_sem;//信号量

void printer(const char *str)
{
    if(str == NULL)
    {
        return;
    }
    //p操作(信号量-1)
    sem_wait(&g_sem);
    while(*str!='\0')
    {
        cout<<*str;
        fflush(stdout);
        str++;//指针移动
        sleep(1);
    }
    cout<<endl;
    //v操作（信号量+1）
    sem_post(&g_sem);
}

void *thread_func1(void *arg)
{
    //线程分离
    pthread_detach(pthread_self());
    const char * str1 = "hello";
    printer(str1);
    pthread_exit(NULL);
}

void *thread_func2(void *arg)
{
    pthread_detach(pthread_self());
    const char * str2 = "world";
    printer(str2);
    pthread_exit(NULL);
}



int main ()
{
    //初始化
    int initVal = 1;
    sem_init(&g_sem,0,initVal);
    pthread_t tid1;
    int ret = pthread_create(&tid1,NULL,thread_func1,NULL);
    if(ret ==-1)
    {
        perror("thread create error:");
        _exit(-1);
    }
    pthread_t tid2;
    ret = pthread_create(&tid2,NULL,thread_func2,NULL);
    if(ret ==-1)
    {
        perror("thread create error:");
        _exit(-1);
    }
    int cnt =60;
    while(cnt--)
    {
        sleep(1);
    }   

    //销毁信号量
    sem_destroy(&g_sem);

    return 0;
}