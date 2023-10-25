//
//  tree_list.h
//  C++
//
//  Created by 木林 on 2023/10/23.
//

#ifndef tree_list_h
#define tree_list_h

#include<cassert>
#include<iostream>
#include<list>
#include<vector>
#include<stack>



template<class T>
struct binaryTreeNode {//标志位是为了方便转成线索树
    T element;
    binaryTreeNode<T> *leftChild, *rightChild;
    bool lThread,rThread;//左右标志位,0代表指针,1代表前驱或者后继
    
    binaryTreeNode(const T& value=0,binaryTreeNode<T> *l=nullptr,binaryTreeNode<T> *r=nullptr,bool lT=false,bool rT=false) : element(value), leftChild(l), rightChild(r),lThread(lT),rThread(rT) {}
};

template<class T>
class binaryTreeList {
public:
    binaryTreeList() : e(nullptr) {}
    
    binaryTreeList(std::initializer_list<T> t):e(nullptr) {
        for (auto &x: t) {
            insert(e, x);
        }
    }
    
    binaryTreeList(const binaryTreeList<T>&);
    
    binaryTreeList(const std::vector<T>& preOrder_or_postOrder,const std::vector<T>& inOrder,bool preOrder_or_postOrder_flag);//先序/后序与中序遍历序列建立二叉树
    
    binaryTreeList<T>& operator=(const binaryTreeList<T>&);
    
    ~binaryTreeList() { clearTree(e); }
    
    int count_leaf(){return count_leaf(e);}//统计整棵树的叶子数
    
    void insert(binaryTreeNode<T> *&, const T &);
    
    void clearTree(binaryTreeNode<T> *node);
    
    bool empty() { return e == nullptr; }
    
    binaryTreeNode<T>* root(){return e;}
    
    T& leftChild(binaryTreeNode<T>* node){assert(node->leftChild!= nullptr);return node->leftChild->element;}
    
    T& rightChild(binaryTreeNode<T>* node){assert(node->rightChild!= nullptr);return node->rightChild->element;}
    
    void insertChild(binaryTreeNode<T>* node,bool left,bool right,const T& value);
    
    void deleteChild(binaryTreeNode<T>* node,bool left,bool right){
        if (left){
            clearTree(node->leftChild);
        }
        if(right){
            clearTree(node->rightChild);
        }
    }
    
    void print_no_recursion(bool preOrder,bool inOrder,bool postOrder){ print_no_recursion(e,preOrder,inOrder,postOrder);}
    
    void print_recursion(bool preOrder,bool inOrder,bool postOrder){ print_recursion(e,preOrder,inOrder,postOrder);}
    
    int depth(){return depth(e);}//整棵树的深度
    
private:
    int depth(binaryTreeNode<T> *node);
    void print_recursion(binaryTreeNode<T>* node,bool preOrder,bool inOrder,bool postOrder);//递归遍历
    void print_no_recursion(binaryTreeNode<T>* node,bool preOrder,bool inOrder,bool postOrder);//非递归遍历
    int count_leaf(binaryTreeNode<T>* node);//统计某个节点以下的树的叶子数
    binaryTreeNode<T> * build_tree_preorder_inorder_recursion(const std::vector<T>&,int pStart,int pEnd,const std::vector<T>&,int iStart,int iEnd);
    
    binaryTreeNode<T> * build_tree_postorder_inorder_recursion(const std::vector<T>&,int pStart,int pEnd,const std::vector<T>&,int iStart,int iEnd);
    binaryTreeNode<T>* copy_tree(const binaryTreeNode<T>* other);
    binaryTreeNode<T> *e;//头节点
};

template<class T>
binaryTreeList<T>& binaryTreeList<T>::operator=(const binaryTreeList<T>& other){
    e=copy_tree(other.e);
}

template<class T>
binaryTreeList<T>::binaryTreeList(const std::vector<T>& preOrder_or_postOrder,const std::vector<T>& inOrder,bool preOrder_or_postOrder_flag){
    /*
     第三参数值为true:先序遍历
     */
    if(preOrder_or_postOrder_flag)
    {
        e=build_tree_preorder_inorder_recursion(preOrder_or_postOrder, 0, (int)preOrder_or_postOrder.size(), inOrder, 0, (int)inOrder.size());
    }else{
        e=build_tree_postorder_inorder_recursion(preOrder_or_postOrder, 0, (int)preOrder_or_postOrder.size(), inOrder, 0, (int)inOrder.size());
    }
}

template <class T>
binaryTreeNode<T>* copy_tree(const binaryTreeNode<T>* other) {
    if (other==nullptr) {
        return nullptr;
    }else{
        return new binaryTreeNode<T>(other->element,copy_tree(other->leftChild),copy_tree(other->rightChild),other->lThread,other->rThread);
    }
}

template<class T>
binaryTreeList<T>::binaryTreeList(const binaryTreeList<T>& t){
    e=copy_tree(t.e);
}

