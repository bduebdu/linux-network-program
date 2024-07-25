#include <iostream>

using namespace std;

#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int main ()
{
    const char * filename ="test.txt";
    int fd = open(filename, O_RDWR | O_CREAT);
    if (fd==-1)
    {
        perror("open error:");     
    }

    char buffer[16];

    bzero(buffer,sizeof(buffer));
    size_t readBytes = read(fd,buffer,sizeof(buffer)-1);
    printf("readBytes = %ld,\tbuffer = %s\n",readBytes,buffer);
    
    off_t offset = lseek(fd,0,SEEK_CUR);
    cout <<"offset:" << offset << endl;

    readBytes = read(fd,buffer,sizeof(buffer)-1);
    printf("readBytes = %ld,\tbuffer = %s\n",readBytes,buffer);
    
    offset = lseek(fd,0,SEEK_END);

    close(fd);
    return 0;
}