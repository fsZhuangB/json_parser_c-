#include <string>
#include <iostream>
using namespace std;

class json_content {
    public:
    string json_value;
};

static void json_parse_whiteSpace(json_content* c);
int main()
{
    // string json = " null";
    // json_content jc;
    // jc.json_value = json;

    // json_parse_whiteSpace(&jc);
    // cout << jc.json_value << endl;
    // string json = "good";
    // auto b = json.begin();
    // cout << *b << endl;
    string ss = "good";
    cout << ss << endl;
    ss.clear();
    cout << ss << endl;
    return 0;
}

static void json_parse_whiteSpace(json_content* c)
{
    auto beg = (c->json_value).begin();
    auto end = (c->json_value).end();
    while (*beg == ' ')
        beg++;
    c->json_value = c->json_value.assign(beg, end);
}