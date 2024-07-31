#include "binarySearchTree.h"
#include <stdio.h>
#include<stdlib.h>

enum STATUS_CODE
{
    ON_SUCCESS,
    NULL_PTR,
    MALLOC_ERROR,
    ACCESS_INVAILD,
};

//初始化树
int binarySearchTreeInit(BinarySearchTree **pTree,int (*compareFunc)(ELEMENT_TYPE arg1,ELEMENT_TYPE arg2))
{
    BinarySearchTree * tree = (BinarySearchTree *)malloc(sizeof(BinarySearchTree)*1);
    if(tree == NULL)
    {
        return MALLOC_ERROR;
    }
    //清除脏数据
    memset(tree,0,sizeof(BinarySearchTree)*1);
    //树的元素个数为0
    tree->size = 0;
    tree->root = NULL;
    //比较器
    tree->compareFunc = compareFunc;



    //二级指针解引用
    *pTree =tree;
    return ON_SUCCESS;
}


static BinarySearchNode *createBinarySearchTreeNode(ELEMENT_TYPE data,BinarySearchNode * parent)
{
    BinarySearchNode *newNode = (BinarySearchNode *)malloc(sizeof(BinarySearchNode)*1);
        if(newNode ==NULL)
        {
            return NULL;
        }
        //清除脏数据
        memset(newNode,0,sizeof(BinarySearchNode)*1);
        newNode->data= data;
        newNode->parent = NULL;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
}

//插入树
int binarySearchTreeInsert(BinarySearchTree *pTree,ELEMENT_TYPE data)
{
    if(pTree == NULL)
    {
        return NULL_PTR;
    }
    if(pTree-> root ==NULL)//空树
    {
        pTree->root = createBinarySearchTreeNode(data,NULL);
    if(pTree-> root ==NULL)
    {
        return NULL_PTR;
    }
        //树的元素个数+1
        (pTree->size)++;
        return ON_SUCCESS;
    }

    //程序至此一定不是空树
    BinarySearchNode * travelNode = pTree->root;
    
    BinarySearchNode * parentNode = NULL;
    int cmp =0;
    while(travelNode!= NULL)
    {
        cmp = pTree->compareFunc(data,travelNode->data);
        if(cmp<0)
        {
            travelNode = travelNode->left;
        }
        else if (cmp>0)
        {
            travelNode = travelNode->right;
        }
        else if (cmp == 0)
        {
            travelNode->data =data;
        }

    }

    BinarySearchNode * newNode = createBinarySearchTreeNode(data,parentNode);
    if(newNode==NULL)
    {
        return NULL_PTR;
    }
    if(cmp>0)
    {
        parentNode->right = newNode;
    }
    else if(cmp<0)
    {
        parentNode->left = newNode;
    }

    //退出循环
    //元素个数加1
    pTree->size++;
    return ON_SUCCESS;
}