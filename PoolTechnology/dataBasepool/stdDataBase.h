#ifndef __STDDATABASE_H__
#define __STDDATABASE_H__

#include<string>
#include<vector>
#include<iostream>
using namespace std;


class stdDataBase
{
public:

    //虚析构
    virtual ~stdDataBase() = default;

public:
    //连接数据库
    virtual bool connectDB(const std::string & dbFileName) = 0;
    //执行sql语句
    virtual bool execute(const std::string sql) = 0;
    //查询sql语句
    virtual vector<vector<string>> query(const std::string sql) = 0;
    //关闭数据库
    virtual void closeDB() = 0;

};















#endif