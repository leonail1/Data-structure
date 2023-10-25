//
//  queue.h
//  C++
//
//  Created by 木林 on 2023/10/23.
//

#ifndef queue_h
#define queue_h

#include <new>
#include <iostream>

template<class T>
class queue {
private:
    void shrink();
    void expand();
    T *element;
    int frontItr;//这两个指针不能大于队列长度
    int tailItr;//指向最后一个元素的下一个位置
    int numOfElement;
    int queueSize;
public:
    explicit queue(int queueSize = 10);

    ~queue(){delete[] element;}

    void output() const;

    queue(const queue<T> &t);

    queue& operator=(const queue<T> &t);

    void push(const T& x);

    void pop();

    int size(){return numOfElement;}

    bool empty(){return !static_cast<bool>(numOfElement);}

    T& front()const;

    T& back()const;
};

template<class T>
void queue<T>::output() const{
    for (int i = frontItr; i < tailItr; i=(i+1)%queueSize) {
        std::cout<<element[i]<<" ";
    }
    std::cout<<std::endl;
}

template<class T>
queue<T>& queue<T>::operator=(const queue<T> &t){
    if (*this == t)
        return *this;
    numOfElement = t.numOfElement;
    queueSize = t.queueSize;
    frontItr=0;
    tailItr=0;//只复制队列的元素，头指针位置从0开始
    try {
        element = new T[queueSize];
    } catch (const std::bad_alloc &e) {
        std::cerr << e.what() << std::endl;
    }

    for (int itr = t.front; itr%queueSize < t.tail;) {
        element[tailItr++]=t.element[(itr++)%queueSize];
    }
}

template<class T>
void queue<T>::shrink(){
    if (numOfElement<queueSize/4 && queueSize!=10){
        T* tmp;
        try {
            tmp = new T[std::max(10,queueSize/2)];
        } catch (const std::bad_alloc &e) {
            std::cerr << e.what() << std::endl;
        }

        for (tailItr=0; tailItr < numOfElement; ++tailItr) {
            tmp[tailItr]=element[frontItr];
            frontItr=(frontItr+1)%queueSize;
        }
        frontItr=0;
        std::swap(tmp,element);
        delete tmp;
    }
}

template<class T>
void queue<T>::expand(){
    if (numOfElement==queueSize){
        T* tmp;
        try {
            tmp = new T[queueSize*2];
        } catch (const std::bad_alloc &e) {
            std::cerr << e.what() << std::endl;
        }

        for (tailItr=0; tailItr < numOfElement; ++tailItr) {
            tmp[tailItr]=element[frontItr];
            frontItr=(frontItr+1)%queueSize;
        }
        frontItr=0;
        std::swap(tmp,element);
        delete tmp;
    }
}

template<class T>
void queue<T>::pop(){
    numOfElement--;
    frontItr=(frontItr+1)%queueSize;
    shrink();
}

template<class T>
queue<T>::queue(int queueSize):queueSize(queueSize), numOfElement(0)
,frontItr(0),tailItr(0){
    try {
        element = new T[queueSize];
    } catch (const std::bad_alloc &e) {
        std::cerr << e.what() << std::endl;
    }
}

template<class T>
queue<T>::queue(const queue<T> &t) {
    if (*this == t)
        return *this;
    numOfElement = t.numOfElement;
    queueSize = t.queueSize;
    frontItr=0;
    tailItr=0;//只复制队列的元素，头指针位置从0开始
    try {
        element = new T[queueSize];
    } catch (const std::bad_alloc &e) {
        std::cerr << e.what() << std::endl;
    }

    for (int itr = t.front; itr%queueSize < t.tail;) {
        element[tailItr++]=t.element[(itr++)%queueSize];
    }
}

template<class T>
void queue<T>::push(const T& x){
    element[tailItr]=x;

    numOfElement++;
    tailItr=(tailItr+1)%queueSize;
    try{
        expand();
    } catch (const std::bad_alloc& e) {//如果没办法增大空间，则push失败
        std::cerr<<e.what();
        numOfElement--;
        tailItr=(tailItr==0?queueSize-1:tailItr-1);
    }
}

template<class T>
T& queue<T>::front()const{
    return element[0];
}

template<class T>
T& queue<T>::back() const {
    return element[numOfElement-1];
}

#endif /* queue_h */
