#ifndef __FUNCTION_H__
#define __FUNCTION_H__
#include <iostream>
#include <string.h>
class Funtion
{

public:
    //构造 析构函数
    Funtion();
    ~Funtion();
    //处理登录和注册
    void handleRegisterInfo(const char * username,const char * passwd);
    void handleLoginInfo(const char * username,const char * passwd);
private:
    //判断用户是否已经注册
    bool userIsExist(const char * username);
    //存在时判断用户名密码是否匹配
    bool userIsMatchPasswd(const char * username,const char * passswd);
    //判断用户是否已经登陆
    bool userIsOnlined(const char * username);
};

#endif