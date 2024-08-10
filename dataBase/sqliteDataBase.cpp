#include "sqliteDataBase.h"

stdsqliteDataBase::stdsqliteDataBase() : m_db(nullptr)
{

}
stdsqliteDataBase::~stdsqliteDataBase()
{

}

bool stdsqliteDataBase::connectDB(const std::string & dbFileName)
{
    //连接数据库
    int ret = sqlite3_open(dbFileName.c_str(),&m_db);
    if(ret != SQLITE_OK)
    {
        cout<<"sqlite3 open error"<<sqlite3_errmsg(m_db)<<endl;
        return false;
    }
}
//执行sql语句
bool stdsqliteDataBase::execute(const std::string sql)
{
    int ret = sqlite3_exec(m_db,sql.c_str(),NULL,NULL,NULL);
    if(ret != SQLITE_OK)
    {
        cout<<"sqlite3 exec error"<<sqlite3_errmsg(m_db)<<endl;
        return false;
    }
    return true;
}
//查询sql语句
std::vector<vector<string>> stdsqliteDataBase::query(const std::string sql)
{
    vector<vector<string>> data;

    char **result = NULL;
    int rows = 0;
    int cols = 0;
    int ret = sqlite3_get_table(m_db,sql.c_str(),&result,&rows,&cols,NULL);
    if(ret != SQLITE_OK)
    {
        cout<<"sqlite3_get_table error"<< sqlite3_errmsg(m_db) <<endl;
        return data;
    }
    for(int row =1;row<=rows;row++)
    {
        vector <string> rowData;
        for(int col = 0;col<cols;col++)
        {
            rowData.push_back(result[row*cols+col]);
        }
        data.push_back(rowData);
        
    }

}

//关闭数据库
void stdsqliteDataBase::closeDB()
{
    sqlite3_close(m_db);
};