//
//  KMP.h
//  C++
//
//  Created by 木林 on 2023/10/23.
//

#ifndef KMP_h
#define KMP_h

//
// Created by 木林 on 2023/10/9.
// findNext函数是课件要求的版本
//

#include <cstddef>
#include <cstring>
#include <string>
#include <vector>

int KMP(char* text, char* pattern, size_t index = 0) {
    /*
     * 这个版本能过oj
     */
    int lenText=strlen(text),lenPattern= strlen(pattern);
    int _next[lenPattern+1];
    _next[0]=0;
    _next[1]=0;
    int k=0;
    for (int i = 1; i < lenPattern; ++i) {
        while(k>0&&pattern[i]!=pattern[k])k=_next[k];
        _next[i+1]=pattern[i]==pattern[k]?++k:0;
    }
    k=0;
    for (int i = 0; i < lenText; ++i) {
        while(k&&text[i]!=pattern[k])k=_next[k];
        if (text[i]==pattern[k]) {
            k++;
        }
        if (k==lenPattern){
            print_recursionf("%d\n",i-k+2);
        }
    }
    for (int i = 1; i <= lenPattern; ++i) {
        print_recursionf("%d",_next[i]);
        print_recursionf((i==lenPattern)?"":" ");
    }
}
int findDp(const std::string &text, const std::string &pattern, size_t index = 0) {
    int dp[pattern.size()][256];

    for (int i = 0; i < pattern.size(); ++i) {
        for (int j = 0; j < 256; ++j) {
            dp[i][j] = 0;
        }
    }

    int X = 0;
    dp[0][pattern[0]] = 1;
    for (int i = 1; i < pattern.size(); ++i) {
        for (int j = 0; j < 256; ++j) {
            if (j == (int) pattern[i]) {
                dp[i][j] = i + 1;
            } else {
                dp[i][j] = dp[X][j];
            }
        }
        X = dp[X][pattern[i]];
    }

    int j = 0;
    for (int i = index; i < text.size(); ++i) {
        j = dp[j][text[i]];
        if (j == pattern.size()) {
            return i - j + 1;
        }
    }
    return -1;
}

int findNext(const std::string &text, const std::string &pattern, int index = 0) {
    //首先计算next数组
    std::vector<int> next(pattern.size(), 1);//没有公共前缀记为1
    next[0] = 0;//定义

    for (int i = 1; i < pattern.size(); ++i) {
        for (int j = i-1; j >= 1; j--) {
            //从模式串的第二个字符开始寻找最长真前后缀
            std::string a=pattern.substr(0,j);
            std::string b=pattern.substr(i-j,j);
            if (a==b){
                //如果前缀相同
                next[i]+=j;
                break;//从最长子串开始搜索的，第一个搜到的就是最长的
            }
        }
    }

    int j=0;
    for (int i = 0; i < text.size(); ++i) {
        while (j > 0 && text[i] != pattern[j]) {
            j = next[j] - 1; // 回退到最长公共前后缀的下一位，由于我们的next数组相比原始定义多了1，所以这里要减1
        }
        // 如果当前字符匹配，则递增模式串的索引
        if (text[i] == pattern[j]) {
            j++;
        }
        // 如果模式串完全匹配，则返回匹配的开始位置
        if (j == pattern.size()) {
            return i - j + 1;
        }
    }
    return -1;//没找到
}



#endif /* KMP_h */
