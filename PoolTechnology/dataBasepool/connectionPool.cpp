#include "connectionPool.h"
#include <fstream>//文件流
#include <jsoncpp/json/json.h>  //json解析库
#include <chrono>//时间库

//提供一个静态方法，让外部调用唯一实例
ConnectionPool * ConnectionPool::getConnectionPool()
{
    static ConnectionPool pool;
    return &pool;
}
//向连接池添加连接
bool ConnectionPool::addConnection()
{
    stdMysqlDataBase * conn = new stdMysqlDataBase();
    //打上时间戳
    conn->refreshAliveTime();
    //连接数据库
    conn->connectDB(m_dbname);
    //添加连接到连接池
    m_connectionQueue.push(conn);

    return true;
}

ConnectionPool::ConnectionPool()
{
    //解析本地持久化数据
    if(parseJsonFile()==false)
    {
        cout << "parseJsonFile failed" << endl;
        return;
    }

    for(int idx=0;idx<m_minSize;idx++)
    {
        addConnection();
    }
    //创建生产者线程
    thread producer(&ConnectionPool::produceConnection,this);
    //创建回收者线程
    thread recycle(&ConnectionPool::recycleConnection,this);
    //分离线程
    producer.detach();
    recycle.detach();
}

//解析本地持久化数据
bool ConnectionPool::parseJsonFile()
{
    ifstream ifs("./config.json");
    if(ifs.is_open()==false)
    {
        cout << "config.json open failed" << endl;
        return false;
    }
    Json::Reader rd;
    Json::Value root;

    if(rd.parse(ifs,root)==false)//解析失败
    {
        cout << "json parse failed" << endl;
        return false;
    }
    m_hostname = root["hostname"].asString();
    m_username = root["username"].asString();
    m_passwd = root["passwd"].asString();
    m_port = root["port"].asInt();
    m_dbname = root["dbname"].asString();
    //最小连接数
    m_minSize = root["minSize"].asInt();
    //最大空闲时间
    m_maxIdleTime = root["maxIdleTime"].asInt();

    return true;

}
//生产连接
void ConnectionPool::produceConnection()
{
    while(1)
    {
        //unique_lock 智能锁;
        unique_lock<mutex> locker(m_mutexQ);
        
        while(m_connectionQueue.size()>=m_minSize)
        {
            m_cond_produce.wait(locker);
        }
        //添加连接到连接池
        addConnection();
        //唤醒消费者
        m_cond_consume.notify_all();//类似于pthread_cond_broadcast

    }
}
//回收连接到连接池
void ConnectionPool::recycleConnection()
{
    while(1)
    {
        //休眠1秒
        this_thread::sleep_for(chrono::seconds(1));
        //加锁;
        std::lock_guard<mutex> lock(m_mutexQ);
        while(m_connectionQueue.size()>m_minSize)
        {
            //跟消费者无关系
            //取出队头
            stdMysqlDataBase * conn = m_connectionQueue.front();
            //当前时间-上次操作的时间
            if(conn->getAliveTime()>m_maxIdleTime)
            {
                m_connectionQueue.pop();
                delete conn;
                conn = nullptr;
            }

        }
    }
}

//从连接池中获取一个mysql连接
shared_ptr<stdMysqlDataBase> ConnectionPool::getConnection()
{
    //1.判断连接池中是否有可用连接
    //2.如果没有可用连接，等待
    //3.如果有可用连接，返回一个连接
    //unique_lock 智能锁;
    std::unique_lock<mutex> locker(m_mutexQ);
    while(m_connectionQueue.empty())
   {
        if(std::cv_status::timeout == m_cond_consume.wait_for(locker,std::chrono::seconds(500)))
        {
            //继续等待
            if(m_connectionQueue.empty())
            {
                continue;
            }
        }
    }

    //程序到了这个地方
    //数据库连接池中有可用连接
    std::shared_ptr<stdMysqlDataBase> connPtr (m_connectionQueue.front(),[this](stdMysqlDataBase* conn){
        //共享资源
        m_mutexQ.lock();
        //更新连接的存活时间
        conn->refreshAliveTime();
        //回收连接到连接池
        m_connectionQueue.push(conn);
        m_mutexQ.unlock();  

    });

    m_connectionQueue.pop();
    m_cond_produce.notify_all();//唤醒生产者
    return connPtr;

}

ConnectionPool::~ConnectionPool()
{

}