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
        _exit(0);
    }
    else if (pid == 0)
    {
        int cnt = 80;
        while(cnt --)
        {
            sleep(1);
        }
    }
    else
    {
        perror("fork error");
    }
}