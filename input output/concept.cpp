#include <iostream>

using namespace std;

#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main ()
{
    const char * filename ="test.txt";
    //open files
    int fd = open(filename, O_RDWR | O_CREAT);
    if (fd==-1)
    {
        perror("open error:");     
    }
    cout << "fd:" << fd << endl;

    close(fd);
    return 0;
}