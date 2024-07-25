#include <iostream>

using namespace std;

#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    const char * filename = "./111.txt";
    cout << "-----:"<< access(filename,F_OK)<<endl;
    return 0;
}