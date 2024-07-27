#include <iostream>
using namespace std;
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <cstring>
#include<string>
#define BUFFER_SIZE 32
int main()
{
    //创建管道
    int pipefd[2];
    memset(pipefd,0,sizeof(pipefd));//清空脏数据
    pipe(pipefd);
    pid_t pid = fork();//子进程
    if(pid > 0)
    {
        close(pipefd[0]);
        int num = 666;
        cout<< "I am parent"<<endl;
        string res = "hello world";
        write(pipefd[1],res.c_str(),res.size());

        // char readBuffer[BUFFER_SIZE] = {0};
        // read(pipefd[0],readBuffer ,sizeof(readBuffer));
        // printf("parent readBuffer = %s\n",readBuffer);

        wait(NULL);//回收
    }
    else if (pid== 0)
    {
        close(pipefd[1]);
        sleep(3);
        cout<< "I am child"<<endl;
        int flags=fcntl(pipefd[0],F_GETFL);//得到文件描述符的状态标记
        cout<< "flags:"<<flags<<endl;

        //设置文件描述符为非阻塞
        fcntl(pipefd[0],F_SETFL,flags | O_NONBLOCK);
        //子进程负责读（read是阻塞函数；没有函数是阻塞的，只有文件描述符是阻塞的）
        char buffer[BUFFER_SIZE] = {0};
        read(pipefd[0],buffer,sizeof(buffer));
        
        cout<< "hello world"<<endl;
        printf("buffer = %s\n",buffer);
    }

    else if(pid< 0)
    {
        perror("fork error:");
        exit(-1);
    }

    return 0;
}