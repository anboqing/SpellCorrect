#include <iostream>  
#include <cstdlib>  
#include <cstdio>  
#include <string>  
#include <cstring>  
#include <wchar.h>  
#include <typeinfo>  
using namespace std;  
   

int main(int argc, char *argv[])
{

	cout << sizeof(wstring)<<endl;

	cout << sizeof(string)<<endl;
	
	cout << sizeof(wchar_t)<<endl;

	cout << sizeof(int)<<endl;

	cout <<sizeof(char)<<endl;

	cout <<sizeof(short)<<endl;

	cout <<sizeof("安")<<endl;

	wchar_t *wc ;

    char *p="安";

    cout <<sizeof(p)<<endl;

    cout <<strlen(p)<<endl;

    string s("安");

    wstring ss("安");
	return 0;
}
