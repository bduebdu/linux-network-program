#include "connectionPool.h"
#include "mysqlDataBase.h"
#include <iostream>
using namespace std;


//不使用线程池
void op1()
{
    for(int idx=0;idx<5;idx++)
    {
        stdMysqlDataBase conn;
        conn.connectDB("demoTest");
        const char * sqlStr = "insert into students (name,age,grade) values('shuo',%d,'262');";
        char requestSql[128] = {0};
        sprintf(requestSql,sqlStr,idx);
        conn.execute(requestSql);

    }
}



//使用线程池
void op2(ConnectionPool * pool)
{
    for(int idx=0;idx<5;idx++)
    { 
        std::shared_ptr<stdMysqlDataBase> conn = pool->getConnection();
        cout<<"enter"<<endl;
        const char * sqlStr = "insert into students (name,age,grade) values('shuo',%d,'262');";
        char requestSql[128] = {0};
        sprintf(requestSql,sqlStr,idx);
        conn->execute(requestSql);
    }
    
}
void text()
{
#if 0
    std::chrono::steady_clock::time_point beginTime = std::chrono::steady_clock::now();
    op1();
    std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
    auto length = endTime - beginTime;
    cout<<"runtime:"<<length.count()<<endl;
#else
    ConnectionPool * pool = ConnectionPool::getConnectionPool();
    std::chrono::steady_clock::time_point beginTime = std::chrono::steady_clock::now();
    op2(pool);
    std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
    auto length = endTime - beginTime;
    cout<<"runtime:"<<length.count()<<endl;

#endif
}

int main()
{
    text();
    
    return 0;
}