#ifndef __STDTCPSERVER_H__
#define __STDTCPSERVER_H__

#include <iostream>
using namespace std;
#include <string>
#include <memory>

/* 前置声明 */
struct StdTcpSocketPrivate;
struct StdTcpServerPrivate;

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
    bool isConnected() const;
    /* 发送信息 */
    int sendMessage(std::string & sendMsg);
    /* 发送信息 */
    int sendMessage(const char * sendMsg, size_t n);
    /* 接收信息 */
    int recvMessage(std::string & recvMessage);
    /* 接收信息 */
    int recvMessage(void * buf, size_t n);
    /* 设置属性 */
    StdTcpSocketPrivate * getSockAttr();
private:
    /* 智能指针 */
    std::unique_ptr<StdTcpSocketPrivate> m_sockAttr;
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
    std::shared_ptr<StdTcpSocket> getClientSock();
private:
    /* 端口 */
    int m_port;
    /* 服务器的属性 */
    std::unique_ptr<StdTcpServerPrivate> m_tcpAttr;
};


#endif //__STDTCPSERVER_H__