#include<iostream>
using namespace std;
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/wait.h>

#define BUFFER_SIZE 16

int main()
{
    const char * filename = "./commonFifo";
    int ret = access(filename,F_OK);
    
        int cnt = 3;
        const char* buf = "hello world";
        int writeBytes = 0;
        int fd = open(filename,  O_RDONLY);
        if(fd==-1)
        {
            perror("open error");
            _exit(-1);
        }
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
            sleep(1);
        }
        close (fd);//关闭文件描述符
        return 0;
}