template<class T>
binaryTreeNode<T>* binaryTreeList<T>::build_tree_postorder_inorder_recursion(const std::vector<T> & postOrder, int pStart, int pEnd, const std::vector<T> & inOrder, int iStart, int iEnd){
    
    if (pStart==pEnd) {//递归出口
        return nullptr;
    }
    
    binaryTreeNode<T>* root=new binaryTreeNode<T>(postOrder[pEnd-1]);//这里和先序序列不一样
    int root_inOrder_index=0;
    
    for (int i=iStart; i<iEnd; ++i) {//找到中序序列的根位置
        if (inOrder[i]==postOrder[pEnd-1]) {
            root_inOrder_index=i;
            break;
        }
    }
    
    int leftTreeChildNum=root_inOrder_index-iStart;
    
    root->leftChild=build_tree_postorder_inorder_recursion(postOrder, pStart, pStart+leftTreeChildNum, inOrder, iStart, root_inOrder_index);
    root->rightChild=build_tree_postorder_inorder_recursion(postOrder, pStart+leftTreeChildNum, pEnd-1, inOrder, root_inOrder_index+1, iEnd);//pEnd不包括最后一项(根节点),iStart不包括根节点
    
    return root;
}

template<class T>
binaryTreeNode<T> * binaryTreeList<T>::build_tree_preorder_inorder_recursion(const std::vector<T> & preOrder, int pStart, int pEnd, const std::vector<T> & inOrder, int iStart, int iEnd){
    
    //递归出口
    if (pStart==pEnd) {
        return nullptr;//递归中，对空树返回空指针
    }
    //在preOrder中找到根节点(第一个)，在inOrder中找到对应的根节点(注意是每次递归中的根节点，不是整棵树的)，从而找到左子树
    binaryTreeNode<T>* root=new binaryTreeNode<T>(preOrder[pStart]);
    int root_index_inOrder = 0;//在中序序列中找到本次递归的根节点位置
    for (int i=iStart; i<iEnd; ++i) {
        if (inOrder[i]==preOrder[pStart]) {
            root_index_inOrder=i;
            break;
        }
    }
    
    int leftTreeChildNum=root_index_inOrder-iStart;//某个根节点的左孩子个数
    
    //易错：注意此处pStart\pEnd\iStart\iEnd的值，要保证是本次递归中的左右序列边界
    root->leftChild=build_tree_preorder_inorder_recursion(preOrder, pStart+1, pStart+1+leftTreeChildNum, inOrder, iStart, root_index_inOrder);//iEnd不包括根节点
    root->rightChild=build_tree_preorder_inorder_recursion(preOrder, pStart+1+leftTreeChildNum, pEnd, inOrder, root_index_inOrder+1, iEnd);
    
    return root;
}

template<class T>
void binaryTreeList<T>::print_no_recursion(binaryTreeNode<T> *node, bool preOrder, bool inOrder, bool postOrder){
    assert((preOrder&&!inOrder&&!postOrder)||(!preOrder&&inOrder&&!postOrder)||(!preOrder&&!inOrder&&postOrder));//保证只有一个为1
    
    std::stack<binaryTreeNode<T>*> s;
    std::vector<T> result;
    binaryTreeNode<T>* currentNode=e;//从整棵树的根节点开始
    
    if (preOrder) {
        //先序遍历：在访问到某个节点的时候就存储到结果数组当中
        while (currentNode!=nullptr||!s.empty()) {//注意这里是或，不是与
            while(currentNode!=nullptr){
                //先序遍历要求访问根节点后先遍历左子树，所以需要一个循环，一直遍历到最左下的节点
                result.push_back(currentNode->element);//每经过一个节点就入结果数组，并入栈
                s.push(currentNode);
                currentNode=currentNode->leftChild;//向左子树遍历
            }
            //由于上面的循环结束条件的限制，程序执行到这里时currentNode一定等于nullptr
            //所以需要再次更新currentNode，让他指向栈顶元素
            currentNode=s.top();s.pop();
            currentNode=currentNode->rightChild;//此时父节点和左子树都已经遍历完成，向右子树进行遍历
        }
    }else if(inOrder){
        //中序遍历：在访问到某个节点的时候先入栈，一直到树的最深处再开始存到结果数组
        while (currentNode!=nullptr||!s.empty()) {//注意这里是或，不是与
            while(currentNode!=nullptr){
                //中序遍历要求先遍历左子树，所以需要一个循环，一直遍历到最左下的节点
                //这里和先序遍历不一样的地方就是这里不将节点元素值存入结果数组
                s.push(currentNode);
                currentNode=currentNode->leftChild;//向左子树遍历
            }
            //由于上面的循环结束条件的限制，程序执行到这里时currentNode一定等于nullptr
            //所以需要再次更新currentNode，让他指向栈顶元素
            currentNode=s.top();s.pop();
            result.push_back(currentNode->element);//注意：结果数组操作移到这里
            currentNode=currentNode->rightChild;//此时父节点和左子树都已经遍历完成，向右子树进行遍历
        }
    }else{
        //后序遍历：在中序遍历的基础上，需要判断是否已经扫描过左子树和右子树。如果全部扫描完毕，才能将父节点的元素值存储到结果数组中
        binaryTreeNode<T>* lastVisit=nullptr;//用于存储被访问过的右子树节点
        
        while (currentNode!=nullptr||!s.empty()) {//注意这里是或，不是与
            while(currentNode!=nullptr){
                //基础循环和中序遍历一致
                s.push(currentNode);
                currentNode=currentNode->leftChild;
            }
            
            //由于上面的循环结束条件的限制，程序执行到这里时currentNode一定等于nullptr
            //所以需要再次更新currentNode，让他指向栈顶元素
            currentNode=s.top();//先不能弹出栈顶元素，先判断右子树有没有访问过
            if (currentNode->rightChild==nullptr||
                lastVisit==currentNode->rightChild) {
                //如果栈顶元素没有右子节点/右子节点已经被访问过
                //才能访问父节点，并出栈
                result.push_back(currentNode->element);
                s.pop();
                //下面这行代码容易忘记
                lastVisit=currentNode;//访问了父节点，更新lastvisit
                currentNode=nullptr;//这个元素出栈了，currentNode不能再指向它
            }else{
                currentNode=currentNode->rightChild;
                lastVisit=currentNode;//访问了右子节点之后，需要记录下来(易错)
            }
        }
    }
    
    for(auto& e:result){
        std::cout<<e<<" ";
    }
}

