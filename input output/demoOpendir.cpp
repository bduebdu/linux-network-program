#include <iostream>
using namespace std;
#include <dirent.h>
#include <sys/types.h>
#include <cstdio>

int main()
{
    const char* filepath = "~/桌面/c++/music"; // 文件夹路径
    DIR *dir = opendir(filepath);
    if (dir == NULL)
    {
        perror("open error");
        return -1;
    }

    // 读取文件夹内容
    struct dirent *content = NULL;
    const char* target_file = "music.mp3"; // 目标文件名

    while ((content = readdir(dir)) != NULL)
    {
        if ((int)content->d_type == 8) // 判断是否为常规文件
        {
            if (strcmp(content->d_name, target_file) == 0) // 判断文件名是否匹配
            {
                cout << "Found the file: " << content->d_name << endl;
                closedir(dir);
                return 0;
            }
        }
    }

    cout << "File not found." << endl;
    closedir(dir);
    return 0;
}