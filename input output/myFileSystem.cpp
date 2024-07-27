#include <iostream>
using namespace std;
#include <fstream>

class MyFileSystem
{
public:
    MyFileSystem();

    MyFileSystem(string &filename, int flags, int mode);
    MyFileSystem(const char *filename, int flags, int mode);

    ~MyFileSystem();

public:
    int myOpen(string &filename, int flags, int mode);
    int myOpen(const char *filename, int flags, int mode);
    int myClose();
private:

private:
    string m_filename;
    int m_fd;
};

int main()
{
    MyFileSystem mysys;





    return 0;    
}