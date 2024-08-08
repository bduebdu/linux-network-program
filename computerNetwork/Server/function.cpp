#include "function.h"


/* 构造函数 */
Function::Function(const StdTcpSocketPtr & clientInfo)
{
    m_clientInfo = clientInfo;
}

/* 析构函数 */
Function::~Function()
{

}

void Function::handleRegisterInfo(const Msg & msg)
{
    /* todo... */
    std::cout << "username:" << msg.name << std::endl;
    std::cout << "passwd:" << msg.passwd << std::endl;

    string replyInfo = "注册成功.";
    m_clientInfo->sendMessage(replyInfo);
}

void Function::handleLoginInfo(const Msg & msg)
{
    std::cout << "username:" << msg.name << std::endl;
    std::cout << "passwd:" << msg.passwd << std::endl;

    string replyInfo = "登陆失败, 密码不正确.";
    m_clientInfo->sendMessage(replyInfo);
#if 0
    /* 判断用户名是否已经注册 */
    if (userIsExist(username) == false)
    {
        /* 程序进入这个里面, 说明用户名不存在 */


        /* 将信息发送到客户端 */
        
    }
    else 
    {
        /* 如果用户名存在, 判断用户名和密码是否匹配 */
        userIsMatchPasswd(username, passwd);

        /* 判断用户是否已经登陆 */
        userIsOnlined(username);
    }
#endif

}


/* 判断用户名是否存在 */
bool Function::userIsExist(const char * username)
{
    /* todo... */
    return true;
}

/* 用户名和密码是否匹配 */
bool Function::userIsMatchPasswd(const char * username, const char *  passwd)
{
    /* todo... */
    return true;
}

/* 用户是否已经登陆/在线 */
bool Function::userIsOnlined(const char * username)
{
    /* todo... */
    return true;
}