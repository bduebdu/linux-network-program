#include <iostream>
using namespace std;
#include "memoryPool.h"
#include <cstring>

int main()
{
    MemoryPool pool;
    char * ptr = (char *)pool.allocateMem();
    strcpy(ptr, "hello world");
    cout <<"ptr:"<<ptr << endl;
    pool.freeMem(ptr);
    return 0;
}