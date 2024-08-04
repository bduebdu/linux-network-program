#include "stdTcpServer.h"
#include <iostream>
using namespace std;
#include <sys/types.h>          
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cctype>

/* 构造函数 */
StdTcpSocket::StdTcpSocket() : m_sockAttr(new StdTcpSocketPrivate)
{
    /* 通信句柄 */
    m_sockAttr->connfd = -1;
    /* 通信是否建立 */
    m_sockAttr->m_connected = false;
}

/* 析构函数 */
StdTcpSocket::~StdTcpSocket()
{
    close(m_sockAttr->connfd);
    /* 通信句柄 */
    m_sockAttr->connfd = -1;
    /* 通信是否建立 */
    m_sockAttr->m_connected = false;

    if (m_sockAttr)
    {
        delete m_sockAttr;
        m_sockAttr = nullptr;
    }
}

/* 连接服务器 */
int StdTcpSocket::connectToServer(const char * ip, int port)
{
    /* 创建套接字 */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket error:");
        return -1;
    }
    m_sockAttr->connfd = sockfd;

    /* 连接服务器 */
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    inet_pton(AF_INET, ip, &(serverAddress.sin_addr.s_addr));
    
    int ret = connect(sockfd, reinterpret_cast<const sockaddr *>(&serverAddress), sizeof(serverAddress));
    if (ret != 0)
    {
        perror("connect error:");
        return -1;
    }

    m_sockAttr->m_connected = true;
    
    return 0;
}


/* 获取/得到 属性 */
StdTcpSocketPrivate * StdTcpSocket::getSockAttr()
{
    return m_sockAttr;
}


/* 是否连接成功 */
bool StdTcpSocket::isConnected()
{
    return m_sockAttr->m_connected;
}

/* 发送信息 */
int StdTcpSocket::sendMessage(const char * sendMsg, size_t n)
{
    int writeBytes = write(m_sockAttr->connfd, sendMsg, n);
    return writeBytes;
}

/* 发送信息 */
int StdTcpSocket::sendMessage(std::string & sendMsg)
{
    return sendMessage(sendMsg.c_str(), sendMsg.size());
}

/* 接收信息 */
int StdTcpSocket::recvMessage(void * buf, size_t n)
{
    int readBytes = read(m_sockAttr->connfd, buf, n);
    return readBytes;
}

/* 接收信息 */
int StdTcpSocket::recvMessage(std::string & recvMessage)
{
    /* todo... */
    return 0;
}


/* 构造函数 */
StdTcpServer::StdTcpServer() : m_tcpAttr(new StdTcpServerPrivate)
{
    /* 监听套接字 */
    m_tcpAttr->sockfd = -1;
    /* 是否监听 */
    m_tcpAttr->m_isRunning = false;
}

/* 析构函数 */
StdTcpServer::~StdTcpServer()
{
    /* 关闭套接字句柄 */
    close(m_tcpAttr->sockfd);

    {
        m_tcpAttr->sockfd = -1;
        m_tcpAttr->m_isRunning = false;
    }

    /* 释放内存 */
    if (m_tcpAttr)
    {
        delete m_tcpAttr;
        m_tcpAttr = nullptr;
    }
}




/* 设置监听 */
bool StdTcpServer::setListen(int port)
{
    /* 类内部维护端口信息. */
    this->m_port = port;

    /* 创建套接字 */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket error");
        return false;
    }

    /* 设置监听的套接字 */
    m_tcpAttr->sockfd = sockfd;
   
    /* 设置端口复用 */
    int optVal = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
    if (ret != 0)
    {
        perror("bind error:");
        return false;
    }

    /* 绑定IP和端口 */
    struct sockaddr_in localAddress;
    memset(&localAddress, 0, sizeof(localAddress));
    /* 地址族 */
    localAddress.sin_family = AF_INET;
    localAddress.sin_port = htons(m_port);
    localAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    /* 绑定 */
    ret = bind(sockfd, reinterpret_cast<const sockaddr *>(&localAddress), sizeof(localAddress));
    if (ret != 0)
    {
        perror("bind error:");
        return false;
    }
    
    // 给监听的套接字设置监听
    ret = listen(sockfd, 10);
    if (ret != 0)
    {
        perror("listen error:");
        return false;
    }

    /* 设置状态为:正在监听 */
    m_tcpAttr->m_isRunning = true;

    return true;
}



/* 接收连接 */
StdTcpSocketPtr StdTcpServer::getClientSock()
{
    int clientConnfd = accept(m_tcpAttr->sockfd, NULL, NULL);
    if (clientConnfd == -1)
    {
        perror("accpet error:");
        return StdTcpSocketPtr();
    }

    /* 程序到这个地方, 就说明有客户端进行连接 */
    cout << "clientConnfd:" << clientConnfd << endl;

    /* 通信类 */
    StdTcpSocketPtr ptr(new StdTcpSocket);
    /* 套接字 */
    ptr->getSockAttr()->connfd = clientConnfd;
    ptr->getSockAttr()->m_connected = true;

    return ptr;
}