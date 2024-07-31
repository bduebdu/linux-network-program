#include <iostream>
using namespace std;
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

int g_money = 100;//全局
//线程资源共享
void* handleThread1(void *arg)
{
    pthread_detach(pthread_self());//线程分离（操作系统回收）
    int num =*(int*)arg;
    while(g_money>0)
    {
        sleep(1);
        g_money-=1;
        printf("thread1 g_money = %d\n",g_money);
        
    }
    #if 0
    int num = 1;
    while (1)
    {
        printf("num=%d\n",num);
        num+=2;
        sleep(2);
    }
    #endif
    pthread_exit(NULL);
}

void* handleThread2(void *arg)
{
    pthread_detach(pthread_self());
    int num = 0;
    while (g_money>0)
    {
        sleep(1);
        g_money-=1;
        printf("thread2 g_money = %d\n",g_money);
        
    }
    pthread_exit(NULL);
}


int main()
{
    #if 0
    //线程号id
    pthread_t threadId = pthread_self();
    cout<< "threadid;"<<threadId<<endl;
    //进程号id 
    pid_t pidId = getpid();
    cout<< "pidid;"<<pidId<<endl;
    #endif

    //创建线程

        pthread_t threadId1;
        int num1=100;
        int ret = pthread_create(&threadId1,NULL,handleThread1,&num1);
        if(ret!=0)
        {
            perror("create thread error");
            exit(-1);
        }


        pthread_t threadId2;
        ret = pthread_create(&threadId2,NULL,handleThread2,NULL);
        if(ret!=0)
        {
            perror("create thread error");
            exit(-1);
        }
   
    #if 0 
    //回收资源
    //线程号被分离时无法回收资源
    pthread_join(threadId1,NULL);
    pthread_join(threadId2,NULL);
    #endif

    int cnt =100;
    while (cnt--)
    {
        
        sleep(1);
        if(cnt==95)
        {
            pthread_cancel(threadId2);
        }
    }

    return 0;
}