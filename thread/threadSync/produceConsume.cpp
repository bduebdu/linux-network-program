#include <iostream>
using namespace std;
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctime>
#include <stdlib.h>

pthread_mutex_t g_mutex;
pthread_cond_t g_cond;
struct Node//链表
{
    int data;
    Node * next;
};

Node * g_head = NULL;//链表头节点


void * handleProduce(void* arg)
{
    while(true)
    {
        int random = rand()%100+1;
        Node *newNode = static_cast<Node *>(malloc(sizeof(Node)*1));
        newNode->data = random;
        //上锁
        pthread_mutex_lock(&g_mutex);
        //头插
        newNode->next = g_head;
        g_head = newNode;
        //解锁
        pthread_mutex_unlock(&g_mutex);//解锁
        pthread_cond_signal(&g_cond);//发信号
        usleep(1000);


    }
}

void * handleConsume(void* arg)
{
    
}

int main()
{
    srand(static_cast<unsigned int>(time(NULL)));//随机数
    pthread_mutex_init(&g_mutex,NULL);
    pthread_cond_init(&g_cond,NULL);//新建锁和条件变量

    pthread_t produceId;//生产者线程
    int ret = pthread_create(&produceId,NULL,handleProduce,NULL);
    if(ret!=0)
    {
        perror("thread create error");
        _exit(-1);
    }
    pthread_t consumeId;//消费者线程
    ret = pthread_create(&produceId,NULL,handleConsume,NULL);
    if(ret!=0)
    {
        perror("thread create error");
        _exit(-1);
    }

    //回收资源
    pthread_join(produceId,NULL);
    pthread_join(consumeId,NULL);

    pthread_mutex_destroy(&g_mutex);
    pthread_cond_destroy(&g_cond);
    return 0;
}