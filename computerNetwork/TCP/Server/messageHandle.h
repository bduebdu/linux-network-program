#ifndef __MESSAGEHANDLE_H__
#define __MESSAGEHANDLE_H__

#include <unordered_map>
#include <functional>
#include "stdShared.h"
#include "function.h"
#include "stdTcpServer.h"

/* 函数注册表/基 函数映射表 */
class MessageHandle
{
public:
    using HandleFunction = std::function<void(const Msg &)>;

    /* 构造函数 */
    MessageHandle(const StdTcpSocketPtr & clientInfo);

    /* 析构函数 */
    ~MessageHandle();

public:
    /* 处理信息 */
    void handleMessage(const Msg & msg);

private:
    Function m_funtion;
    /* 映射 */
    std::unordered_map<int, HandleFunction> m_handles;
};


#endif //__MESSAGEHANDLE_H__