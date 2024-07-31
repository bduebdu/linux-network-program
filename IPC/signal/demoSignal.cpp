#include<iostream>
using namespace std;
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <cstring>

void handleSignal(int sig)
{
    printf("caught%d\n",sig);
}
 int main ()
 {



    pid_t pid = fork();
    if(pid > 0)
    {
        sleep(6);
        kill(pid,SIGKILL);
    }
    else if (pid == 0)
    {
        struct sigaction newact;
        memset(&newact,0,sizeof(newact));
        newact.sa_flags = 0;
        newact.sa_handler = handleSignal;
        sigemptyset(&newact.sa_mask);//清空集合


    }
    else
    {
        perror("error");
        exit(-1);
    }

    return 0;
 }