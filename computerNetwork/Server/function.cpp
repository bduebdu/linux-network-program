#include "function.h"


void Funtion::handleLoginInfo(const char * username,const char * passwd)
{

}
void Funtion::handleLoginInfo(const char * username,const char * passwd)
{
    //判断用户是否已经注册
    if(userIsExist(username)==false)
    {
        //程序进入时说明用户名不存在


        //信息发送到客户端
    }
    else
    {
        //存在时判断用户名密码是否匹配
        if(userIsMatchPasswd(username,passwd)==false)
        {

        }
        //判断用户是否已经登陆
        if(userIsOnlined(username)==false)
        {

        }
    }

}


    //判断用户是否已经注册
bool Function::userIsExist(const char * username)
{
    return true;
}
    //存在时判断用户名密码是否匹配
bool Function::userIsMatchPasswd(const char * username,const char * passswd)
{
    return true;
}
    //判断用户是否已经登陆
bool Function::userIsOnlined(const char * username)
{
    return true;
}