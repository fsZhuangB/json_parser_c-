/* test file */

#include <iostream>
#include "rafajson.hpp"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

/* this macro get four parameters */
/* if it fails, print stderr to screen */
#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do {\
        test_count++;\
        if (equality)\
            test_pass++;\
        else {\
            fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
            main_ret = 1;\
        }\
    } while(0)


/* this macro get the EXPECT_EQ_BASE to show the result */
#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

static void test_parse_null() {
    json_value value;
    value.type = json_type::JSON_NULL;
    EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&value, "null"));
    EXPECT_EQ_INT(json_type::JSON_NULL, json_get_type(&value));
}

static void test_parse_true() {
    json_value value;
    value.type = json_type::JSON_TRUE;
    EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&value, "true"));
    EXPECT_EQ_INT(json_type::JSON_TRUE, json_get_type(&value));
}

static void test_parse_false() {
    json_value value;
    value.type = json_type::JSON_FALSE;
    EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&value, "false"));
    EXPECT_EQ_INT(json_type::JSON_FALSE, json_get_type(&value));
}

static void test_invalid_value() {
    json_value value;
    value.type = json_type::JSON_FALSE;
    EXPECT_EQ_INT(JSON_PARSE_INVALID_VALUE, json_parse(&value, "nul"));
    EXPECT_EQ_INT(json_type::JSON_NULL, json_get_type(&value));

    value.type = json_type::JSON_FALSE;
    EXPECT_EQ_INT(JSON_PARSE_INVALID_VALUE, json_parse(&value, "?"));
    EXPECT_EQ_INT(json_type::JSON_NULL, json_get_type(&value));
}

static void test_parse_expect_value() {
    json_value value;
    
    value.type = json_type::JSON_FALSE;

}

static void test_parse() {
    test_parse_null();
    test_parse_true();
    test_parse_false();
    test_invalid_value();
    /* ... */
}

int main() {
    test_parse();
    std::cout << test_pass << "/" << test_count << " (" << test_pass * 100.0 / test_count << ") " << "passed" << std::endl;
    return main_ret;
}