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