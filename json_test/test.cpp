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

#define test_number(expect, content)                        \
do\
{\
    Json json = parse_ok(content);\
    EXPECT_TRUE(json.json_value_is_Number());\
    EXPECT_EQ(json.json_value_to_Double(), expect);\
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
}