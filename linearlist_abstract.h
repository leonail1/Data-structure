//
//  linearlist_abstract.h
//  C++
//
//  Created by 木林 on 2023/10/23.
//

#ifndef linearlist_abstract_h
#define linearlist_abstract_h

//
// Created by 木林 on 2023/9/12.
//


//
//  linearlist.h
//  Stack
//
//  Created by 木林 on 2023/5/24.
//

#ifndef linearlist_h
#define linearlist_h
#include <fstream>

template<class T>
class linearlist{
public:
    virtual ~linearlist(){};
    virtual bool empty()const=0;
    virtual int size()const=0;
    virtual T& get(int index)const=0;
    virtual int indexOf(const T& find_element)const=0;
    virtual void erase(int index)=0;
    virtual void insert(int index,const T& x)=0;
    virtual std::ostream& output(std::ostream& out)const=0;
};


#endif /* linearlist_abstract_h */
