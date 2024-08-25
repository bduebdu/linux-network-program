#include "memoryPool.h"
#include <iostream>
#include <cstring>
using namespace std;
//构造函数
MemoryPool::MemoryPool()
{
   m_blockHeader = nullptr;
   m_nodeHeader = nullptr;
}
//申请内存
void * MemoryPool::allocateMem()
{
    //如果没有空闲的内存块，则申请一个新的内存块
    if(m_nodeHeader == nullptr)
    {
        Block * newBlock = new Block;
        memset(newBlock, 0, sizeof(Block));
        for(int idx = 0; idx < NodeCount-1; idx++)
        {
            newBlock->Node[idx].next = &(newBlock->Node[idx+1]);
        }
        newBlock->Node[NodeCount-1].next = nullptr;
        //头插法
        newBlock->next = m_blockHeader;
        m_blockHeader = newBlock;
        //连接
        m_nodeHeader = &(m_blockHeader->Node[0]);
    }
    //连接
    auto ptr = m_nodeHeader;
    memset(ptr, 0, sizeof(UseNode));
    m_nodeHeader = m_nodeHeader->next;
    return ptr;
}

//释放内存
void MemoryPool::freeMem(void * ptr)
{
    UseNode *node = static_cast<UseNode *>(ptr);
    memset(node, 0, sizeof(UseNode));
    //头插法
    node->next = m_nodeHeader;
    m_nodeHeader = node;
}


//析构函数
MemoryPool::~MemoryPool()
{
    while(m_blockHeader != NULL)
    {
        auto deleteNode = m_blockHeader;
        m_blockHeader = m_blockHeader->next;
        delete deleteNode;
        deleteNode = nullptr;
    }

}