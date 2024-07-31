#ifndef __BINARY_SEARCH_TREE
#define __BINARY_SEARCH_TREE

#define ELEMENT_TYPE void *

typedef struct BinarySearchNode
{
    ELEMENT_TYPE data;//数据域
    struct BinarySearchNode * left;
    struct BinarySearchNode * right;
    struct BinarySearchNode * parent;
    //指针域
}BinarySearchNode;


typedef struct BinarySearchTree
{
    //根节点
    BinarySearchNode * root;
    int size;//根的元素个数


    //函数指针  回调函数
    int (*compareFunc)(ELEMENT_TYPE arg1,ELEMENT_TYPE arg2);
}BinarySearchTree;
//初始化树
int binarySearchTreeInit(BinarySearchTree **pTree,int (*compareFunc)(ELEMENT_TYPE arg1,ELEMENT_TYPE arg2));
//插入树
int binarySearchTreeInsert(BinarySearchTree *pTree,ELEMENT_TYPE data);
#endif
