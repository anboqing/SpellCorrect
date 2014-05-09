#include <stdint.h>
#include <stdexcept>
#include <string>
#include <iostream>
#include <vector>
#include "EncodingConverter.h"

int getMin(int a, int b, int c)
{
    int min1 = a > b ? b : a;
    return min1 > c ? c : min1;
}

bool isGbkFirstByte(const unsigned char &uc)
{
    return uc >= 0x81 && uc <= 0xFE;
}

void saveGbk2Vector(const std::string &str, std::vector<uint16_t> &v)
{
    v.clear();
    for (std::string::size_type ix = 0; ix != str.size(); ++ix)
    {
        if (isGbkFirstByte(str[ix]))
        {
            if (ix + 1 == str.size())
            {
                throw std::runtime_error("gbk error");
            }
            uint8_t high = str[ix];
            uint8_t low = str[++ix];
            uint16_t whole = (high << 8) + low;
            v.push_back(whole);
        }
        else
        {
            uint8_t en = str[ix];
            v.push_back((uint16_t)en);
        }
    }
}

int editDistance(const std::vector<uint16_t>  &sa, const std::vector<uint16_t> &sb)
{
    int **matrix = new int *[sa.size() + 1];
    for (std::vector<uint16_t>::size_type i = 0; i <= sa.size(); ++i)
    {
        matrix[i] = new int[sb.size() + 1];
        memset(matrix[i], 0, sb.size() + 1);
    }

    for (std::vector<uint16_t>::size_type i = 0; i <= sa.size(); ++i)
    {
        matrix[i][0] = i;
    }
    for (std::vector<uint16_t>::size_type i = 0; i <= sb.size(); ++i)
    {
        matrix[0][i] = i;
    }

    for (std::vector<uint16_t>::size_type i = 1; i <= sa.size(); ++i)
    {
        for (std::vector<uint16_t>::size_type j = 1; j <= sb.size(); ++j)
        {
            int cost = sa[i - 1] == sb[j - 1] ? 0 : 1;
            matrix[i][j] =
                getMin( matrix[i - 1][j] + 1,
                        matrix[i][j - 1] + 1,
                        matrix[i - 1][j - 1] + cost);
        }
    }
    int ret = matrix[sa.size()][sb.size()];
    for (std::vector<uint16_t>::size_type i = 0; i <= sa.size(); ++i)
    {
        delete[] matrix[i];
    }
    delete matrix;
    return ret;
}

int getEditDistance(const std::string &sa, const std::string &sb)
{
    std::vector<uint16_t> va, vb;
    saveGbk2Vector(sa, va);
    saveGbk2Vector(sb, vb);
    return editDistance(va, vb);
}
using namespace std;
int main(int argc, char const *argv[])
{
    string sa("你好");
    string sb("你");
    EncodingConverter convertor;
    string a = convertor.utf8_to_gbk(sa);
    string b = convertor.utf8_to_gbk(sb);
    cout <<getEditDistance(a,b);
    return 0;
}