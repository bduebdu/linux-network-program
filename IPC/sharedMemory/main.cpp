#include<iostream>
using namespace std;
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>

#define BUFFER_SIZE 16

int main()
{
    //打开文件
    const char * filename = "./text.txt";
    int fd = open (filename,O_RDWR);
    if(fd==-1)
    {
        perror("open error");
        exit(-1);
    }

    //获取文件大小
    int length = lseek(fd,0,SEEK_END);
    printf("length = %d\n",length);
    void * addr = mmap(NULL,length,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    if (addr == MAP_FAILED)
    {
        perror("mmap error");
        exit(-1);
    }
    //关闭文件描述符
    close(fd);
    pid_t pid = fork();
    if(pid >0)
    {
        cout<< "parent process."<<endl;
        const char *str = "hello world";
        strncpy((char * )addr, str, strlen(str) + 1);
        //释放资源


    }
    else if (pid == 0)
    {
        cout<< "child process."<<endl;
        usleep(1000);
        char buffer[BUFFER_SIZE]={0};
        //地址拷贝
        memcpy(buffer,addr,sizeof(buffer));
        //读内容
        printf("addr=%s\n",(char*)addr);
    }
    else
    {
        cout << "fork error"<<endl;
        exit(-1);
    }
    //接触映射
    munmap(addr,length);



    return 0;
}