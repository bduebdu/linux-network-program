#include<iostream>
using namespace std;
#include<mysql/mysql.h>
#include<stdlib.h>
#include<unistd.h>
#include<iomanip>
/*
void demoLogic()
{
    const char * sql ="create table if not exists students(       \
                        id int auto_increment primary key,\
                        name text not null,\
                        age int,\
                        grade varchar(10)\
                        );";
    int ret = mysql_query(sqlDB,sql);
    if(ret != 0)
    {
        cout<<"mysql_query error"<<endl;
        _exit(-1);
    }
    //插入数据
    // sql = "insert into students (name,age,grade) values ('zhangsan','20','A');";
    // sql = "insert into students (name,age,grade) values ('zhangsan','20','A');";
    // sql = "insert into students (name,age,grade) values ('zhangsan','20','A');";

    //查询语句
    sql = "select * from students;";
    ret = mysql_query(sqlDB,sql);
    if(ret != 0)
    {
        cout<<"mysql_query error"<<endl;
        _exit(-1);
    }
    cout<<"查询成功"<<endl;
    //结果集
    MYSQL_RES * result = mysql_store_result(sqlDB);
    if(result == NULL)
    {
        cout<<"mysql_store_result error."<<endl;
        exit(-1);
    }

    //获取结果集的列数
    unsigned int cols = mysql_num_fields(result);
    cout<<"cols:"<<cols<<endl;
    unsigned int rows = mysql_num_fields(result);
    cout<<"rows:"<<rows<<endl;


    //获取数据库字段
    MYSQL_FIELD * field = NULL;
    while((field = mysql_fetch_field(result)) != NULL)
    {
        cout<<left<<setw(16)<<field->name;
    }
    cout<<endl;

    //获取数据
    MYSQL_ROW row;
    while((row = mysql_fetch_row(result))!=NULL)
    {
        //每个row变量表示一行
        for(int col = 0;col<cols;col++)
        {
            cout<<left<<setw(16)<<row[col];
        }
        cout<<endl;
    }

    //释放结果集
    mysql_free_result(result);
    
    //关闭数据库
    mysql_close(sqlDB);
}
*/


int main()
{
    //连接数据库
    MYSQL * sqlDB = nullptr;
    sqlDB = mysql_init(NULL);
    if (sqlDB == nullptr)
    {
        cout<<"falied"<<endl;
        _exit(-1);
    }
    cout <<"初始化成功"<<endl;

    const char * host = "localhost";
    const char * user = "root";
    const char * passwd = "123456";
    const char * baseName = "demoTest";
    int port = 3306;

    if(mysql_real_connect(sqlDB,host,user,passwd,baseName,port,NULL,0)==NULL)
    {
        cout<<"File:"<<__FILE__<<"Line:"<<__LINE__<<"connect error"<<mysql_error(sqlDB)<<endl;
        exit(-1);
    }
    cout<<"连接成功"<<endl;

    //事务开始
    if(mysql_query(sqlDB,"START TRANSACTION"))
    {
        cout<<"START TRANSACTION"<<mysql_error(sqlDB)<<endl;
        exit(-1);
    }

    //执行sql
    const char *sql1 = "update students set age = age -100 where name = 'lisi';";
    const char *sql2 = "update students set age = age +100 where name = 'wangwu';";
    
    if(mysql_query(sqlDB,sql1))
    {
        cout<<"File:"<<__FILE__<<"Line:"<<__LINE__<<mysql_error(sqlDB)<<endl;
        mysql_query(sqlDB,"ROLLBACK");
        exit(-1);
    }
    else
    {
        //sql语句没问题
        unsigned int affectRows = mysql_affected_rows(sqlDB);
        cout<<"affectRows:"<<affectRows<<endl;
        if(affectRows ==0)
        {
            mysql_query(sqlDB,"ROLLBACK");
            exit(-1);
        }
    
    }
    cout<<"good sql1.."<<endl;

    if(mysql_query(sqlDB,sql2))
    {
        cout<<"File:"<<__FILE__<<"Line:"<<__LINE__<<mysql_error(sqlDB)<<endl;
        mysql_query(sqlDB,"ROLLBACK");
        exit(-1);
    }
    else
    {
        //sql语句没问题
        unsigned int affectRows = mysql_affected_rows(sqlDB);
        cout<<"affectRows:"<<affectRows<<endl;
        if(affectRows ==0)
        {
            mysql_query(sqlDB,"ROLLBACK");
            exit(-1);
        }
    
    }

    cout<<"good sql2.."<<endl;
    //事务提交
    
    
    
    //关闭数据库
    mysql_close(sqlDB);

    return 0;
}