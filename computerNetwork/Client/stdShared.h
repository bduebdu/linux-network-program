#ifndef __STDSHARED_H__
#define __STDSHARED_H__
#include "stdTcpServer.h"

enum FUNCTION_OPTION
{
    REGISTER = 1,
    LOGIN,
    EXIT,
    ADDFRIEND,
    DELFRIEND,
    NEWGROUP,
    EXITGROUP,
};
struct Msg
{
    FUNCTION_OPTION type;
    char name[20];
    char passwd[20];
};

#endif // !__STDSHARED_H__