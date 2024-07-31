#include <iostream>
using namespace std;
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <cstring>
#include<string>
int main ()
{
    int pipefd[2];
    pipe(pipefd);
    memset(pipefd,0,sizeof(pipefd));
    close(pipefd[0]);
    string res = "hello world";
    int flags=fcntl(pipefd[0],F_GETFL);
    write(pipefd[1],res.c_str(),res.size());


    return 0;
}