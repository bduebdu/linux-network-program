#ifndef __MEMORYPOOL_H__
#define __MEMORYPOOL_H__

#define NodeSize 8
#define NodeCount  1024
class MemoryPool
{
public:
    MemoryPool();
    ~MemoryPool();

public:
    //申请内存(相同大小)
    void * allocateMem();
    //释放内存
    void freeMem(void * ptr);
private:
    struct UseNode
    {
        char data[NodeSize];
        UseNode * next;
    };

    struct Block
    {
        UseNode Node[NodeCount];
        Block * next;
    };
    //大内存块
    Block * m_blockHeader;
    //小内存块
    UseNode * m_nodeHeader;
};




#endif // __MEMORYPOOL_H__
