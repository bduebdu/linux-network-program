#ifndef __STDSHARED_H__
#define __STDSHARED_H__

enum FUNCTION_OPTION
{
    REGISTER = 1,
    LOGIN,
    EXIT,
};


struct Msg
{
    FUNCTION_OPTION type;
    char name[20];
    char passwd[20];
};

#endif