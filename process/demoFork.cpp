#include<iostream>
using namespace std;
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

static int writeFile(const char * filename, const void * buf, size_t size)
{
    int fd = open (filename,O_RDWR | O_CREAT, 0644);
    if (fd ==-1)
    {
        perror("open error");
        return -1;
    }
    int writefile = write(fd, buf, size );
    close(fd);

    return writefile;
}

static int readFile(const char * filename, void * buf, size_t size)
{
    int fd = open (filename,O_RDWR );
    if (fd ==-1)
    {
        perror("open error");
        return -1;
    }

    int readBytes = read(fd, buf,size); 
     close(fd);
    return readBytes;
}

int main()
{
    int num =100;
    //子进程和父进程执行顺序没有必然关系
   //vfork与fork的作用基本相同，但fork让子进程先执行
    pid_t pid = fork();
    if(pid > 0)
    {
        num += 666;
        printf("parent process - num : %d\n",num);
        //父进程的程序
    
        printf("parent process child pid = %d,pid = %d\n",pid,getpid());
        writeFile("./test.txt",&num, sizeof(num));
    }
    else if (pid== 0)
    {
        int buffer = 0;
        sleep(2);
        readFile("./test.txt", &buffer, sizeof(buffer));
        printf("child process - num : %d\n",buffer);
        //子进程代码
        printf("child process pid = %d,ppid = %d\n",getpid(), getppid() );

        int buffer = 0;
        int ret = readFile();
        if(ret == -1)
        {
            //进程退出
            int exitStatus = 11;
            _exit(exitStatus);
        }
        printf("buffer = %d\n",buffer);
    }

    else if(pid< 0)
    {
        perror("fork error:");
        exit(-1);
    }
    //回收子进程
    int childStatus = 0;
    wait(&childStatus);
    printf("childStatus = %d\n",childStatus);
    printf("hello world!"); 
    

    return 0;
}