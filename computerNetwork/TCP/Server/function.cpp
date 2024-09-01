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
#include <sys/stat.h>



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
    // cout << "resStr:" << resStr << endl;
    m_clientInfo->sendMessage(resStr);
}
//处理在线歌词
void Function::handleOnlineLyricInfo(const string & msg)
{
    cout << "handleOnlineLyricInfo:" <<  msg << endl;

    const char * lyricname = NULL;
    json_object * jsonObj = json_tokener_parse(msg.c_str());
    if (jsonObj != NULL)
    {
        lyricname = json_object_get_string(json_object_object_get(jsonObj, "lyricname"));
    }


    /* 构建json字符串 */
    /* 1. 创建响应json对象 */
    json_object * resObj = json_object_new_object();
    /* 2. 设置<key:value> */
    json_object_object_add(resObj, "type", json_object_new_int(SEARCH_LYRIC));


    /* 拼接歌词的文件路径 */
    string lyricFilePath = "./music/"+ string(lyricname) + ".lrc";  // 假设音乐文件为MP3格式
    string lyricData = readMusicFile(lyricFilePath);
    cout << "size:" << lyricData.size() << endl;
    if (!lyricData.empty()) 
    {
        // 使用 Base64 编码音乐数据
        string encodedLyricData = base64_encode(reinterpret_cast<const unsigned char*>(lyricData.c_str()), lyricData.size());

        json_object_object_add(resObj, "lyricname", json_object_new_string(lyricname));
        json_object_object_add(resObj, "lyriccontent", json_object_new_string(encodedLyricData.c_str()));
    
    }
    else
    {
        cout<<"empty"<<endl;
    }

    const char * resStr = json_object_to_json_string(resObj);
    // cout << "resStr:" << resStr << endl;
    m_clientInfo->sendMessage(resStr);
}    

//处理在线列表
void Function::handleOnlineListInfo(const string &msg) 
{
        std::cout << "handleOnlineListInfo:" << msg << std::endl;
        // 解析请求中的类型
        json_object *jsonObj = json_tokener_parse(msg.c_str());
        if (jsonObj != NULL) {
            int type = json_object_get_int(json_object_object_get(jsonObj, "type"));
            if (type == ONLINE_LIST) {
                // 读取指定路径下的所有歌曲名
                std::vector<std::string> musicNames;
                // 打开目录
                DIR *dir = opendir("./music");
                if (dir != NULL) {
                    struct dirent *entry;
                    while ((entry = readdir(dir)) != NULL) {
                        // 检查是否为 "." 或 ".."
                        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                            continue;
                        }
                        std::string fullPath = "./music/";
                        fullPath += entry->d_name;
                        struct stat fileInfo;
                        if (stat(fullPath.c_str(), &fileInfo) == 0 && S_ISREG(fileInfo.st_mode)) {
                            // 检查文件扩展名
                            if (fullPath.find_last_of(".") != std::string::npos) {
                                std::string extension = fullPath.substr(fullPath.find_last_of(".") + 1);
                                if (extension == "mp3") {
                                    std::string baseName = entry->d_name;
                                    size_t dotPos = baseName.find_last_of(".");
                                    if (dotPos != std::string::npos) {
                                        baseName.erase(dotPos); // 去除扩展名
                                    }
                                    musicNames.push_back(baseName);
                                }
                            }
                        }
                    }
                    closedir(dir);
                } else {
                    std::cout << "Failed to open directory: ./music" << std::endl;
                }

                // 构建响应 JSON 对象
                json_object *resObj = json_object_new_object();
                json_object_object_add(resObj, "type", json_object_new_int(ONLINE_LIST));

                // 添加歌曲名列表
                json_object *musicNamesArray = json_object_new_array();
                for (const auto &musicName : musicNames) {
                    json_object_array_add(musicNamesArray, json_object_new_string(musicName.c_str()));
                }
                json_object_object_add(resObj, "musicnames", musicNamesArray);

                // 转换为字符串并发送
                const char *resStr = json_object_to_json_string(resObj);
                m_clientInfo->sendMessage(resStr);

                // 清理内存
                json_object_put(resObj);
            }
        }

}

//处理批量下载
void Function::handleMultipleDownloadInfo(const string &msg) 
{
    std::cout << "handleMultipleDownloadInfo:" << msg << std::endl;

    // 解析请求中的类型
    json_object *jsonObj = json_tokener_parse(msg.c_str());
    if (jsonObj != NULL) 
    {
        int type = json_object_get_int(json_object_object_get(jsonObj, "type"));
        if (type == MULTIPLE_DONLOAD)
        {
            // 解析请求中的歌曲名称列表
            json_object *musicNamesJsonArray = json_object_object_get(jsonObj, "musicnames");
            if (musicNamesJsonArray != NULL && json_object_is_type(musicNamesJsonArray, json_type_array))
            {
                // 创建一个JSON对象作为响应
                json_object *resObj = json_object_new_object();
                json_object_object_add(resObj, "type", json_object_new_int(MULTIPLE_DONLOAD));

                // 创建一个JSON数组用于存储歌曲数据
                json_object *musicDataArray = json_object_new_array();

                // 遍历歌曲名称列表
                for (unsigned int i = 0; i < json_object_array_length(musicNamesJsonArray); ++i)
                {
                    json_object *musicNameJson = json_object_array_get_idx(musicNamesJsonArray, i);
                    if (musicNameJson != NULL && json_object_is_type(musicNameJson, json_type_string))
                    {
                        const char *musicName = json_object_get_string(musicNameJson);

                        // 拼接音乐的文件路径
                        std::string musicFilePath = "./music/" + std::string(musicName) + ".mp3";  // 假设音乐文件为MP3格式
                        std::string musicData = readMusicFile(musicFilePath);

                        if (!musicData.empty())
                        {
                            // 使用 Base64 编码音乐数据
                            std::string encodedMusicData = base64_encode(reinterpret_cast<const unsigned char*>(musicData.c_str()), musicData.size());

                            // 创建一个JSON对象存储单首歌曲的数据
                            json_object *musicObj = json_object_new_object();
                            json_object_object_add(musicObj, "musicname", json_object_new_string(musicName));
                            json_object_object_add(musicObj, "musiccontent", json_object_new_string(encodedMusicData.c_str()));

                            // 拼接歌词文件路径
                            std::string lyricsFilePath = "./music/" + std::string(musicName) + ".lrc";
                            std::string musicData = readMusicFile(lyricsFilePath);

                            if (!musicData.empty())
                            {
                                // 使用 Base64 编码歌词数据
                                std::string encodedLyricsData = base64_encode(reinterpret_cast<const unsigned char*>(musicData.c_str()), musicData.size());
                                json_object_object_add(musicObj, "lyricscontent", json_object_new_string(encodedLyricsData.c_str()));
                            }
                            else
                            {
                                std::cout << "Failed to read lyrics file: " << musicName << std::endl;
                            }

                            // 将单首歌曲的数据添加到数组中
                            json_object_array_add(musicDataArray, musicObj);
                        }
                        else
                        {
                            std::cout << "Failed to read music file: " << musicName << std::endl;
                        }
                    }
                }

                // 将音乐数据数组添加到响应对象中
                json_object_object_add(resObj, "musiclist", musicDataArray);

                // 构建JSON字符串
                const char *resStr = json_object_to_json_string(resObj);

                // 发送数据
                m_clientInfo->sendMessage(resStr);

                // 清理
                json_object_put(resObj);
            }
        }
    }
    else
    {
        std::cout << "Failed to parse JSON request." << std::endl;
    }
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