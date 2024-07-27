#include <iostream>

using namespace std;

#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{

    const char * filename = "./demoAppend.cpp";

    struct stat fileinfo;


    stat(filename,&fileinfo);


    return 0;
}