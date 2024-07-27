#include<iostream>
using namespace std;

#include <unistd.h>

 int main ()
 {
    int num = 100;
    pid_t pid = fork();
    if(pid > 0)
    {
        num+=666;
        printf("parent process-num:%d\n",num);
        printf("parent process - child pid = %d, pid = %d\n", pid,getpid());

    }
    else if(pid == 0)
    {
        sleep(3);
        printf("child process-num: %d\n",num);
        printf("child process - pid = %d\n, ppid = %d\n", getpid(),getppid());
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