#include <string>
#include <iostream>
using namespace std;

int main()
{
    string content;
    content = "json_test";
    const char * p = content.c_str();
    cout << *p << endl;
    content += 2;
    const char *p2 = content.c_str();
    cout << *p2 << endl;

    return 0;
}