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

#define test_null(strJson) \
 do                             \
     {\
    Json json = parse_ok;\
    EXPECT_TRUE(json.json_value_is_Null());\
   } while(0)

#define test_bool(expect, content) \
do \
{\
    Json json = parse_ok(content)\
    EXPECT_TRUE(json.json_value_is_bool()) \
    EXPECT_EQ(json.json_value_to_bool())\
    json = Json(!expect);\
    EXPECT_EQ(json.json_value_to_bool(), !expect);\
} while(0)

TEST(Str2Json, JsonNull)
{
    test_null("null");
    test_null("   null\n\r\t");
}