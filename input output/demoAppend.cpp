#include <iostream>

using namespace std;

#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


void writeFile(string & content)
{
    const char * filename ="write.txt";
    int fd = open(filename, O_RDWR | O_CREAT ,0644);
    if (fd==-1)
    {
        perror("error:");
    }
    off_t offset = lseek(fd,0,SEEK_END);
    size_t writeBytes = write (fd,(const void*)content.c_str(),content.size());
    close(fd);
}

void writeFile(string & content, int flag)
{
    const char * filename ="write.txt";
    int fd = open(filename, O_RDWR | O_CREAT | O_APPEND,0644);
    if (fd==-1)
    {
        perror("error:");
    }
    size_t writeBytes = write (fd,(const void*)content.c_str(),content.size());
    close(fd);
    
}

int main ()
{
    string content;
    while (1)
    {
        cout << "输入内容" ;
        cin >> content;
        if(content =="break")
        {
            break;
        }
        writeFile(content);
    }

}