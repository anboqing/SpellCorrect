#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <inttypes.h>
using namespace std;

void cleanToken(char *pattern)
{
    char *p = pattern;
    char *buf = new char[sizeof(char)*strlen(pattern) + 1];
    char ch, pre = ' ';
    int i = 0;

    vector<string> token_gbk;
    
    token_gbk.push_back("，");
    token_gbk.push_back("。");
    token_gbk.push_back("：");
    token_gbk.push_back("“");
    token_gbk.push_back("”");
    token_gbk.push_back("；");
    token_gbk.push_back("’");
    token_gbk.push_back("‘");
    token_gbk.push_back("？");
    token_gbk.push_back("《");
    token_gbk.push_back("》");
    token_gbk.push_back("！");
    token_gbk.push_back("、");

    //考虑中文情况，第一个字节小于0
    uint8_t head, tail;
    head = (*p) & 0xff;
    tail = (*(p + 1)) & 0xff;
    uint16_t whole = (head << 8) + tail;
    bool flag = false;

    for (vector<string>::iterator iter = token_gbk.begin(); iter != token_gbk.end(); ++iter)
    {
        const char *high = (*iter).c_str();
        const char *low = high + 1;

        uint8_t h = *high;
        uint8_t l = *low;

        uint16_t w = (h << 8) + l;

        if (whole == w)
        {
            //说明是标点
            flag = true;
            break;
        }
    }
    if (flag == true)
    {
        //是标点符号的话就把2个字节都变为空格，否则不处理
        *p = ' ';
        *(++p) = ' ';
    }


    buf[i] = '\0';
    memset(pattern, 0, strlen(pattern)*sizeof(char));
    strcpy(pattern, buf);
}

bool isGBK(unsigned char head, unsigned char tail)
{
    head = head & 0xff;
    tail = tail & 0xff;
    return ((head >= 0x81 && head <= 0xfe &&
             (tail >= 0x40 && tail <= 0x7e ||
              tail >= 0x80 && tail <= 0xfe)) ? true : false);
}

bool isGB2312(unsigned char head, unsigned char tail)
{
    head = head & 0xff;
    tail = tail & 0xff;
    return (((head >= 0xa1 && head <= 0xf7) &&
             (tail >= 0xa1 && tail <= 0xfe)) ? true : false);
}

int main(int argc, char const *argv[])
{
    ifstream ifs("Ch_Token.txt");
    char buf[1000] = {0};
    string line;
    while (getline(ifs, line))
    {
        cout << line;
        strcpy(buf, line.c_str());
        cleanToken(buf);
        cout << buf;
    }
    ifs.close();
    return 0;
}
