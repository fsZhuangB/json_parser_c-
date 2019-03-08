/* a test function for my library */

#include <iostream>
#include <stdlib.h>
#include "rafajson.hpp"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do {\
        test_count++;\
        if (equality)\
            test_pass++;\
        else {\
            fprintf(stderr, "%s:%d: expect: " formart " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
            main_ret = 1;\
        }\
    } while(0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

static void test_parse_null() {
    json_value v;
    v.type = json_type::JSON_TRUE;
    EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "null");
    EXPECT_EQ_INT(JSON_NULL, json_get_type(&v);
}

static void test_parse()
{
    test_parse_null();
}

int main()
{
    return 0;
}