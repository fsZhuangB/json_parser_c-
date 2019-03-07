/* a test function for my library */
// good
#include "rafajson.hpp"
#include <iostream>
using namespace std;
int main()
{
    enum class Enum_type {Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday};
    Enum_type weekDays = Enum_type::Monday;

    switch (weekDays)
    {
        case Enum_type::Monday:
            cout << "It's monday!" << endl;
            break;
    
        default: 
        cout << "have a nice day!" << endl;
            break;
    }
    return 0;

}

