#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <iostream>
#include <string>
#include "stdTcpServer.h"
#include "stdShared.h"

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
    /* 用户名和密码是否匹配 */
    bool userIsMatchPasswd(const char * username, const char *  passwd);
    /* 用户是否已经登陆/在线 */
    bool userIsOnlined(const char * username);
public:
    /* 处理注册 */
    void handleRegisterInfo(const Msg & msg);
    /* 处理登陆 */
    void handleLoginInfo(const Msg & msg);

    void handleAddFriendInfo(const Msg & msg);
            
    void handleDelFriendInfo(const Msg & msg);

    void handleNewGroupInfo(const Msg & msg);

    void handleAExitGroupInfo(const Msg & msg);

private:
    StdTcpSocketPtr m_clientInfo;
};


#endif //__FUNCTION_H__