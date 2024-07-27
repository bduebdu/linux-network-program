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
    int fd = open(filename, O_RDWR | O_CREAT);
    if (fd==-1)
    {
        perror("open error:");     
    }
    cout << "fd:" << fd << endl;

    string name = "zhangsan";

    size_t writeBytes = write (fd,(const void *)name.c_str(),name.size());
    close(fd);
    return 0;
}