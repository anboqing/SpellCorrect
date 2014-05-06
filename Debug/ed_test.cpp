#include <string>
#include <iostream>
using namespace std;


#include <string.h>

int getMin(int a, int b, int c)
{
    int min1 = a > b ? b : a;
    return min1 > c ? c : min1;
}

int getEditDistance(const string &sa, const string &sb)
{
    int **matrix = new int *[sa.size() + 1];

    for (int i = 0; i <= sa.size(); ++i)
    {
        matrix[i] = new int[sb.size() + 1];
        memset(matrix[i],0,sb.size()+1);
    }

    for (int i = 0; i <= sa.size(); ++i)
    {
        matrix[i][0] = i;
    }
    for (int i = 0; i <= sb.size(); ++i)
    {
        matrix[0][i] = i;
    }

    for (int i = 1; i <= sa.size(); ++i)
    {
        for (int j = 1; j <= sb.size(); ++j)
        {
            int cost = sa[i - 1] == sb[j - 1] ? 0 : 1;
            matrix[i][j] =
                getMin( matrix[i - 1][j] + 1,
                        matrix[i][j - 1] + 1,
                        matrix[i - 1][j - 1] + cost);
        }
    }
    int ret = matrix[sa.size()][sb.size()];
    for (int i = 0; i <= sa.size(); ++i)
    {
        delete[] matrix[i];
    }
    delete matrix;
    return ret;
}

int main(int argc, char const *argv[])
{
    string a("a");
    string above("above");
    std::cout << getEditDistance(a, above);
    return 0;
}