//
//  linearlist_array.h
//  C++
//
//  Created by 木林 on 2023/10/23.
//

#ifndef linearlist_array_h
#define linearlist_array_h

#include <iostream>
#include <cassert>
#include "linearlist_abstract.h"

template<class T>
class arraylist : public linearlist<T> {
public:
    explicit arraylist(int element_length = 10);

    arraylist(const arraylist<T> &t);

    arraylist<T> &operator=(const arraylist<T> &t);

    [[nodiscard]] virtual bool empty() const { return static_cast<bool>(num_of_element); }

    [[nodiscard]] virtual int size() const { return num_of_element; }

    [[nodiscard]] virtual T &get(int index) const;

    [[nodiscard]] virtual int indexOf(const T &find_element) const;

    virtual void erase(int index);

    virtual void insert(int index, const T &x);

    virtual std::ostream &output(std::ostream &out) const;

private:
    void shrink();

    void expand();

    std::unique_ptr<T[]> element;
    int num_of_element;//元素个数
    int length_of_element;//数组空间
};

template<class T>
arraylist<T>::arraylist(int element_length) {
    assert(element_length > 0);//线性表大小为正数
    try {
        element.reset(new T[std::max(element_length, 10)]);//为unique_ptr重新分配内存，至少分配十个元素的空间
    } catch (const std::bad_alloc &e) {
        std::cerr << "Memory allocation failed in construction: " << e.what() << '\n';
    }
    num_of_element = 0;
    length_of_element = std::max(element_length, 10);
}

template<class T>
arraylist<T>& arraylist<T>::operator=(const arraylist<T> &t){
    element=t.element;
    num_of_element=t.num_of_element;
    length_of_element=t.length_of_element;
}

template<class T>
arraylist<T>::arraylist(const arraylist<T> &t) {
    try {
        element.reset(new T[t.length_of_element]);//内存重分配
    } catch (const std::bad_alloc &e) {
        std::cerr << "Memory allocation failed in copy construction: " << e.what() << '\n';
    }
    for (int i = 0; i < t.num_of_element; ++i) {//深拷贝
        element[i] = t.element[i];
    }

    num_of_element = t.num_of_element;
    length_of_element = t.length_of_element;
}

template<class T>
T &arraylist<T>::get(int index) const {
    assert(index >= 0 && index < num_of_element);
    return element[index];
}

template<class T>
int arraylist<T>::indexOf(const T &find_element) const {
    for (int i = 0; i < num_of_element; ++i) {
        if (element[i] == find_element)
            return i;
    }
    return -1;//没找到元素
}

template<class T>
void arraylist<T>::shrink() {
    /*当元素个数小于数组长度的1/4时进行缩容，缩容到原来的1/2，最小要有十个元素*/
    if (num_of_element < length_of_element / 4 && length_of_element > 10) {
        length_of_element = std::max(length_of_element / 2, 10);
        std::unique_ptr<T[]> tempo_ptr;
        try {
            tempo_ptr.reset(length_of_element);
        } catch (const std::bad_alloc &e) {
            std::cerr << "Memory allocation failed in shrink process: " << e.what() << '\n';
        }
        for (int i = 0; i < num_of_element; ++i) {
            tempo_ptr[i] = element[i];
        }
        element = std::move(tempo_ptr);
    }
}

template<class T>
void arraylist<T>::expand() {
    /*当数组满了，就加倍*/
    if (num_of_element == length_of_element) {
        length_of_element *= 2;
        std::unique_ptr<T[]> tempo_ptr;
        try {
            tempo_ptr.reset(length_of_element);
        } catch (const std::bad_alloc &e) {
            std::cerr << "Memory allocation failed in expand process: " << e.what() << '\n';
        }
        for (int i = 0; i < num_of_element; ++i) {
            tempo_ptr[i] = element[i];
        }
        element = std::move(tempo_ptr);
    }
}

template<class T>
void arraylist<T>::erase(int index) {
    assert(index >= 0 && index < num_of_element);
    for (int i = index; i < num_of_element - 1; ++i) {
        element[i] = element[i + 1];
    }
    shrink();
    --num_of_element;
}

template<class T>
void arraylist<T>::insert(int index, const T &x) {
    assert(index >= 0 && index <= num_of_element);//index==num_of_element：尾插
    if (index != num_of_element) {
        for (int i = num_of_element - 1; i >= index; ++i) {
            element[i + 1] = element[i];
        }
    }
    element[index] = x;
    ++num_of_element;
    expand();
}

template<class T>
std::ostream &arraylist<T>::output(std::ostream &out) const {
    for (int i = 0; i < num_of_element; ++i) {
        out << element[i] << " ";
    }
}

#endif /* linearlist_array_h */
