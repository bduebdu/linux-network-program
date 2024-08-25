#include "mysqlDataBase.h"
#include<vector>
#include<string>
#include<iostream>
using namespace std;

stdMysqlDataBase::stdMysqlDataBase(): m_sqlDB(nullptr)
{

}

stdMysqlDataBase::~stdMysqlDataBase()
{
    closeDB();
}
//连接数据库
bool stdMysqlDataBase::connectDB(const std::string & dbFileName)
{
    m_sqlDB = mysql_init(NULL);
    if (m_sqlDB == nullptr)
    {
        cout<<"falied"<<endl;
        _exit(-1);
    }
    cout <<"初始化成功"<<endl;

    const char * host = "localhost";
    const char * user = "root";
    const char * passwd = "123456";
    int port = 3306;

    if(mysql_real_connect(m_sqlDB,host,user,passwd,dbFileName.c_str(),port,NULL,0)==NULL)
    {
        cout<<"File:"<<__FILE__<<"Line:"<<__LINE__<<"connect error"<<mysql_error(m_sqlDB)<<endl;
        return false;
    }
    cout<<"连接成功"<<endl;
    return true;
}
//执行sql语句
bool stdMysqlDataBase::execute(const std::string sql)
{
    //执行sql

    int ret = mysql_query(m_sqlDB,sql.c_str());
    if(ret != 0)
    {
        cout<<"mysql_query error"<<endl;
        return false;
    }
    return true;
}
//查询sql语句
std::vector<vector<string>> stdMysqlDataBase::query(const std::string sql)
{
    vector<vector<string>> Data;

    if(execute(sql)==false)//执行sql语句
    {
        return Data;
    }

    //结果集
    MYSQL_RES * result = mysql_store_result(m_sqlDB);
    if(result == NULL)
    {
        cout<<"mysql_store_result error."<<endl;
        exit(-1);
    }

    //获取结果集的列数
    unsigned int cols = mysql_num_fields(result);

    vector<string> rowData;
    //获取数据
    MYSQL_ROW row;
    while((row = mysql_fetch_row(result))!=NULL)
    {
        //每个row变量表示一行
        for(int col = 0;col<cols;col++)
        {
            rowData.push_back(row[col]);
        }
        Data.push_back(rowData);
    }

    //释放结果集
    mysql_free_result(result);
    return Data;
}

//关闭数据库
void stdMysqlDataBase::closeDB()
{
    if(m_sqlDB!= NULL)
    {
       mysql_close(m_sqlDB); 
       m_sqlDB = NULL;
    }
    
};

//刷新存活时间
void stdMysqlDataBase::refreshAliveTime()
{
    m_backupCurrentTime = std::chrono::steady_clock::now();//获取当前时间

}
//获取存活时间
long stdMysqlDataBase::getAliveTime()
{
    //返回当前时间减去备份时间
    std::chrono::nanoseconds res = std::chrono::steady_clock::now() - m_backupCurrentTime;
    //精度转换
    std::chrono::seconds sec = std::chrono::duration_cast<std::chrono::seconds>(res);

    return sec.count();
}