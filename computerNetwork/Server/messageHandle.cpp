#include "messageHandle.h"
#include "stdShared.h"

/* 构造函数 */
MessageHandle::MessageHandle(const StdTcpSocketPtr & clientInfo) : m_funtion(clientInfo)
{
    /* 注册业务 */
    m_handles[REGISTER] = [this](const Msg & msg) {m_funtion.handleRegisterInfo(msg); };
    m_handles[LOGIN] = [this](const Msg & msg) {m_funtion.handleLoginInfo(msg); };

    /* */
}


/* 析构函数 */
MessageHandle::~MessageHandle()
{

}


/* 处理信息 */
void MessageHandle::handleMessage(const Msg & msg)
{
#if 0
    if (msg.type == REGISTER)
    {
        m_funtion.handleRegisterInfo(msg);
    }
    else if (msg.type == LOGIN)
    {
        m_funtion.handleLoginInfo(msg);
    }
    // ...
#endif

    auto iter = m_handles.find(msg.type);
    if (iter != m_handles.end())
    {
        /* 执行回调函数 */
        iter->second(msg);
    }
    else 
    {
        /* 处理位置的消息类型 */
        cout << "unknown message type." << endl;
    }
}