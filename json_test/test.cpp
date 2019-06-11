//
// Created by fszhuangb on 2019-06-05.
//

#include <gtest/gtest.h>
#include <string>
#include "../src/json.hpp"
using namespace rafaJSON;
using namespace std;

Json parse_ok(const string& strJson)
{
    string errMsg;
    Json json = Json::parse(strJson, errMsg);
    EXPECT_EQ(errMsg, "");
    return json;
}

#define test_error(expect, strJson)            \
  do                                           \
  {                                            \
    string errMsg;                             \
    Json json = Json::parse(strJson, errMsg);  \
    auto pos = errMsg.find_first_of(":");      \
    auto actual = errMsg.substr(0, pos);       \
    EXPECT_EQ(actual, expect);                 \
  } while (0)                                  \

#define test_null(strJson)                     \
 do                                            \
   {                                           \
    Json json = parse_ok(strJson);             \
    EXPECT_TRUE(json.json_value_is_Null());    \
   } while(0)

#define test_bool(expect, content)                          \
do                                                          \
{                                                           \
    Json json = parse_ok(content);                          \
    EXPECT_TRUE(json.json_value_is_Bool());                 \
    EXPECT_EQ(json.json_value_to_Bool(), expect);           \
    json = Json(!expect);                                   \
    EXPECT_EQ(json.json_value_to_Bool(), !expect);          \
} while(0)

#define test_number(expect, strJson)                        \
do                                                          \
{                                                           \
    Json json = parse_ok(strJson);                          \
    EXPECT_TRUE(json.json_value_is_Number());               \
    EXPECT_EQ(json.json_value_to_Double(), expect);         \
} while(0)

#define test_string(expect, strJson) \
do\
{\
    Json json = parse_ok(strJson);\
    EXPECT_TRUE(json.json_value_is_String());\
    EXPECT_EQ(json.json_value_to_String(), expect);\
} while(0)

TEST(Str2Json, JsonNull)
{
    test_null("null");
    test_null("   null\n\r\t");
}

TEST(Str2Json, JsonBool)
{
    test_bool(true, "true");
    test_bool(false, "false");
}

TEST(Str2Json, JsonNumber)
{
    test_number(0.0, "0");
    test_number(0.0, "-0");
    test_number(0.0, "0.0");
    test_number(1.0, "1");
    test_number(-1.0, "-1");
    test_number(1.5, "1.5");
    test_number(-1.5, "-1.5");
    test_number(3.14156, "3.14156");
    test_number(1E10, "1E10");
    test_number(1e10, "1e10");
    test_number(1E+10, "1E+10");
    test_number(1E-10, "1E-10");
    test_number(-1E10, "-1E10");
    test_number(-1e10, "-1e10");
    test_number(-1E+10, "-1E+10");
    test_number(-1e+10, "-1e+10");
    test_number(-1e-10, "-1e-10");
    test_number(1.234E10, "1.234E10");
    test_number(1.234e10, "1.234e10");
    test_number(5.0E-234, "5.0E-234");
    test_number(0, "1e-10000");
    test_number(1.0000000000000002, "1.0000000000000002");
    test_number(4.9406564584124654e-324, "4.9406564584124654e-324");
    test_number(-4.9406564584124654e-324, "-4.9406564584124654e-324");
    test_number(2.2250738585072009e-308, "2.2250738585072009e-308");
    test_number(-2.2250738585072009e-308, "-2.2250738585072009e-308");
    test_number(2.2250738585072014e-308, "2.2250738585072014e-308");
    test_number(-2.2250738585072014e-308, "-2.2250738585072014e-308");
    test_number(1.7976931348623157e+308, "1.7976931348623157e+308");
    test_number(-1.7976931348623157e+308, "-1.7976931348623157e+308");
    string errMsg;
    Json json = Json::parse("1.2e+12", errMsg);
    EXPECT_TRUE(json.json_value_is_Number());
    json = Json(3.1415);
    EXPECT_EQ(3.1415, json.json_value_to_Double());
}

TEST(Str2Json, JsonString)
{
    test_string("", "\"\"");
    test_string("Hello", "\"Hello\"");
    test_string("Hello\nWorld", "\"Hello\\nWorld\"");
    test_string("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");
    string s = "Hello";
    s.push_back('\0');
    s += "World";
    test_string(s, "\"Hello\\u0000World\"");
    test_string("\x24", "\"\\u0024\"");

    string errMsg;
    Json json = Json::parse("\"something\"", errMsg);
    json = Json("another thing");
    EXPECT_EQ(json.json_value_to_String(), "another thing");
}