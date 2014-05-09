#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <cstring>
#include <wchar.h>
#include <typeinfo>
using namespace std;

inline wchar_t* MBCS2Unicode(wchar_t* buff,const char * str)
{
    wchar_t * wp = buff;
    const char * p = str;
    while(*p)
    {
        *wp = (wchar_t) 0x0;
        if(*p & 0x80)
        {
            //因为sizeof(wchar_t) 在linux为4
            //所以需要trick处理一下
            char temp[sizeof(wchar_t)];
            temp[0] = *p;
            p++;
            temp[1] = *p;
            for(int i = 2; i < sizeof(wchar_t); i++)
                temp[i] = 0x0;
            *wp = *(wchar_t *)temp;
        }
        else{
            *wp = (wchar_t) *p;
        }
        wp++;
        p++;
    }
    *wp = 0x00000000;
    return buff;
}
 
inline wstring str2wstr(char* str)
{
    size_t len = strlen(str);
    wchar_t* b = (wchar_t *)malloc((len+1)*sizeof(wchar_t));
    MBCS2Unicode(b, str);
    wstring r(b);
    free(b);
    return r;
}

//''' 【编辑距离算法】 【levenshtein distance】 【字符串相似度算法】 '''
inline int levenshtein(const wstring& s1, const wstring& s2) {
    if(s1.length() == 0 ) {
        return s2.length();
    }
    if(s2.length() == 0 ) {
        return s1.length();
    }
    vector<int*> distance_matrix(s1.length() + 1);
    vector<int> memory((s1.length() + 1)  * (s2.length() + 1));

    for(int row = 0; row <= s1.length() ; row++ ) {
        distance_matrix[row] = &memory[0] + row * (s2.length() + 1);
        distance_matrix[row][0] = row;
    }
    for(int col = 0; col <= s2.length() ; col++ ) {
        distance_matrix[0][col] = col;
    }

    for(int i = 1; i <= s1.length() ; i++ ) {
        for(int j = 1; j <= s2.length() ; j++ ) {
            int deletion = distance_matrix[i-1][j] + 1;
            int insertion = distance_matrix[i][j-1] + 1;
            int substitution = distance_matrix[i-1][j-1] + (s1[i - 1] == s2[j - 1]? 0:1);
            distance_matrix[i][j] = min(insertion, min(deletion, substitution));
        }
    }

    return distance_matrix[s1.length()][s2.length()]; 
}
    
int main(int argc, char* argv[]) {
    wstring s1 = str2wstr("安勃卿");
    wstring s2 = str2wstr("安勃");
    cout << levenshtein(s1, s2) << endl;
    return 0;
}
