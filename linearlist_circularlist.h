//
//  linearlist_circularlist.h
//  C++
//
//  Created by 木林 on 2023/10/23.
//

#ifndef linearlist_circularlist_h
#define linearlist_circularlist_h

#include <memory>
#include <iostream>
#include "linearlist_abstract.h"

template<typename T>
struct Node {
    T value;
    Node<T>* next;
    Node<T>* prev;

    explicit Node(T val = T()) : value(val), next(nullptr),prev(nullptr) {}
};

template<typename T>
class CircularLinkedList : public linearlist<T> {
private:
    Node<T>* head;
    Node<T>* tail;
    int listsize;
public:
    class myiterator;

    friend class myiterator;

    CircularLinkedList() {
        try{
            head=new Node<T>();
            tail=new Node<T>();
        }catch (const std::bad_alloc& e) {
            std::cout<<"新建链表时内存不足"<<e.what()<<std::endl;
        }
        listsize = 0;
    }

    ~CircularLinkedList(){
        if (listsize<=2){
            delete head->next;
            delete head;
            delete tail->next;
            delete tail;
            return ;
        }
        Node<T>* tmp=head->next->next;
        Node<T>* old=head->next;
        while(tmp!=tail->next){
            delete old;
            old=tmp;
            tmp=tmp->next;
        }
        delete head;
        delete tail->next;
        delete tail;
    }

    myiterator begin() { return myiterator(head->next); }

    myiterator end() {
        /* 注意，返回的是最后一个元素的迭代器*/
        return myiterator(tail->next);
    }

    void push_back(T value);

    [[nodiscard]] int remove(T value);//成功返回0，否则1

    void insert(int index, const T &x);

    [[nodiscard]]virtual bool empty() const { return !static_cast<bool>(listsize); }

    [[nodiscard]]virtual int size() const { return listsize; }

    [[nodiscard]]virtual T &get(int index) const;

    [[nodiscard]]virtual int indexOf(const T &find_element) const;

    virtual void erase(int index);//根据索引删除

    virtual std::ostream &output(std::ostream &out) const;

    class myiterator {
    private:
        Node<T>* current;
    public:
        explicit myiterator(Node<T>* node)
                : current(node) {}

        myiterator(const myiterator &t) : current(t.current) {}

        T &operator*() { return current->value; }

        int operator-(const myiterator &other) const {
            int result = 0;
            Node<T>* _ptr=current;
            while (_ptr != other.current) {
                _ptr = _ptr->prev;
                result++;
            }
            return result;
        }

        myiterator &operator++() {
            current = current->next;
            return *this;
        }

        myiterator operator++(int) {
            myiterator tmp = *this;
            current = current->next;
            return tmp;
        }

        myiterator &operator--() {
            current = current->prev;
            return *this;
        }

        myiterator operator--(int) {
            myiterator tmp = *this;
            current = current->prev;
            return tmp;
        }

        bool operator==(const myiterator &other) const {
            return current == other.current;
        }

        bool operator!=(const myiterator &other) const {
            return current != other.current;
        }
    };
};


template<class T>
void CircularLinkedList<T>::push_back(T value) {
    insert(listsize,value);
}

template<class T>
int CircularLinkedList<T>::remove(T value) {
    Node<T>* node = head->next;
    Node<T>* prevNode = head;

    for (int i = 0; i < listsize; ++i) {
        if (node->value == value) {
            // Remove node
            if (listsize==1){//只有一个元素则直接删除
                delete node;
                head= nullptr;
                tail= nullptr;
                listsize--;
                return 0;
            }
            prevNode->next = node->next;
            node->next->prev = prevNode;

            listsize--;
            delete node;
            return 0;
        }
        prevNode = node;
        node = node->next;
    }
    return 1;//没有符合的元素
}

template<class T>
void CircularLinkedList<T>::insert(int index, const T &x) {
    assert(index >= 0 && index <= listsize);
    Node<T>* insert_ptr = head->next;
    Node<T>* prev_ptr=head;
    Node<T>* newNode;

    try {
        newNode=new Node<T>(x);
    } catch (const std::bad_alloc &e) {
        std::cerr << "在insert中创建新节点时内存不足:" << e.what();
    }

    if (listsize==0){//特判
        head->next=newNode;
        newNode->prev=newNode;
        tail->next=newNode;
        newNode->next=newNode;
    }else if (index==listsize){
        Node<T>* oldLastNode=tail->next;

        oldLastNode->next=newNode;
        newNode->prev=oldLastNode;
        newNode->next=head->next;
        head->next->prev=oldLastNode;
        tail->next=newNode;
    }else {
        for (int i = 0; i < index; ++i) {
            insert_ptr = insert_ptr->next;
            prev_ptr=prev_ptr->next;
        }
        prev_ptr->next=newNode;
        newNode->prev=prev_ptr;
        newNode->next=insert_ptr;
        insert_ptr->prev=newNode;
    }
    listsize++;
}


template<class T>
T &CircularLinkedList<T>::get(int index) const {
    assert(index >= 0 && index < listsize);
    Node<T>* ptr = head->next;
    for (int i = 0; i < index; ++i) {
        ptr = ptr->next;
    }
    return ptr->value;
}

template<class T>
int CircularLinkedList<T>::indexOf(const T &find_element) const {
    Node<T>* ptr = head->next;
    int position = 0;
    for (int i = 0; i < listsize; ++i){
        if (ptr->value == find_element) {
            return position;
        }
        ptr = ptr->next;
        position++;
    }
    return -1;//没找到
}

template<class T>
void CircularLinkedList<T>::erase(int index) {
    assert(index >= 0 && index < listsize);
    Node<T>* ptr = head->next;
    Node<T>* prev = head;

    if (listsize==1){//只有一个元素，直接删除
        delete ptr;
        head= nullptr;
        tail= nullptr;
        listsize--;
        return;
    }

    for (int i = 0; i < index; ++i) {
        ptr = ptr->next;
        prev = prev->next;
    }
    prev->next = ptr->next;
    ptr->next->prev = prev;
    delete ptr;
    listsize--;
}

template<class T>
std::ostream &CircularLinkedList<T>::output(std::ostream &out) const {
    if (listsize==0){
        out<<"空链表"<<std::endl;
        return out;
    }
    Node<T>* ptr=head->next;
    for (int i=0;i<listsize;i++){
        out<<ptr->value;
        ptr=ptr->next;
    }
    return out;
}

#endif /* linearlist_circularlist_h */
