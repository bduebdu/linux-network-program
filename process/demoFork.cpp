#include<iostream>
using namespace std;

#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static int writeFile(const char * filename, const void *buf,size_t size)
{
    int fd = open(filename, O_RDWR | O_CREAT,0644);
    if(fd==1)
    {
        perror("open error");
        return -1;
    }
    int writeBytes = write(fd,buf ,size);
    close(fd);
    return writeBytes;
}
static int readFile(const char * filename, const void *buf,size_t size)
{
    int fd = open(filename, O_RDWR );
    if(fd==-1)
    {
        perror("open error");
        return -1;
    }
    int readBytes = write(fd,buf ,size);
    close(fd);
    return readBytes;
}
 int main ()
 {
    int num = 100;
    pid_t pid = fork();
    if(pid > 0)
    {
        num+=666;
        printf("parent process-num:%d\n",num);
        printf("parent process - child pid = %d, pid = %d\n", pid,getpid());
        
        writeFile("./record.txt", &num,sizeof(num));
    }
    else if(pid == 0)
    {
        sleep(3);
        printf("child process-num: %d\n",num);
        printf("child process - pid = %d\n, ppid = %d\n", getpid(),getppid());

        int buffer = 0;
        readFile("./record.txt",&buffer,sizeof(buffer));
        
        printf("buffer = %d\n",buffer);
    }
    else if (pid < 0)
    {

        perror("fork error:");
        exit(-1);
    }
       printf("hello world\n");
    while (1)
    {
        sleep(1);
    }
    return 0;
    
 }