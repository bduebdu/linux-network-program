#include "messageHandle.h"
#include "stdShared.h"
#include <json-c/json.h>
/* 构造函数 */
MessageHandle::MessageHandle(const StdTcpSocketPtr & clientInfo) : m_funtion(clientInfo)
{
    /* 注册业务 */
    m_handles[REGISTER] = [this](const string & msg) {m_funtion.handleRegisterInfo(msg); };
    m_handles[LOGIN] = [this](const string & msg) {m_funtion.handleLoginInfo(msg); };
    // m_handles[ADDFRIEND] = [this](const string & msg) {m_funtion.handleAddFriendInfo(msg); };
    m_handles[SEARCH_MUSIC] = [this](const string & msg) {m_funtion.handleOnlineMusicInfo(msg); };
    m_handles[SEARCH_LYRIC] = [this](const string & msg) {m_funtion.handleOnlineLyricInfo(msg); };
    m_handles[ONLINE_LIST] = [this](const string & msg) {m_funtion.handleOnlineListInfo(msg); };



}


/* 析构函数 */
MessageHandle::~MessageHandle()
{

}


/* 处理信息 */
void MessageHandle::handleMessage(const string & msg)//此处msg是json字符串
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
    json_object * jsonObj = json_tokener_parse(msg.c_str());
    if(jsonObj!=NULL)
    {
        int type = json_object_get_int(json_object_object_get(jsonObj,"type"));
        auto iter = m_handles.find(type);
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
}