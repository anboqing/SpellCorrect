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
int main()
{
    hash_map<ClassA, string, hash_A, equal_A> hmap;
    ClassA a1(12);
    hmap[a1] = "I am 12";
    ClassA a2(198877);
    hmap[a2] = "I am 198877";
    cout << hmap[a1] << endl;
    cout << hmap[a2] << endl;
    return 0;
}