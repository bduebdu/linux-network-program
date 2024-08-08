#include "stdTcpServer.h"
#include <iostream>
using namespace std;
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <cctype>
#include <arpa/inet.h>

/* 前置声明 */
struct StdTcpSocketPrivate
{
    /* 通信句柄 */
    int connfd;
    /* 通信是否建立成功 */
    bool m_connected;
};

struct StdTcpServerPrivate
{
    /* 监听句柄 */
    int sockfd;
    /* 服务器是否正在监听 */
    bool m_isRunning;
};

/* 构造函数 */
StdTcpSocket::StdTcpSocket() : m_sockAttr(std::make_unique<StdTcpSocketPrivate>())
{
    /* 通信句柄 */
    m_sockAttr->connfd = -1;
    /* 通信是否建立 */
    m_sockAttr->m_connected = false;
}

/* 析构函数 */
StdTcpSocket::~StdTcpSocket()
{
    /* 关闭文件描述符 */
    if (m_sockAttr->connfd > 0)
    {
        close(m_sockAttr->connfd);
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
        /* 抛异常. */
        throw std::runtime_error("socket create error.");
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
        /* 抛异常. */
        throw std::runtime_error("connect error.");
    }

    m_sockAttr->m_connected = true;
    
    return 0;
}

/* 获取/得到 属性 */
StdTcpSocketPrivate * StdTcpSocket::getSockAttr()
{
    return m_sockAttr.get();
}

/* 是否连接成功 */
bool StdTcpSocket::isConnected() const
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
StdTcpServer::StdTcpServer() : m_tcpAttr(std::make_unique<StdTcpServerPrivate>())
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
    if (m_tcpAttr->sockfd > 0)
    {
        close(m_tcpAttr->sockfd);
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
        throw std::runtime_error("socket create error.");
    }

    /* 设置监听的套接字 */
    m_tcpAttr->sockfd = sockfd;
   
    /* 设置端口复用 */
    int optVal = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
    if (ret != 0)
    {
        perror("bind error:");
        throw std::runtime_error("setsockopt error.");
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
        throw std::runtime_error("bind error");
    }
    
    // 给监听的套接字设置监听
    ret = listen(sockfd, 10);
    if (ret != 0)
    {
        perror("listen error:");
        throw std::runtime_error("listen error");
    }

    /* 设置状态为:正在监听 */
    m_tcpAttr->m_isRunning = true;

    return true;
}

/* 接收连接 */
std::shared_ptr<StdTcpSocket> StdTcpServer::getClientSock()
{
    int clientConnfd = accept(m_tcpAttr->sockfd, NULL, NULL);
    if (clientConnfd == -1)
    {
        perror("accpet error:");
        throw std::runtime_error("accept error.");
    }

    /* 程序到这个地方, 就说明有客户端进行连接 */
    cout << "clientConnfd:" << clientConnfd << endl;

    /* 通信类 */
    auto client = std::make_shared<StdTcpSocket>();
    /* 套接字 */
    client->getSockAttr()->connfd = clientConnfd;
    client->getSockAttr()->m_connected = true;

    return client;
}