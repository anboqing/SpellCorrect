#include <ext/hash_map>
#include <string>
#include <iostream>


using namespace std;
using namespace __gnu_cxx;

//define the class
class ClassA
{
public:
    ClassA(int a): c_a(a) {}
    int getvalue()const
    {
        return c_a;
    }
    void setvalue(int a)
    {
        c_a;
    }
private:
    int c_a;
};
//1 define the hash function
struct hash_A
{
    size_t operator()(const class ClassA &A)const
    {
        //  return  hash<int>(classA.getvalue());
        return A.getvalue();
    }
};
//2 define the equal function
struct equal_A
{
    bool operator()(const class ClassA &a1, const class ClassA &a2)const
    {
        return  a1.getvalue() == a2.getvalue();
    }
};

class str_compare
{
public:
    bool operator()(const string &str1, const string &str2)const
    {
        return   str1 == str2;
    }
};

struct str_hash
{
    size_t operator()(const string &str) const
    {
        return __stl_hash_string(str.c_str());
    }
};


int main()
{
    // hash_map<ClassA, string, hash_A, equal_A> hmap;
    // ClassA a1(12);
    // hmap[a1] = "I am 12";
    // ClassA a2(198877);
    // hmap[a2] = "I am 198877";
    // cout << hmap[a1] << endl;
    // cout << hmap[a2] << endl;

    // hash_map<string,string,str_hash,str_compare>  myhash;
    
    // myhash["google"]="newplan";
   
    // myhash["baidu"]="anboqing";

    // string key(" keyword ");
    // string value(" value ");

    // myhash.insert(make_pair(key,value));

    // if(myhash.find(key)!=myhash.end()){
    // 	cout <<myhash[key]<<endl;
    // }
   
    // if(myhash.find("google")!=myhash.end())
    //   cout<<myhash["google"]<<endl;

    hash_map<string,pair<string,int>,str_hash,str_compare> myhash;

   	string key("key");
   	string word("word");
   	int frequency = 10; 
   	pair<string,int> mypair = make_pair(key,frequency);

   	myhash.insert(make_pair(key,mypair));

   	cout << myhash[key].second << " " << myhash[key].first<<endl;

    return 0;
}