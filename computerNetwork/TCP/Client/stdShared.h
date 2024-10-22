#ifndef __STDSHARED_H__
#define __STDSHARED_H__


enum FUNCTION_OPTION
{
    REGISTER = 1,   /* 注册 */
    LOGIN,          /* 登陆 */
    EXIT,
    ADDFRIEND,      /* 添加好友 */
    DELFRIEND,      /* 删除好友 */
    FRIENDCHAT,        //好友聊天
    FRIENDLIST,         //好友列表
    NEWGROUP,       /* 建群 */
    EXITGROUP,      /* 退群 */
    JOINGROUP,      //加入群
    INVITEGROUP,       //邀请加群
    CHATGROUP,      //群聊聊天
};

enum REPLY_CODE
{
    REGISTER_SUCCESS,   //注册成功
    REGISTER_USEREXIST,   //重复注册
    LOGIN_SUCCESS,     //登陆成功
    LOGIN_PASSWD_ERROR,//密码错误
    LOGIN_NOUSER,      //无用户

};

struct Msg
{
    FUNCTION_OPTION type;
    char name[20];
    char passwd[20];
    char toname[20];
};

struct ReplyMsg
{
    FUNCTION_OPTION type;//操作类型

    REPLY_CODE ststic_code;//回复的状态码
};


#endif // __STDSHARED_H__