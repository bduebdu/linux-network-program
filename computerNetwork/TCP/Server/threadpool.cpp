#include "threadpool.h"
#include <cstring>
#include <unistd.h>
#define DEFAULT_MIN_THREADS 3
#define DEFAULT_MAX_THREADS 8
#define INCREASE_NUMS 2
#define DECREASE_NUMS 2
#include <pthread.h>
#include <signal.h>

void *workerThreadFunc(void *arg);
// 判断线程是否存活
bool threadIsAlive(pthread_t tid)
{
    int res = pthread_kill(tid, 0);
    if (res == ESRCH)
    {
        return false;
    }
    return true;
}
void exitThread(ThreadPool *pool, pthread_t tid)
{
    for (int idx = 0; idx < pool->m_maxThreadNums; idx++)
    {
        if (pool->m_threads[idx] == tid)
        {
            pool->m_threads[idx] = 0;
        pthread_exit(NULL);
        }
    }
}

/* 管理者线程函数 */
void *manageThreadFunc(void *arg)
{
    // pthread_exit(NULL);
    pthread_detach(pthread_self());

    ThreadPool *pool = (ThreadPool *)arg;

    while (true)
    {
        sleep(1);

        // 加🔓
        // 取出任务队列的任务个数
        // 取出存活的线程数
        pthread_mutex_lock(&(pool->m_mutex));
        int queueSize = pool->m_queueSize;
        int aliveThreadNum = pool->m_aliveThreadNums;
        pthread_mutex_unlock(&(pool->m_mutex));

        // 取出忙碌的线程数
        pthread_mutex_lock(&(pool->m_busyMutex));
        int busyThreadNums = pool->m_busyThreadNums;
        pthread_mutex_unlock(&(pool->m_busyMutex));

       // printf("size:%d, alive:%d, busy:%d.\n", queueSize, aliveThreadNum, busyThreadNums);

        // 任务队列任务数 大于活的线程数，开辟新空间
        int addCnt = 0;
        int ret = 0;
        if ((queueSize > (aliveThreadNum - busyThreadNums)) && aliveThreadNum < pool->m_maxThreadNums)
        {
            // 加🔓
            pthread_mutex_lock(&(pool->m_mutex));
            for (int idx = 0; idx < (pool->m_maxThreadNums) && addCnt < INCREASE_NUMS; idx++)
            {
                // 探测
                if (pool->m_threads[idx] == 0)
                {
                    ret = pthread_create(&(pool->m_threads[idx]), NULL, workerThreadFunc, pool);
                    if (ret != 0)
                    {
                        perror("thread create error");
                        throw std::runtime_error("thread_create error");
                    }
                    printf("%ld join thread pool", pool->m_threads[idx]);

                    addCnt++;
                    // 活着的线程数加一
                    (pool->m_aliveThreadNums)++;
                }
            }
            pthread_mutex_unlock(&(pool->m_mutex));
        }
        // 什么时候需要减少线程池的线程数量
        // 任务队列的任务数 远<于 存活（未工作）的线程数

        if ((queueSize  < (aliveThreadNum -busyThreadNums)) && aliveThreadNum > pool->m_minThreadNums)
        {
            // 减少线程
            pthread_mutex_lock(&(pool->m_mutex));

            pool->m_exitNum = DECREASE_NUMS;

            for (int idx = 0; idx < DECREASE_NUMS; idx++)
            {
                pthread_cond_signal(&(pool->m_queueNotEmpty));
            }

            pthread_mutex_unlock(&(pool->m_mutex));
        }
    }
}

