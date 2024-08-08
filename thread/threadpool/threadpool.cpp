#include"threadpool.h"
#include<cstring>
#include<unistd.h>
#define DEFAULT_MIN_THREADS 3
#define DEFAULT_MAX_THREADS 8
//管理者线程函数
void * manageThreadFunc(void *arg)
{

}
void * workerThreadFunc(void *arg)//工作者线程
{
    pthread_detach(pthread_self());

    ThreadPool pool = *(ThreadPool *)arg;

    //去队列中取任务处理
    
    while(true)
    {
        pthread_mutex_lock(&(pool.m_mutex));//加锁
        while (pool.m_queueSize == 0)
        {
            pthread_cond_wait(&(pool.m_queueNotEmpty),&(pool.m_mutex));
        }

    //从队列取数据
    Task_t newTask;
    newTask.function = pool.m_queue[pool.m_queueFront].function;
    newTask.arg = pool.m_queue[pool.m_queueFront].arg;

    //移动队头位置
    pool.m_queueFront = (pool.m_queueFront + 1) % pool.m_queueCapacity;
    //任务队列的任务数-1
    pool.m_queueSize--;

    pthread_mutex_unlock(&(pool.m_mutex));//解锁

    pthread_mutex_lock(&(pool.m_busyMutex));//上锁2

    //忙碌线程增加
    pool.m_busyThreadNums++;
    pthread_mutex_unlock(&(pool.m_busyMutex));//解锁2
    //执行任务
    newTask.function(newTask.arg);
    //忙碌线程减少
    pthread_mutex_lock(&(pool.m_busyMutex));//上锁2
    pool.m_busyThreadNums--;
    pthread_mutex_unlock(&(pool.m_busyMutex));//解锁2
    pthread_cond_signal(&(pool.m_queueNotFull));//发信号

    }

}

ThreadPool::ThreadPool(int minThreads,int maxThreads,int maxQueueCapacity)//构造
{


    if(minThreads <= 0 || maxThreads<=0)
    {
        minThreads = DEFAULT_MIN_THREADS;
        maxThreads = DEFAULT_MAX_THREADS;
    }
    this->m_minThreadNums = minThreads;
    this->m_maxThreadNums = maxThreads;
    this->m_queueCapacity = maxQueueCapacity;

    //任务队列初始化
    this->m_queue = new Task_t[this->m_queueCapacity];
    if(this->m_queue == NULL)
    {
        perror("new error.");
        _exit(-1);//退出线程
    }
    //清空脏数据
    memset(this->m_queue,0,sizeof(Task_t)*this->m_queueCapacity);
    
    this->m_threads = new pthread_t[this->m_maxThreadNums];
    if(this->m_threads == NULL)
    {
        perror("new thread error.");
        _exit(-1);//退出线程   
    }
    //清空脏数据
    memset(this->m_threads,0,sizeof(pthread_t)* this->m_maxThreadNums);
    //队列属性
    this->m_queueSize = 0;
    this->m_queueFront = 0;
    this->m_queueTail = 0;



    //启动管理者线程
    int ret = pthread_create(&(this->manageThread),NULL,manageThreadFunc,this);
    if(ret == -1)
    {
        perror("threead create error.");
        _exit(-1);//退出线程  
    }

    this->m_busyThreadNums = 0;//
    this->m_aliveThreadNums = 0;//
    for(int idx =1;idx < minThreads;idx++)
    {
        ret = pthread_create(&(this->m_threads[idx]),NULL,workerThreadFunc,this);
        if(ret == -1)
        {
            perror("threead create error.");
            _exit(-1);//退出线程  
        }    
        this->m_aliveThreadNums ++;
    }

    //初始化锁
    pthread_mutex_init(&m_mutex,NULL);
    pthread_mutex_init(&m_busyMutex,NULL);
    pthread_cond_init(&m_queueNotEmpty,NULL);//条件变量
    pthread_cond_init(&m_queueNotFull,NULL);

}

void ThreadPool::addTask(void*(*function)(void* arg),void*arg)//添加任务
{
    //任务添加到公共队列
    //上锁
    pthread_mutex_lock(&(this->m_mutex));
    while(this->m_queueSize == this->m_queueCapacity)
    {
        pthread_cond_wait(&(this->m_queueNotFull),&(this->m_mutex));
    }
    this->m_queue[this->m_queueTail].function =function;
    this->m_queue[this->m_queueTail].arg =arg;
    this->m_queueTail = (this->m_queueTail +1)% this->m_queueCapacity;
    //任务数量+1
    this->m_queueSize++;

    //解锁
    pthread_mutex_unlock(&(this->m_mutex));
    //发信号
    pthread_cond_signal(&this->m_queueNotEmpty);
}

ThreadPool::~ThreadPool()//析构
{
    
}