#include "function.h"
#include <cstring>
#include <iostream>
#include <json-c/json.h>
#include <dirent.h>
#include <sys/types.h>
#include <cstdio>
#include <fstream>
using namespace std;
#include "sqliteDataBase.h"
#include "base64.h"



/* 构造函数 */
Function::Function(const StdTcpSocketPtr & clientInfo)
{
    m_clientInfo = clientInfo;
    m_sqliteDB.connectDB("./test.db");
}

/* 析构函数 */
Function::~Function()
{

}

void Function::handleRegisterInfo(const string & msg)
{
    cout<<"handleRegisterInfo:"<<endl;
    const char * username = NULL;
    const char * passwd = NULL;
    json_object * jsonObj = json_tokener_parse(msg.c_str());
    if(jsonObj!= NULL)
    {
        username = json_object_get_string(json_object_object_get(jsonObj,"username"));        
        passwd = json_object_get_string(json_object_object_get(jsonObj,"passwd"));        

    }
    //创建json对象
    json_object * resObj = json_object_new_object();
    //设置keyvalue
    json_object_object_add(resObj,"type",json_object_new_int(REGISTER));

    //用户存在
    if(userIsExist(username) == true)
    {
        json_object_object_add(resObj,"result",json_object_new_string("failed"));
        json_object_object_add(resObj,"reason",json_object_new_string("failed"));
    }
    else
    {
        saveUserInfo(username,passwd);
        json_object_object_add(resObj,"result",json_object_new_string("success"));
    }
    //json对象转成json字符串
    const char * resStr = json_object_to_json_string(resObj);
    m_clientInfo->sendMessage(resStr);
}

void Function::handleLoginInfo(const string & msg)
{
    cout<<"handleLoginInfo:"<<endl;
    const char * username = NULL;
    const char * passwd = NULL;
    json_object * jsonObj = json_tokener_parse(msg.c_str());
    if(jsonObj!= NULL)
    {
        username = json_object_get_string(json_object_object_get(jsonObj,"username"));        
        passwd = json_object_get_string(json_object_object_get(jsonObj,"passwd"));        

    }
   //创建json对象
    json_object * resObj = json_object_new_object();
    //设置keyvalue
    json_object_object_add(resObj,"type",json_object_new_int(LOGIN));

    if(userIsExist(username)==false)//判断用户是否注册
    {
        json_object_object_add(resObj,"result",json_object_new_string("failed"));
        json_object_object_add(resObj,"reason",json_object_new_string("用户不存在"));
    }
    else
    {
        if(userIsMatchPasswd(username,passwd)==false)
        {
            json_object_object_add(resObj,"result",json_object_new_string("failed"));
            json_object_object_add(resObj,"reason",json_object_new_string("密码错误"));
        }
        else
        {
            json_object_object_add(resObj,"result",json_object_new_string("success"));
        }
    }
    //将json对象转成json字符串
    const char * resStr = json_object_to_json_string(resObj);
    cout<<"resStr:"<<resStr<<endl;
    m_clientInfo->sendMessage(resStr);
}


string Function::readMusicFile(const string &filePath) //读取音乐文件
{
    ifstream file(filePath, ios::in | ios::binary);
    if (file) 
    {
        // 获取文件大小
        file.seekg(0, ios::end);
        size_t size = file.tellg();
        file.seekg(0, ios::beg);

        // 读取文件内容
        string buffer(size, '\0');
        file.read(&buffer[0], size);
        file.close();

        return buffer;
    } 
    else 
    {
        return "";  // 文件读取失败
    }
}

/* 处理在线音乐 */
void Function::handleOnlineMusicInfo(const string & msg)
{
    cout << "handleOnlineMusicInfo:" <<  msg << endl;

    const char * musicname = NULL;
    json_object * jsonObj = json_tokener_parse(msg.c_str());
    if (jsonObj != NULL)
    {
        musicname = json_object_get_string(json_object_object_get(jsonObj, "musicname"));
    }


    /* 构建json字符串 */
    /* 1. 创建响应json对象 */
    json_object * resObj = json_object_new_object();
    /* 2. 设置<key:value> */
    json_object_object_add(resObj, "type", json_object_new_int(SEARCH_MUSIC));


    /* 拼接音乐的文件路径 */
    string musicFilePath = "./music/"+ string(musicname) + ".mp3";  // 假设音乐文件为MP3格式
    string musicData = readMusicFile(musicFilePath);
    cout << "size:" << musicData.size() << endl;
    if (!musicData.empty()) 
    {
        // 使用 Base64 编码音乐数据
        string encodedMusicData = base64_encode(reinterpret_cast<const unsigned char*>(musicData.c_str()), musicData.size());

        json_object_object_add(resObj, "musicname", json_object_new_string(musicname));
        json_object_object_add(resObj, "musiccontent", json_object_new_string(encodedMusicData.c_str()));
    
    }
    else
    {
        cout<<"empty"<<endl;
        
       
    }

    const char * resStr = json_object_to_json_string(resObj);
    cout << "resStr:" << resStr << endl;
    m_clientInfo->sendMessage(resStr);
}

void Function::handleDownloadInfo(const string & msg)
{
   
}



void Function::handleAddFriendInfo(const string & msg)
{
    // cout << "msg.toName" << msg.toName << endl;
}

/* 判断用户名是否存在 */
bool Function::userIsExist(const char * username)
{
    string sql = "select count(1) from userinfo where username = '%s';";
    char requestsql[128] = {0};
    sprintf(requestsql,sql.c_str(),username);
    vector<vector<string>> res = m_sqliteDB.query(requestsql);
    if(res[0][0] == "0")
    {
        return false;   
    }
    
    return true;
}


//保存用户信息
bool Function::saveUserInfo(const char * username, const char *  passwd)
{
    std::string sql = "insert into userInfo (username,passwd) values ('%s','%s');";
    char requestSql[128] = {0};
    sprintf(requestSql,sql.c_str(),username,passwd);
    if(m_sqliteDB.execute(requestSql) == false)
    {
        cout<<"save error"<<__FILE__<<":"<<__LINE__<<endl;
        return false;
    }
    return true;

}

/* 用户名和密码是否匹配 */
bool Function::userIsMatchPasswd(const char * username, const char *  passwd)
{
    string sql = "select count(1) from userInfo where username = '%s' and passwd = '%s';";
    char requestsql[128] = {0};
    sprintf(requestsql,sql.c_str(),username,passwd);
    vector<vector<string>> res = m_sqliteDB.query(requestsql);
    //QueryResult
    if(res[0][0] == "0")
    {
        return false;
    }

    return true;
}

/* 用户是否已经登陆/在线 */
bool Function::userIsOnlined(const char * username)
{
    /* todo... */
    return true;
}