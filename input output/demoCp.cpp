#include <iostream>
using namespace std;
#include <cstdio>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 128

int myCopyFile(const char*src,const char*dest)
{
    int fd1 = open(src,O_RDONLY);
    if(fd1==-1)
    {
        printf("file not exist");
        return -1;
    }
    int permission = 0;
    access(src,W_OK)==0? permission|=W_OK :permission;
    access(src,R_OK)==0? permission|=R_OK :permission;
    access(src,X_OK)==0? permission|=X_OK :permission;
  
    int fd2 = open(dest,O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if(fd2==-1)
    {
        printf("open error");
        return -1;
    }

    char buffer [BUFFER_SIZE] = {0};

    while(1)
    {
        size_t readBytes = read(fd1,buffer,sizeof(buffer)-1);
        if(readBytes<=0)
        {
            break;
        }
        write(fd2,buffer,readBytes);
    }
    //回收资源
    close(fd1);
    close(fd2);
    return 0;
}

int main (int argc,const char *argv[])
{
    if (argc !=3)
    {
        printf("input too few");
        return -1;        
    }
    const char * srcFile = argv[1];
    const char * dstFile = argv[2];

    myCopyFile(srcFile,dstFile);

    return 0;
}