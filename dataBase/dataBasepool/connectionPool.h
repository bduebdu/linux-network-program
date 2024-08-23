#ifndef __CONNECTIONPOOL_H__
#define __CONNECTIONPOOL_H__

#include "mysqlDataBase.h"
#include <memory>
#include <iostream>
#include "stdDataBase.h"
#include <queue>
using namespace std;

#include <thread>//线程类
#include <mutex>//互斥锁
#include <condition_variable>//条件变量

class ConnectionPool 
{

public:
    //提供一个静态方法，让外部调用唯一实例
    static ConnectionPool * getConnectionPool();
    //删除拷贝构造函数
    ConnectionPool(const ConnectionPool &obj) = delete;
    //删除赋值构造函数
    ConnectionPool & operator=(const ConnectionPool &obj) = delete;    
    //析构函数
    ~ConnectionPool();

public:
    //获取一个空闲连接
    shared_ptr<stdDataBase> getConnection();

private:

    //构造函数私有化(单例)
    ConnectionPool();
    //向连接池添加连接
    bool addConnection();
    //生产连接
    void produceConnection();
    //回收连接到连接池
    void recycleConnection();
    //解析本地持久化数据
    bool parseJsonFile();
    //初始化连接池
    queue<stdMysqlDataBase *> m_connectionQueue;
    //互斥锁
    mutex m_muteQ;
    //条件变量
    condition_variable m_cond_produce;//生产者条件变量(连接池中的连接不够时)
    condition_variable m_cond_consume;//消费者条件变量（连接池有多余连接的时候）
    //配置连接数据库的信息
    string m_hostname;
    string m_username;
    string m_passwd;
    int m_port;
    string m_dbname;
    int m_minSize;

};




#endif // CONNECTIONPOOL_H