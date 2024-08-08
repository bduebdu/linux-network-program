#ifndef __STDSHARED_H__
#define __STDSHARED_H__


enum FUNCTION_OPTION
{
    REGISTER = 1,   /* 注册 */
    LOGIN,          /* 登陆 */
    EXIT,
    ADDFRIEND,      /* 添加好友 */
    DELFRIEND,      /* 删除好友 */
    NEWGROUP,       /* 建群 */
    EXITGROUP,      /* 退群 */
};


struct Msg
{
    FUNCTION_OPTION type;
    char name[20];
    char passwd[20];
};


#endif // __STDSHARED_H__