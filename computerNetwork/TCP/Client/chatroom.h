#ifndef __CHATROOM_H__
#define __CHATROOM_H__

#include "stdTcpServer.h"

class ChatRoom
{
public:
    ChatRoom(const StdTcpSocketPtr & clientInfo);


    ~ChatRoom();
public:
    //聊天室入口
    void ChatEnter();


private:
    //聊天菜单
    void ChatMenu();
    //添加好友
    void addFriend();
    //删除好友
    void delFriend();


private:
    StdTcpSocketPtr m_clientSock;
    static const int m_offset = 3;//偏移
};






#endif