template<class T>
int binaryTreeList<T>::count_leaf(binaryTreeNode<T>* node) {
    if (node== nullptr) {
        return 0;
    }else if (node->leftChild== nullptr&&node->rightChild== nullptr){
        return 1;
    }else{
        return count_leaf(node->leftChild)+ count_leaf(node->rightChild);
    }
}

template<class T>
void binaryTreeList<T>::print_recursion(binaryTreeNode<T>* node,bool preOrder, bool inOrder, bool postOrder) {
    assert((preOrder&&!inOrder&&!postOrder)||(!preOrder&&inOrder&&!postOrder)||(!preOrder&&!inOrder&&postOrder));//保证只有一个为1
    
    if (preOrder){
        if (node== nullptr)return;
        std::cout<<node->element<<" ";
        print_recursion(node->leftChild,preOrder,inOrder, postOrder);
        print_recursion(node->rightChild,preOrder,inOrder,postOrder);
    }else if (inOrder){
        if (node== nullptr)return;
        print_recursion(node->leftChild,preOrder,inOrder,postOrder);
        std::cout<<node->element<<" ";
        print_recursion(node->rightChild,preOrder,inOrder,postOrder);
    }else if(postOrder){
        if (node== nullptr)return;
        print_recursion(node->leftChild,preOrder,inOrder,postOrder);
        print_recursion(node->rightChild,preOrder,inOrder,postOrder);
        std::cout<<node->element<<" ";
    }
}

template<class T>
void binaryTreeList<T>::insertChild(binaryTreeNode<T> *node, bool left, bool right, const T &value) {//left和right为1代表要插入的子树
    /*
     * 必须保证一开始传入的node不能是nullptr
     */
    if (node== nullptr){
        node=new binaryTreeNode<T>;
        node->element=value;
    }else{//可能同时插入左子树和右子树
        if (left){
            insertChild(node->leftChild,left,right,value);
        }
        if (right){
            insertChild(node->rightChild,left,right,value);
        }
    }
}

template<class T>
int binaryTreeList<T>::depth(binaryTreeNode<T> *node) {
    if (node == nullptr) {
        return 0;//节点为空，深度为0
    } else {
        int leftDepth = depth(node->leftChild);
        int rightDepth = depth(node->rightChild);
        return std::max(leftDepth, rightDepth) + 1;
    }
}

template<class T>
void binaryTreeList<T>::clearTree(binaryTreeNode<T> *node) {
    if (node != nullptr) {
        clearTree(node->leftChild);//先删除左子树和右子树，再删除父节点
        clearTree(node->rightChild);
        delete node;
        node = nullptr;
    }
}

template<class T>
void binaryTreeList<T>::insert(binaryTreeNode<T> *&node, const T &value) {
    if (node == nullptr) {
        try {
            node = new binaryTreeNode<T>;
        } catch (const std::bad_alloc &e) {
            std::cerr << e.what() << std::endl;
            return;
        }
        node->element = value;
        return;
    }
    
    //一个节点的子节点只有三种情况：没有，只有左，左右都有
    if (node->leftChild == nullptr && node->rightChild == nullptr) {
        insert(node->leftChild, value);
    } else if (node->rightChild == nullptr) {
        insert(node->rightChild, value);
    } else {
        insert(node->leftChild, value);
    }
}


#endif /* tree_list_h */
