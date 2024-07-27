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

        char readBuffer[BUFFER_SIZE] = {0};
        read(pipefd[0],readBuffer ,sizeof(readBuffer));
        printf("parent readBuffer = %s\n",readBuffer);

        wait(NULL);//回收
    }
    else if (pid== 0)
    {
        close(pipefd[1]);
        sleep(3);
        cout<< "I am child"<<endl;
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