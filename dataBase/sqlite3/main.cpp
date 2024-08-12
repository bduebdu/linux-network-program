#include<iostream>
using namespace std;
#include<stdio.h>
#include<sqlite3.h>
#include<unistd.h>
#include<iomanip>
#include<vector>
void execute(sqlite3 * myDB)
{
    //创建数据库表
    const char * sql = "create table if not exists userinfo(  \
                        username text not null, \
                        passwd text not null   \
                        );";
    int ret = sqlite3_exec(myDB,sql,NULL,NULL,NULL);//执行sql语句
    if(ret != SQLITE_OK)
    {
        cout<<"sqlite3 open error"<<sqlite3_errmsg(myDB)<<endl;
        _exit(-1);
    }
   

    
#if 0
    string username;
    cout<<"请输入用户名"<<endl;
    cin >> username;
    string passwd;
    cout<<"请输入密码"<<endl;
    cin >> passwd;
    //插入数据
    char buffer[64] = "insert into userinfo (username,passwd) values ('%s','%s');";
    char requestSql[128] ={0};
    sprintf(requestSql,buffer,username.c_str(),passwd.c_str());
    cout << "requestSql1:"<<requestSql<<endl;   
    //执行sql语句
    ret = sqlite3_exec(myDB,requestSql,NULL,NULL,NULL);
    if(ret != SQLITE_OK)
    {
        cout<<"sqlite3 exec error"<<sqlite3_errmsg(myDB)<<endl;
        _exit(-1);
    }
#else    
    //to do...


    //修改数据
    sql ="update userinfo set passwd = '666' where username = 'zhangsan';";
    ret = sqlite3_exec(myDB,sql,NULL,NULL,NULL);
#endif    
}

int main()
{
    sqlite3 * myDB = NULL;
    const char * baseName = "./test.db";
    int ret = sqlite3_open(baseName,&myDB);//连接数据库
    if(ret != SQLITE_OK)
    {
        cout<<"sqlite3 open error"<<sqlite3_errmsg(myDB)<<endl;
        _exit(-1);
    }

    const char * sql ="select * from students;";
    
    char **result = NULL;
    int rows = 0;
    int cols = 0;
    ret = sqlite3_get_table(myDB,sql,&result,&rows,&cols,NULL);
    if(ret != SQLITE_OK)
    {
        cout<<"sqlite3_get_table error"<<sqlite3_errmsg(myDB)<<endl;
        _exit(-1);
    }

    cout<<"row:"<<rows<<"col:"<<cols<<endl;
#if 0
    //二维数组
    for(int row = 0;row<rows;row++)
    {
        for(int col =0;col<cols;col++)
        {
            cout<<result[row][col]<<" ";
        }
        cout<<endl;
    }
#endif
    vector<vector<string>> Datas;

    for(int row =1;row<=rows;row++)
    {
        vector<string> rowData;
        for(int col =0;col<cols;col++)
        {
            cout <<left<<setw(16)<<result[row * cols +col];
            rowData.push_back(result[row*cols+col]);
        }
        Datas.push_back(rowData);
        
    }
    cout<<endl;

    cout<<"rows:"<<Datas.size()<<endl;
    for(int idx = 0;idx<Datas.size();idx++)
    {
        vector<string> rowData = Datas[idx];
        for(int jdx = 0;jdx <rowData.size();jdx++)
        {
            cout<<"rowData["<<jdx<<"]="<<rowData[jdx]<<"\t";

        }
        cout<<endl;
    }



    sqlite3_close(myDB);//关闭数据库

    return 0;
}