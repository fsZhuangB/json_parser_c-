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
    test_string("\xC2\xA2", "\"\\u00A2\"");
    test_string("\xE2\x82\xAC", "\"\\u20AC\"");
    test_string("\xF0\x9D\x84\x9E", "\"\\uD834\\uDD1E\"");
    test_string("\xF0\x9D\x84\x9E", "\"\\ud834\\udd1e\"");

    string errMsg;
    Json json = Json::parse("\"something\"", errMsg);
    json = Json("another thing");
    EXPECT_EQ(json.json_value_to_String(), "another thing");
}

TEST(Str2Json, JsonArray)
{
    Json json = parse_ok("[ ]");
    EXPECT_TRUE(json.json_value_is_Array());
    EXPECT_EQ(json.json_get_size(), 0);

    json = parse_ok("[null, false, true, 123, \"abc\"]");
    EXPECT_TRUE(json.json_value_is_Array());
    EXPECT_EQ(json.json_get_size(), 5);
    EXPECT_EQ(json[0], Json(nullptr));
    EXPECT_EQ(json[1], Json(false));
    EXPECT_EQ(json[2], Json(true));
    EXPECT_EQ(json[3], Json(123.0));
    EXPECT_EQ(json[4], Json("abc"));

//    json = parse_ok("[[], [0], [0, 1], [0, 1, 2]]");
//    EXPECT_TRUE(json.json_value_is_Array());
//    EXPECT_EQ(json.json_get_size(), 4);
//    for (int i = 0; i < 4; i++)
//    {
//        Json json2 = json[i];
//        EXPECT_TRUE(json2.json_value_is_Array());
//        EXPECT_EQ(json2.json_get_size(), i);
//        for (int j = 0; j <= i; j++)
//        {
//            EXPECT_TRUE(json2.json_value_is_Array());
//            EXPECT_EQ(json2.json_get_size(), j);
//            EXPECT_EQ(json2[j], j);
//        }
//    }
}

TEST(Str2Json, JsonObject) {
    Json json = parse_ok("{ }");
    EXPECT_TRUE(json.json_value_is_Object());
    EXPECT_EQ(json.json_get_size(), 0);

    json = parse_ok(" { "
                   "\"n\" : null , "
                   "\"f\" : false , "
                   "\"t\" : true , "
                   "\"i\" : 123 , "
                   "\"s\" : \"abc\", "
                   "\"a\" : [ 1, 2, 3 ],"
                   "\"o\" : { \"1\" : 1, \"2\" : 2, \"3\" : 3 }"
                   " } ");
    EXPECT_TRUE(json.json_value_is_Object());
    EXPECT_EQ(json.json_get_size(), 7);

    EXPECT_TRUE(json["n"].json_value_is_Null());

    EXPECT_TRUE(json["f"].json_value_is_Bool());
    EXPECT_EQ(json["f"].json_value_to_Bool(), false);

    EXPECT_TRUE(json["t"].json_value_is_Bool());
    EXPECT_EQ(json["t"].json_value_to_Bool(), true);

    EXPECT_TRUE(json["i"].json_value_is_Number());
    EXPECT_EQ(json["i"].json_value_to_Double(), 123.0);

    EXPECT_TRUE(json["s"].json_value_is_String());
    EXPECT_EQ(json["s"].json_value_to_String(), "abc");

    EXPECT_TRUE(json["a"].json_value_is_Array());
    EXPECT_EQ(json["a"].json_get_size(), 3);

    EXPECT_TRUE(json["o"].json_value_is_Object());
    EXPECT_EQ(json["o"].json_get_size(), 3);
}