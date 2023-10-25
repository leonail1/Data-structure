//
//  string.h
//  C++
//
//  Created by 木林 on 2023/10/23.
//

#ifndef string_h
#define string_h
#include <iostream>
#include <assert.h>
using std::ostream;
using std::istream;
using std::cin;
using std::cout;

class String
{
private:
    char * str;             // pointer to string
    int len;                // length of string
    static int num_strings; // String类对象数量
    static const int CINLIM = 80;  // cin input limit   输入限制
public:
    // constructors and other methods
    explicit String(const char * s); // constructor
    String();               // default constructor
    String(const String &); // copy constructor
    ~String(){delete[] str;num_strings--;}             // destructor
    [[nodiscard]]int length () const { return len; }
    [[nodiscard]]int size() const { return len; }
    // overloaded operator methods
    String & operator=(const String &);
    String & operator=(const char *);
    char & operator[](int i);
    const char & operator[](int i) const;
    String & operator+(const String &st);
    String & operator+(const char *s);
    // overloaded operator friends
    friend bool operator<(const String &st, const String &st2);
    friend bool operator>(const String &st1, const String &st2);
    friend bool operator==(const String &st, const String &st2);
    friend bool operator==(const String &st, const char *st2);
    friend ostream & operator<<(ostream & os, const String & st);
    friend istream & operator>>(istream & is, String & st);
    // static function
    static int HowMany(){return num_strings;}
public:
    //功能函数
    void append(const String &st);  //追加字符串
    void append(const char*s);      //追加字符串
    void push_back(const char ch);  //追加字符
    void swap(char* s);             //交换值
    void swap(String& st);          //交换值
    int find(const char*s, size_t index);   //寻找字符串
    int find(const String&st, size_t index);//寻找字符串
};

String & String::operator+(const String &st){
    char* tmp;
    try {
        tmp = new char[len + st.len + 1];
        len=len+st.len;
    }catch (const std::bad_alloc& e){
        std::cerr<<"在字符串连接时内存溢出 "<<e.what()<<std::endl;
    }

}

const char & String::operator[](int i) const{
    assert(i>=0&&i<len);
    return str[i];
}

char & String::operator[](int i){
    assert(i>=0&&i<len);
    return str[i];
}

String & String::operator=(const char *s){
    len= strlen(s);
    str=new char[len+1];
    strcpy(str,s);
    return *this;
}

String & String::operator=(const String & t){
    if (t==*this)
        return *this;
    len=t.len;
    str=new char[len+1];
    strcpy(str,t.str);
    return *this;
}

String::String() {
    len = 4;
    str = new char[1];
    str[0] = '\0';                 // default string
    num_strings++;
}

String::String(const String & t){
    len= strlen(t.str);
    str=new char[len+1];
    strcpy(str,t.str);
    num_strings++;
}

String::String(const char * s){
    len= strlen(s);
    str=new char[len+1];
    strcpy(str,s);
    num_strings++;
}

void String::append(const String &st){
    char* tmp;
    try {
        len+= st.len;
        tmp = new char[len + 1];
    }catch (const std::bad_alloc& e){
        std::cerr<<"在字符串连接时内存溢出 "<<e.what()<<std::endl;
    }
    strcpy(tmp,str);
    strcat(tmp,st.str);

    std::swap(tmp,str);
    delete tmp;
}

void String::append(const char* s){
    strcat(str,s);
    len= strlen(str);
}

void String::push_back(const char ch){
    strcat(str,&ch);
    len= strlen(str);
}

void String::swap(char* s){
    String tmp(s);
    strcpy(s,str);
    strcpy(str,tmp.str);
    len= strlen(str);
}

void String::swap(String& st){
    String tmp(st);
    strcpy(st.str,str);
    strcpy(str,tmp.str);
    len= strlen(str);
}

int String::find(const char*s, size_t index){
    int** dp=new int*[strlen(s)];//状态数组dp[状态][字符]
    for (int i = 0; i < strlen(s); ++i) {
        dp[i]=new int[256];//字符的ASCII值
    }

    //dp数组的构建
    int X=0;//影子状态
    dp[0][(int)s[0]]=1;//状态0时碰到匹配的字符则转移到状态1
    for (int j = 1; j < strlen(s); ++j) {//状态
        for (int c = 0; c < 256; ++c) {//字符
            if ((int)s[j]==c){//在状态j下碰到匹配的支付则推进到下一个状态
                dp[j][c]=j+1;
            }else{
                dp[j][c]=dp[X][c];//回退，由影子状态处理
            }
        }
        X=dp[X][(int)s[j]];//影子状态向前推进
    }

    int j=0;//初始状态为0
    for (long i = index; i < strlen(str); ++i) {
        j=dp[j][(int)str[i]];
        if (j== strlen(s)){//匹配成功
            return i- strlen(s)+1;//返回匹配到的字符串在原始文本中的开始索引
        }
    }
    return -1;//没有匹配成功，返回-1
}

int String::find(const String&st, size_t index){
    int** dp=new int*[strlen(st.str)];//状态数组dp[状态][字符]
    for (int i = 0; i < strlen(st.str); ++i) {
        dp[i]=new int[256];//字符的ASCII值
    }

    //dp数组的构建
    int X=0;//影子状态
    dp[0][(int)st.str[0]]=1;//状态0时碰到匹配的字符则转移到状态1
    for (int j = 1; j < strlen(st.str); ++j) {//状态
        for (int c = 0; c < 256; ++c) {//字符
            if ((int)st.str[j]==c){//在状态j下碰到匹配的支付则推进到下一个状态
                dp[j][c]=j+1;
            }else{
                dp[j][c]=dp[X][c];//回退，由影子状态处理
            }
        }
        X=dp[X][(int)st.str[j]];//影子状态向前推进
    }

    int j=0;//初始状态为0
    for (long i = index; i < strlen(str); ++i) {
        j=dp[j][(int)str[i]];
        if (j== strlen(st.str)){//匹配成功
            return i- strlen(st.str)+1;//返回匹配到的字符串在原始文本中的开始索引
        }
    }
    return -1;//没有匹配成功，返回-1
}

#endif /* string_h */
