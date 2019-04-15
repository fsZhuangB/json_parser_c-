#include <string>
#include <iostream>
using namespace std;

int main()
{
    string content;
    content = "json_test";
    const char * p = content.c_str();
    cout << *p << endl;

    return 0;
}