#ifndef __THREADPOOL_H_
#define __THREADPOOL_H_

#include<iostream>
#include<pthread.h>

struct Task_t
{
    //回调函数
    void* (*function)(void *arg);
    //回调函数参数
    void * arg;
};


class ThreadPool
{
public:
    ThreadPool(int minThreads,int maxThreads,int maxQueueCapacity);
    ~ThreadPool();



public:
    void addTask(void*(*function)(void* arg),void*arg);//添加任务


// private:
    int m_queueCapacity;//队列容量
    int m_queueSize;//队列任务个数
    Task_t* m_queue;//队列
    int m_queueFront;
    int m_queueTail;//队列头尾

    //工作的线程
    pthread_t * m_threads;
    //管理者线程
    pthread_t manageThread;
    //忙碌的线程数量
    int m_busyThreadNums;
    //最大的线程数
    int m_maxThreadNums;
    //最小的线程数
    int m_minThreadNums;
    //存活的线程数
    int m_aliveThreadNums;

public:

    pthread_cond_t m_queueNotEmpty;
    pthread_cond_t  m_queueNotFull;    
    pthread_mutex_t m_mutex;//锁1
    pthread_mutex_t m_busyMutex;//锁2减少竞争，提高效率






};






#endif