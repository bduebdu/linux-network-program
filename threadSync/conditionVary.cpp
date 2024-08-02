#include <iostream>
using namespace std;
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>


typedef struct node//节点结构
{
    int data;
    struct node* next;
}Node;

Node *head = NULL;//指向链表头部指针

pthread_mutex_t mutex;//互斥锁

pthread_cond_t cond;//条件变量

void* producer(void* arg)//生产者
{
    while(1)
    {
        sleep(3);
        //创建链表节点
        Node* newNode = (Node*)malloc(sizeof(Node));
        //初始化节点
        newNode->data = rand()%1000 +1;
        pthread_mutex_lock(&mutex);//互斥锁保护
        newNode->next = head;//指针域
        head = newNode;
        printf("===== produce:%lu,%d\n",pthread_self(),newNode->data);\
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);//通知阻塞的消费者线程，解除阻塞
        sleep(rand()%3);
    }


    return NULL;
}

void * customer(void *arg)
{
    while(1)
    {    
        //加锁
        pthread_mutex_lock(&mutex);
        while (head == NULL)
        {
            //无数据
            //等待条件变量的过程中，解锁互斥锁
            pthread_cond_wait(&cond,&mutex);
            //接受到条件变量后，解除阻塞，再对互斥锁重新加锁
        }
        Node * eatNode = head;
        head = head->next;

        pthread_mutex_unlock(&mutex);//解锁

        printf("====== consume: %lu,%d\n",pthread_self(),eatNode->data);
        //释放内存
        if(eatNode)
        {
            free(eatNode);
            eatNode ==NULL;
        }
    }    
}

int main(int argc,const char *argv[])
{
    //生成随机数种子
    srand(static_cast<unsigned int> (time(NULL)));
    pthread_t produce,consume;
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    pthread_create(&produce,NULL,producer,NULL);//生产者线程
    pthread_create(&consume,NULL,customer,NULL);//消费者线程
    pthread_join(produce,NULL);//阻塞回收子线程
    pthread_join(consume,NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;

}