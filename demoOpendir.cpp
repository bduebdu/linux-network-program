#include <iostream>
using namespace std;
#include <dirent.h>
#include <sys/types.h>
#include <cstdio>

int main()
{
    const char* filepath = "~/桌面/c++";
    DIR * dir = opendir(filepath);
    if(dir==NULL)
    {
        perror("open error");
        return -1;
    }
    //读取文件夹内容
    struct dirent *content = NULL;
    while((content = readdir(dir))!=NULL)
    {
        if((int)content->d_type ==8)
        {
            cout<<"name:"<< content->d_name << "type:"<< (int)content->d_type << endl;
        }
    }
    


    closedir(dir);
    return 0;
}