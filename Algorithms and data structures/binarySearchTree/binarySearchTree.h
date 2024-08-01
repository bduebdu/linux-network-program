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


    //函数指针  回调函数  比较器
    int (*compareFunc)(ELEMENT_TYPE arg1,ELEMENT_TYPE arg2);
    //打印器
    int (*printFunc)(ELEMENT_TYPE arg);
}BinarySearchTree;
//初始化树
int binarySearchTreeInit(BinarySearchTree **pTree,int (*compareFunc)(ELEMENT_TYPE arg1,ELEMENT_TYPE arg2),int(*printFunc)(ELEMENT_TYPE arg));
//插入树
int binarySearchTreeInsert(BinarySearchTree *pTree,ELEMENT_TYPE data);
//树的遍历（前中后序遍历）
int binarySearchTreePreForech(BinarySearchTree *ptree);

int binarySearchTreeInOrderForech(BinarySearchTree *ptree);

int binarySearchTreePostOrderForech(BinarySearchTree *ptree);
//层序遍历
int binarySearchTreeLevelOrderForech(BinarySearchTree *ptree);
#endif
