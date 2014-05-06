#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>

using namespace std;
void func(string &a,
          const char *b = __DATE__,
          const char *c = __FILE__,
          int line = __LINE__);
// void func(string &a,
//           const char *b = __DATE__,
//           const char *c = __FILE__,
//           int line = __LINE__)
void func(string &a,
          const char *b,
          const char *c,
          int line )
{
    cout << a << b << c << line;
}

int main(int argc, char const *argv[])
{
    char c[] = "hello";
    string s("hello");
    func(s);
    return 0;
}