#include <iostream>
using namespace std;
#include "mysqlDataBase.h"

int main ()
{
    stdMysqlDataBase mySqlDB;
    //连接数据库
    if(mySqlDB.connectDB("demoTest")==false)
    {
        cout<<"connect error"<<endl;
        exit(-1);
    }

    std::string sql = "insert into students (name,age,grade) values ('yuan',25,'262class');";

    mySqlDB.execute(sql);

    mySqlDB.closeDB();

    return 0;
}