#include<iostream>
using namespace std;
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
int main()
{
    pid_t pid = fork();
    if (pid > 0)
    {
        printf("I am parent");
        int cnt =120;
         while(cnt --)
        {
            sleep(1);
        }
    }
    else if (pid == 0)
    {//子进程不会回收
        int cnt = 5;
        while(cnt --)
        {
            sleep(1);
        }
        _exit(0);
    }
    else
    {
        perror("fork error");
    }
}