//
//  array_any_dimension.h
//  C++
//
//  Created by 木林 on 2023/10/23.
//

#ifndef array_any_dimension_h
#define array_any_dimension_h

//
// Created by 木林 on 2023/10/16.
// 多维数组：行优先存储(从最后的列开始存储)
//

#include <vector>
#include <assert.h>
#include <iostream>

template<class T>
class dimensionArray{
public:
    dimensionArray(int dimension,std::initializer_list<int> dimension_length);//几个维度、每个维度的长度
    T& Location(std::vector<int> s);//返回对应坐标位置的元素
private:
    T* ele;
    std::vector<int> dimensionLength;
};

template<class T>
T& dimensionArray<T>::Location(std::vector<int> s) {
    assert(s.size()==dimensionLength.size());//确认维度相同
    int index=0;
    for (int i = 0; i < s.size(); ++i) {
        int tmp=1;
        for (int j = i+1; j < s.size(); ++j) {
            tmp*=dimensionLength[i];
        }
        index+=tmp*s[i];
    }
    index+=1;//之前计算出的index是所求元素前面的元素个数总和
    return ele[index];
}

template<class T>
dimensionArray<T>::dimensionArray(int dimension, std::initializer_list<int> dimension_length) {
    assert(dimension_length.size()==dimension);
    int Length=1;
    for (auto& x:dimension_length) {
        Length*=x;
        dimensionLength.push_back(x);
    }

    try{
        ele=new int[Length];
    }catch (const std::bad_alloc& e){
        std::cerr<<e.what()<<std::endl;
    }
}


#endif /* array_any_dimension_h */
