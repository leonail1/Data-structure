//
//  threaded_binary_tree.h
//  C++
//
//  Created by 木林 on 2023/10/25.
//

#ifndef threaded_binary_tree_h
#define threaded_binary_tree_h
#include "binary_tree_list.h"
#define Thread true
#define Link false


template <class T>
class threadedBinaryTree_inOrder {
    binaryTreeNode<T>* headNode_flag;
    void inOrder_traversal(binaryTreeNode<T>*& current,binaryTreeNode<T>* pre);
public:
    threadedBinaryTree_inOrder():headNode_flag(nullptr){}
    threadedBinaryTree_inOrder(const binaryTreeList<T>&);
};

template <class T>
void threadedBinaryTree_inOrder<T>::inOrder_traversal(binaryTreeNode<T> *& current,binaryTreeNode<T>* pre){//用于中序遍历实际二叉树，第二个参数只能赋值为headNode_flag
    
    binaryTreeNode<T> *& currentNode=current;
    binaryTreeNode<T>* preNode=pre;
    if (currentNode==nullptr) return;
    inOrder_traversal(currentNode->leftChild,pre);
    //此时,currentNode指向中序遍历的每一个节点
    
    //前驱
    if (currentNode->leftChild==nullptr) {//左指针为空，就指向前驱，并修改标志位
        currentNode->leftChild=preNode;
        currentNode->lThread=Thread;
    }else{
        currentNode->lThread=Link;
    }
    
    //后继
    if (preNode!=headNode_flag){//确保前驱不为标志头节点 //前驱的右指针为空，就将前驱右指针指向当前节点，并修改标志位
        if (preNode->rightChild==nullptr) {
            preNode->rightChild=currentNode;
            preNode->rThread=Thread;
        }else{
            preNode->rThread=Link;
        }
    }
    
    preNode=currentNode;//更新前驱节点
    inOrder_traversal(currentNode->rightChild,pre);
    
}

template <class T>
threadedBinaryTree_inOrder<T>::threadedBinaryTree_inOrder(const binaryTreeList<T>& BiTree){
    
    binaryTreeList<T> commonBinaryTree=BiTree;//拷贝一份普通二叉树
    binaryTreeNode<T>* currentNode=commonBinaryTree.root();//获取实际二叉树的根节点
    headNode_flag->leftChild=currentNode;
    headNode_flag->lThread=Link;
    headNode_flag->rThread=Thread;
    
    if (currentNode!=nullptr) {
        inOrder_traversal(currentNode, headNode_flag);
    }
    currentNode=commonBinaryTree.root();
    
    //找到中序遍历中的最后一个节点，并更新其右指针与标志头节点的右指针
    while (currentNode!=nullptr&&currentNode->rightChild!=nullptr) {
        currentNode=currentNode->rightChild;
    }
    currentNode->rightChild=headNode_flag;
    currentNode->rThread=Thread;
    headNode_flag->rightChild=currentNode;
}


#endif /* threaded_binary_tree_h */
