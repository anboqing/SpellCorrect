#ifndef EDITDISTANCE_H_
#define EDITDISTANCE_H_

#include <stdint.h>
#include <stdexcept>
#include <string>
#include <vector>
#include "Log.h"
#include "EncodingConverter.h"
#include <iostream>

int getMin(int a, int b, int c)
{
    int min1 = a > b ? b : a;
    return min1 > c ? c : min1;
}

bool isGbkFirstByte(unsigned char c)
{
    return c >= 0x81 && c <= 0xFE;
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
                WRITE_STR(std::string("invalid gbk"));
                throw std::runtime_error("gbk error");
            }
            unsigned char high = str[ix];
            unsigned char low = str[++ix];
            uint16_t whole = (high << 8) + low;
            v.push_back(whole);
        }
        else
        {
            v.push_back((uint16_t) (unsigned char)str[ix]);
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
    if (sa.size() != 0 && sb.size() != 0)
    {
        std::vector<uint16_t> va, vb;
        saveGbk2Vector(sa, va);
        saveGbk2Vector(sb, vb);
        return editDistance(va, vb);
    }
    else
    {
        return 0x11111111;
    }
}

// int editDistance(const std::string &sa, const std::string &sb)
// {
//     int **matrix = new int *[sa.size() + 1];
//     for (std::string::size_type i = 0; i <= sa.size(); ++i)
//     {
//         matrix[i] = new int[sb.size() + 1];
//         memset(matrix[i], 0, sb.size() + 1);
//     }

//     for (std::string::size_type i = 0; i <= sa.size(); ++i)
//     {
//         matrix[i][0] = i;
//     }
//     for (std::string::size_type i = 0; i <= sb.size(); ++i)
//     {
//         matrix[0][i] = i;
//     }

//     for (std::string::size_type i = 1; i <= sa.size(); ++i)
//     {
//         for (std::string::size_type j = 1; j <= sb.size(); ++j)
//         {
//             int cost = sa[i - 1] == sb[j - 1] ? 0 : 1;
//             matrix[i][j] =
//                 getMin( matrix[i - 1][j] + 1,
//                         matrix[i][j - 1] + 1,
//                         matrix[i - 1][j - 1] + cost);
//         }
//     }
//     int ret = matrix[sa.size()][sb.size()];
//     for (std::string::size_type i = 0; i <= sa.size(); ++i)
//     {
//         delete[] matrix[i];
//     }
//     delete matrix;
//     return ret;
// }

#endif