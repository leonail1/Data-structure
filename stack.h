//
//  stack.h
//  C++
//
//  Created by 木林 on 2023/10/23.
//

#ifndef stack_h
#define stack_h

#include <iostream>
#include <new>
#include <cassert>

template<class T>
class stack {
private:
    void shrink();
    void expand();
    T* element;
    int stackSize;
    int numOfElement;//元素个数
public:

    stack();
    stack(const stack<T>& t);
    stack<T>& operator=(const stack<T>& t);
    ~stack(){delete[] element;}

    [[nodiscard]]bool empty()const noexcept{return !static_cast<bool>(numOfElement);}

    [[nodiscard]]int size()const noexcept{return numOfElement;}

    void pop();

    T& top();

    void push(const T& x);


};

template<class T>
stack<T>::stack():stackSize(10),numOfElement(0) {
    try {
        element = new T[stackSize];
    } catch (const std::bad_alloc &e) {
        std::cerr << "在构造函数中内存不足" << e.what() << std::endl;
    }
}

template<class T>
stack<T>& stack<T>::operator=(const stack<T>& t){
    if (t==*this)
        return *this;
    stackSize=t.stackSize;
    numOfElement=t.numOfElement;
    try {
        element = new T[stackSize];
    }catch (const std::bad_alloc& e){
        std::cerr<<"在拷贝构造函数中内存不足"<<e.what()<<std::endl;
    }
    for (int i = 0; i < numOfElement; ++i) {
        element[i]=t.element[i];
    }
    return *this;
}

template<class T>
stack<T>::stack(const stack<T>& t):stackSize(t.stackSize),numOfElement(t.numOfElement) {

    try {
        element = new T[stackSize];
    }catch (const std::bad_alloc& e){
        std::cerr<<"在拷贝构造函数中内存不足"<<e.what()<<std::endl;
    }
    for (int i = 0; i < numOfElement; ++i) {
        element[i]=t.element[i];
    }
}

template<class T>
void stack<T>::pop() {
    assert(numOfElement>0);
    numOfElement--;
    shrink();
}

template<class T>
T& stack<T>::top(){
    assert(numOfElement>0);
    return element[numOfElement-1];
}

template<class T>
void stack<T>::push(const T& x) {
    expand();
    element[numOfElement++]=x;
}

template<class T>
void stack<T>::shrink(){
    if (numOfElement<stackSize/4 && stackSize!=10){
        T* tmp;
        try {
            tmp = new T[std::max(stackSize/2,10)];
            stackSize=std::max(stackSize/2,10);
        }catch (const std::bad_alloc& e){
            std::cerr<<"在拷贝构造函数中内存不足"<<e.what()<<std::endl;
        }
        for (int i = 0; i < numOfElement; ++i) {
            tmp[i]=element[i];
        }
        std::swap(tmp,element);
        delete tmp;
    }
}

template<class T>
void stack<T>::expand(){
    if (numOfElement==stackSize){
        T* tmp;
        try {
            tmp = new T[stackSize*2];
            stackSize*=2;
        }catch (const std::bad_alloc& e){
            std::cerr<<"在拷贝构造函数中内存不足"<<e.what()<<std::endl;
        }
        for (int i = 0; i < numOfElement; ++i) {
            tmp[i]=element[i];
        }
        std::swap(tmp,element);
        delete tmp;
    }
}

#endif /* stack_h */