/* 工作者线程 */
void *workerThreadFunc(void *arg)
{
    /* 线程分离 */
    pthread_detach(pthread_self());

    ThreadPool *pool = (ThreadPool *)arg;

    /* 核心目的: 队列里面取任务 处理 */
    while (true)
    {
        pthread_mutex_lock(&(pool->m_mutex));

        while (pool->m_queueSize == 0)
        {
            // 休眠
            usleep(10000);
            pthread_cond_wait(&(pool->m_queueNotEmpty), &(pool->m_mutex));

            if (pool->m_exitNum > 0)
            {
                (pool->m_exitNum)--;

                if (pool->m_aliveThreadNums > pool->m_minThreadNums)
                {

                    // 存活的线程数减少1
                    (pool->m_aliveThreadNums)--;

                    pthread_mutex_unlock(&(pool->m_mutex));

                    printf("%ld exit  thread pool.\n", pthread_self());

                    exitThread(pool, pthread_self());
                    // pthread_exit(NULL);
                }
            }
        }

        /* 从任务队列里面取数据 */
        Task_t newTask;
        newTask.function = pool->m_queue[pool->m_queueFront].function;
        newTask.arg = pool->m_queue[pool->m_queueFront].arg;

        /* 移动队头的位置 */
        pool->m_queueFront = (pool->m_queueFront + 1) % pool->m_queueCapacity;
        /* 任务队列的任务数减一. */
        (pool->m_queueSize)--;

        /* 解锁 */
        pthread_mutex_unlock(&(pool->m_mutex));

        pthread_mutex_lock(&(pool->m_busyMutex));
        /* 忙碌的线程数 */
        (pool->m_busyThreadNums)++;
        pthread_mutex_unlock(&(pool->m_busyMutex));

        /* 处理任务 */
        newTask.function(newTask.arg);

        pthread_mutex_lock(&(pool->m_busyMutex));
        /* 忙碌的线程数 */
        (pool->m_busyThreadNums)--;
        pthread_mutex_unlock(&(pool->m_busyMutex));
        /* 发信号 */
        pthread_cond_signal(&(pool->m_queueNotFull));
    }
}

/* 构造函数 */
ThreadPool::ThreadPool(int minThreads, int maxThreads, int maxQueueCapacity)
{
    if (minThreads <= 0 || maxThreads <= 0)
    {
        minThreads = DEFAULT_MIN_THREADS;
        maxThreads = DEFAULT_MAX_THREADS;
    }
    this->m_minThreadNums = minThreads;
    this->m_maxThreadNums = maxThreads;
    this->m_queueCapacity = maxQueueCapacity;

    /* 任务队列初始化 */
    this->m_queue = new Task_t[this->m_queueCapacity];
    if (this->m_queue == NULL)
    {
        perror("new queue error.");
        /* 进程退出 */
        _exit(-1);
    }
    /* 清空脏数据 */
    memset(this->m_queue, 0, sizeof(Task_t) * this->m_queueCapacity);
    /* 队列的属性 */
    this->m_queueSize = 0;
    this->m_queueFront = 0;
    this->m_queueTail = 0;

    /* 线程初始化 */
    this->m_threads = new pthread_t[this->m_maxThreadNums];
    if (this->m_threads == NULL)
    {
        perror("new thread error.");
        /* 进程退出 */
        _exit(-1);
    }
    /* 清空脏数据 */
    memset(this->m_threads, 0, sizeof(pthread_t) * this->m_maxThreadNums);

    /* 启动管理者线程 */
    int ret = pthread_create(&(this->manageThread), NULL, manageThreadFunc, this);
    if (ret == -1)
    {
        perror("thread create error.");
        _exit(-1);
    }

    /* 忙碌的线程数 */
    this->m_busyThreadNums = 0;
    /* 存活的线程数 */
    this->m_aliveThreadNums = 0;
    for (int idx = 0; idx < minThreads; idx++)
    {
        int ret = pthread_create(&(this->m_threads[idx]), NULL, workerThreadFunc, this);
        if (ret == -1)
        {
            perror("thread create error.");
            _exit(-1);
        }
        /* 存活的线程数加一. */
        this->m_aliveThreadNums++;
    }

    /* 锁初始化 */
    pthread_mutex_init(&m_mutex, NULL);
    pthread_mutex_init(&m_busyMutex, NULL);
    /* 初始化条件变量 */
    pthread_cond_init(&m_queueNotEmpty, NULL);
    pthread_cond_init(&m_queueNotFull, NULL);

    // 初始化杀掉的线程数
    this->m_exitNum = 0;
}

/* 添加任务 */
void ThreadPool::addTask(void *(*function)(void *arg), void *arg)
{
    /* 将任务添加到任务队列 */

    /* 上锁 */
    pthread_mutex_lock(&(this->m_mutex));

    while (this->m_queueSize == this->m_queueCapacity)
    {
        pthread_cond_wait(&(this->m_queueNotFull), &(this->m_mutex));
    }

    this->m_queue[this->m_queueTail].function = function;
    this->m_queue[this->m_queueTail].arg = arg;

    this->m_queueTail = (this->m_queueTail + 1) % this->m_queueCapacity;
    /* 任务数量加一 */
    this->m_queueSize++;

    /* 解锁 */
    pthread_mutex_unlock(&(this->m_mutex));
    /* 发信号 */
    pthread_cond_signal(&(this->m_queueNotEmpty));
}

/* 析构函数 */
ThreadPool::~ThreadPool()
{
}
