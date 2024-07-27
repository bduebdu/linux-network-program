#include<iostream>
using namespace std;

#include <unistd.h>

int main ()
{
    pid_t pid = getpid();
    cout << "pid:" << pid << endl;
    pid_t ppid = getppid();
    cout << "ppid:" << ppid << endl;
    while(1)
    {
        sleep(1);
    }
    return(0);
}