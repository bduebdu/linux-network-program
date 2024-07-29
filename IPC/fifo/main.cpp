#include<iostream>
using namespace std;
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/wait.h>

#define BUFFER_SIZE 11
int main()
{
    //创建命令管道
    const char * filename = "./fifoInfo";
    int ret = mkfifo(filename,0644);
    if (ret!=0)
    {
        if(errno!= EEXIST)
        {
        perror("mkfifo error");
        exit(-1);
        }
    }

    //创建进程
    pid_t pid= fork();
    if(pid>0)
    {
        //父进程向有名管道写数据
        printf("parent process\n");
        int fd = open(filename,O_RDWR);
        if(fd ==-1)
        {
            perror("open error");
            _exit(-1);
        }
        int cnt = 3;
        const char* buf = "hello world";
        int writeBytes = 0;
        while (cnt--)
        {
            //向fifo写数据
            writeBytes = write(fd,buf,strlen(buf));
            if(writeBytes>0)
            {
                cout << "writeBytes:"<<writeBytes<<endl;
            }
            else if(writeBytes == 0)
            {
                cout << "writeBytes==0"<<endl;
            }
            else if (writeBytes<0)
            {
                perror("write error");
            }
            
        }
        wait(NULL);//回收资源
        close (fd);//关闭文件描述符
    }
    else if (pid==0)
    {
        sleep(3);
        //子进程向有名管道读数据
        cout<<"child process"<<endl;
        int fd = open(filename,  O_RDONLY);
        if(fd==-1)
        {
            perror("open error");
            _exit(-1);
        }
        // int flags=fcntl(fd,F_GETFL);//得到文件描述符的状态标记
        // //设置文件描述符为非阻塞
        // fcntl(fd,F_SETFL,flags | O_NONBLOCK);

        char buffer[BUFFER_SIZE]= {0};
        int readBytes = 0;
        while (1)
        {
            readBytes = read(fd,buffer,sizeof(buffer));
            if(readBytes>0)
            {
                printf("readBytes = %d,\t buffer = %s\n",readBytes,buffer);
            }
            else if (readBytes ==0)
            {
                cout<<"readBytes == 0"<<endl;
               //break;
            }
            else if (readBytes <0)
            {
                cout<<"readBytes <0"<<endl;
               //break;
               if(errno==EAGAIN)
               {

                break;
               }
               else
               {
                perror("read error");
                close(fd);
                _exit(-1);
               }
            }
        sleep(1);
        //清除buffer
        memset(buffer,0,sizeof(buffer));
        }   
    close(fd);
    _exit(0);
        
    }
    else
    {
        perror("fork error");
    }


}