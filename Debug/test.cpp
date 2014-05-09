#include <stdint.h>

#include <vector>
#include <string>
#include <stdexcept>


bool is_gbk_first_alpah(unsigned char c)
{
    return c >= 0x81 && c <= 0xFE;
}

void parse_gbk_string(const string &s, vector<uint16_t> &vec)
{
    for (string::size_type ix = 0; ix != s.size() ; ++ix)
    {
        if (is_gbk_first_alpah(s[ix]))
        {
            if (ix + 1 == s.size())
            {
                throw runtime_error("invalid GBK string");
            }
            unsigned char c1 = s[ix];
            unsigned char c2 = s[ix + 1];
            uint16_t t = (c1 << 8) + c2;
            vec.push_back(t);
            ++ix;
        }
        else
        {
            vec.push_back((uint16_t)(unsigned char)s[ix]);
        }
    }
}

int main(int argc, char const *argv[])
{
    string s = "安勃卿";
    vector<uint16_t> vec;
    EncodingConverter trans;
    s = trans.utf8_to_gbk(s);

    // for(unsigned char x : s){
    //  printf("0x%x\n",s );
    // }

    parse_gbk_string(s, vec);
    for (vector<uint16_t>::iterator iter = vec.begin(); iter != vec.end(); ++iter)
    {


    }
    return 0;
}

int ed(const vector<uint16_t> &w1,const vector<uint16_t> &w2)
{
    int len_a = w1.size();
    int len_b = w2.size();
    int memo[100][100];
    memset(memo, 0x00, 100 * 100 * sizeof(int));
    for (int i = 0; i < = len_a; ++i)
    {
        mem[i][0] = i;
    }
    for (int j = 0; j <= lne_b; ++j)
    {
        mem[0][j] = j;
    }

    for(int i = 1;i<=len_a; ++i){
    	for(int j = 1; j<=len_b;++j){
    		if(w1[i-1]==w2[j-1]){
    			memo[i][j]=memo[i-1][j-1];
    		}else{
    			memo[i][j] = min(mem[i-1][j]+1,min(memo[i][j-1]+1,memo[i-1][j-1]);
    		}
    	}
    }
    return memo[len_a][len_b];
}