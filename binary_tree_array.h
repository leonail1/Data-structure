//
//  tree_array.h
//  C++
//
//  Created by 木林 on 2023/10/23.
//

#ifndef tree_array_h
#define tree_array_h

#include <vector>
#include<assert.h>
#include <iostream>
using std::vector;

template<class T>
class BinaryTree_Array{//数组描述的二叉树
public:
    BinaryTree_Array(int size=1){tree.resize(size,NULL);}
    BinaryTree_Array(std::initializer_list<T>& t) {tree(t);}
    BinaryTree_Array(const BinaryTree_Array<T>& t){tree=t.element;}
    BinaryTree_Array<T>& operator=(const BinaryTree_Array<T>& t){tree=t.element;}

    void clear()noexcept{tree.clear();}

    bool empty()noexcept{return tree.empty();}

    int depth(){assert(!empty());return (int)log2(tree.size())+1;}

    T& root()noexcept{return tree[0];};

    T& value(int index){ assert(index>=0&&index<tree.size());return tree[index];};

    T& parent(int index){
        if (tree.size()==1) {
            throw "No.1 node has no parent,is root node";
        }else{
            return tree[index/2];//节点i的父亲节点是i/2向下取整
        }
    };//返回编号为index的节点的父亲

    T& leftChild(int index){
        if (2*index>tree.size()){//index节点为叶子节点
            throw "No left child";
        }else{
            return tree[2*index];
        }
    }

    T& rightChild(int index){
        if (2*index+1>tree.size()){//index节点为叶子节点
            throw "No right child";
        }else{
            return tree[2*index+1];
        }
    }

    T& leftCouson(int index);//左右兄弟

    T& rightCouson(int index);

    void insertChild(int index,bool leftOrRight,const T& value){tree.insert(index,)};

    void deleteChile(int index,bool leftOrRight)noexcept;

private:
    vector<T> tree;
};

#endif /* tree_array_h */
