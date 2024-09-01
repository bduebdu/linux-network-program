#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <iostream>
#include <string>
#include "stdTcpServer.h"
#include "stdShared.h"
#include "sqliteDataBase.h"
class Function
{
public:
    /* 构造函数 */
    Function(const StdTcpSocketPtr & clientInfo);
    /* 析构函数 */
    ~Function();

private:
    /* 判断用户名是否存在 */
    bool userIsExist(const char * username);
    //保存用户信息
    bool saveUserInfo(const char * username, const char *  passwd);
    /* 用户名和密码是否匹配 */
    bool userIsMatchPasswd(const char * username, const char *  passwd);
    /* 用户是否已经登陆/在线 */
    bool userIsOnlined(const char * username);
    /* 读取音乐文件 */
    string readMusicFile(const string &filePath) ;
public:
    /* 处理注册 */
    void handleRegisterInfo(const string & msg);
    /* 处理登陆 */
    void handleLoginInfo(const string & msg);

    //处理在线音乐
    void handleOnlineMusicInfo(const string & msg);
    //处理在线歌词
    void handleOnlineLyricInfo(const string & msg);
    //处理在线列表
    void handleOnlineListInfo(const string & msg);
    //处理批量下载
    void handleMultipleDownloadInfo(const string & msg);

    void handleAddFriendInfo(const string & msg);
            
    void handleDelFriendInfo(const string & msg);

    void handleNewGroupInfo(const string & msg);

    void handleAExitGroupInfo(const string & msg);

private:
    StdTcpSocketPtr m_clientInfo;

    stdsqliteDataBase m_sqliteDB;
};


#endif //__FUNCTION_H__