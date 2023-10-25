//
//  sparse_matrix_3_tuple.h
//  C++
//
//  Created by 木林 on 2023/10/23.
//

#ifndef sparse_matrix_3_tuple_h
#define sparse_matrix_3_tuple_h

#include <vector>
#include <cassert>
#include <iostream>
#include <cmath>
#include <algorithm>

template<class T>
struct ele {//三元组
    int row;
    int col;
    T element;
};

template<class T>
bool cmp_row(ele<T> a,ele<T> b){//行优先顺序
    if (a.row==b.row){
        return a.col<b.col;
    }else{
        return a.row<b.row;
    }
}

template<class T>
bool cmp_col(ele<T> a,ele<T> b){//列优先顺序
    if (a.col==b.col){
        return a.row<b.row;
    }else{
        return a.col<b.col;
    }
}

template<class T>
class sparseMatrix {
    //稀疏矩阵-三元组
public:
    sparseMatrix():row(0),col(0) {}

    sparseMatrix(const std::vector<ele<T>>&,int rowLength,int colLength);

    sparseMatrix(std::initializer_list<T>,int rowLength,int colLength);

    sparseMatrix(const sparseMatrix<T> &);

    sparseMatrix<T> &operator=(const sparseMatrix<T> &);

    void print_recursion();

    sparseMatrix<T> operator+(const sparseMatrix<T> &);

    sparseMatrix<T> operator-(const sparseMatrix<T> &);

    sparseMatrix<T> operator*(const sparseMatrix<T> &);

    sparseMatrix<T> & transposeMatrix();

private:

    std::vector<ele<T>> element;
    int row;//原始矩阵的边长
    int col;
};

template<class T>
sparseMatrix<T> & sparseMatrix<T>::transposeMatrix(){
    std::swap(row,col);
    for (auto&x:element) {
        std::swap(x.row, x.col);
    }
    sort(element.begin(),element.end(), cmp_row<T>);
    return *this;
}

template<class T>
sparseMatrix<T> sparseMatrix<T>::operator*(const sparseMatrix<T> & t){
    /*
     * 将前一个矩阵按行优先顺序排序，后一个矩阵按列优先顺序排序
     * 对于前一个矩阵的每一个元素(i,j,e)，遍历后一个矩阵每个元素(i1,j1,e1)
     * 如果j==i1，将e*e1加到res中(i,j1)的位置
     */
    std::vector<ele<T>> right=t.element;
    std::vector<ele<T>> result;
    sort(element.begin(),element.end(),cmp_row<T>);
    sort(right.begin(),right.end(), cmp_col<T>);

    for (int i = 0; i < element.size(); ++i) {
        ele f=element[i];
        for (int j = 0; j < right.size(); ++j) {
            ele b=right[j];
            if (f.col==b.row){
                //判断result中有没有(f.row,b.col)位置的元素
                bool is_rowcol_exist=false;
                for (int k = result.size()-1; k >=0 ; --k) {
                    if (result[k].row==f.row&&result[k].col==b.col){
                        //如果有，就直接相加，否则插入
                        result[k].element+=f.element*b.element;
                        is_rowcol_exist=true;
                        break;
                    }
                }
                if (!is_rowcol_exist){
                    //不存在就插入
                    result.push_back({f.row,b.col,f.element*b.element});
                }
            }
        }
        //sparseMatrix<T>(result,f.row,t.col).print_recursion();
    }
    return {result,row,t.col};
}

template<class T>
sparseMatrix<T>::sparseMatrix(const std::vector<ele<T>>& t,int rowLength,int colLength){
    element=t;
    row=rowLength;
    col=colLength;
}

template<class T>
sparseMatrix<T> sparseMatrix<T>::operator-(const sparseMatrix<T> & t){
    std::vector<ele<T>> tmp=t.element;
    for (auto& x:tmp) {
        x.element=-x.element;
    }
    sparseMatrix<T> temp(tmp,t.row,t.col);
    return *this+temp;
}

template<class T>
sparseMatrix<T>  sparseMatrix<T>::operator+(const sparseMatrix<T> & e){
    sparseMatrix<T> t=e;
    std::vector<ele<T>> result=element;
    assert(row==t.row&&col==t.col);

    int size=result.size();
    for(int i=0;i<size;i++){
        int r=result[i].row;
        int c=result[i].col;

        for(int j=0;j<t.element.size();j++){
            if(r==t.element[j].row&&c==t.element[j].col){
                result[i].element+=t.element[j].element;
                t.element.erase(t.element.begin()+j);
            }
        }
    }

    for(int i=0;i<t.element.size();i++){
        result.push_back(t.element[i]);
    }
    sort(result.begin(),result.end(),cmp_row<T>);
    return sparseMatrix<T>(result,row,col);
}

template<class T>
void sparseMatrix<T>::print_recursion() {
    auto itr = element.begin();
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (i == itr->row && j == itr->col){
                std::cout<<itr->element<<((j==col-1)?"":" ");
                itr++;
            }else{
                std::cout<<0<<((j==col-1)?"":" ");
            }
        }
        std::cout<<std::endl;
    }
}

template<class T>
sparseMatrix<T>::sparseMatrix(std::initializer_list<T> t,int rowLength,int colLength) {
    row=rowLength;
    col=colLength;

    std::vector<std::vector<T>> s(row, std::vector<T>(col, 0));
    auto it = t.begin();

    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (*it != 0) {
                element.push_back({i, j, *it});
            }
            it++;
        }
    }

}

template<class T>
sparseMatrix<T>::sparseMatrix(const sparseMatrix<T> &t) {
    element = t.element;
    row=t.row;
    col=t.col;
}

template<class T>
sparseMatrix<T> &sparseMatrix<T>::operator=(const sparseMatrix<T> &t) {
    element = t.element;
    row=t.row;
    col=t.col;
}

#endif /* sparse_matrix_3_tuple_h */
