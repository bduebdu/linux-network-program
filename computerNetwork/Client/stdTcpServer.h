#ifndef __STDTCPSERVER_H__
#define __STDTCPSERVER_H__

#include <iostream>
using namespace std;
#include <string>
#include <memory>

struct StdTcpSocketPrivate
{
    /* 通信句柄 */
    int connfd;
    /* 通信是否建立成功 */
    bool m_connected;
};

/* 通信类 */
class StdTcpSocket
{
public:
    /* 构造函数 */
    StdTcpSocket();

    /* 析构函数 */
    ~StdTcpSocket();

public:
    /* 连接服务器 */
    int connectToServer(const char * ip, int port);
    /* 是否连接成功 */
    bool isConnected();
    /* 发送信息 */
    int sendMessage(std::string & sendMsg);
    /* 发送信息 */
    int sendMessage(const void * sendMsg, size_t n);
    /* 接收信息 */
    int recvMessage(std::string & recvMessage);
    /* 接收信息 */
    int recvMessage(void * buf, size_t n);
    /* 设置属性 */
    StdTcpSocketPrivate * getSockAttr();
private:
    StdTcpSocketPrivate * m_sockAttr;
};

struct StdTcpServerPrivate
{
    /* 监听句柄 */
    int sockfd;
    /* 服务器是否正在监听 */
    bool m_isRunning;
};


/* 智能指针 */
using StdTcpSocketPtr = std::shared_ptr<StdTcpSocket>;

class StdTcpServer
{
public:
    /* 构造函数 */
    StdTcpServer();

    /* 析构函数 */
    ~StdTcpServer();
public:
    /* 设置监听 */
    bool setListen(int port);

    /* 接收连接 */
    StdTcpSocketPtr getClientSock();
private:
    /* 端口 */
    int m_port;
    /* 服务器的属性 */
    StdTcpServerPrivate * m_tcpAttr;
};


#endif //__STDTCPSERVER_H__