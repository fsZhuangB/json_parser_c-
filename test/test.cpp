/* test file */

#include <iostream>
#include "../src/rafajson.hpp"
#include "test.hpp"
static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

/* EXPECT_EQ_BASE
 * this macro get four parameters
 * if it fails, print stderr to screen 
 */
#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do                                                   \
    {                                                    \
        test_count++;                                    \
        if (equality)                                    \
            test_pass++;                                 \
        else {                                           \
            fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
            main_ret = 1;                                \
        }                                                \
    } while(0)


/* EXPECT_EQ_INT
 * this macro get the EXPECT_EQ_BASE to show the int result 
 */
#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

/* EXPECT_EQ_DOUBLE
 * this macro get the EXPECT_EQ_BASE to show the double result 
 */
#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%.17g");

/* TEST_ERROR 
 * simplify code to test error 
 */
#define TEST_ERROR(error, json)                                        \
    do                                                                 \
    {                                                                  \
        json_value value;                                              \
        value.type = json_type::JSON_FALSE;                            \
        EXPECT_EQ_INT(error, json_parse(&value, json));                \
        EXPECT_EQ_INT(json_type::JSON_NULL, json_get_type(&value));    \
    } while(0)

#define TEST_NUMBER(expect, json)                                      \
    do                                                                 \
    {                                                                  \
        json_value value;                                              \
        EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&value, json));        \
        EXPECT_EQ_INT(json_type::JSON_NUMBER, json_get_type(&value));  \
        EXPECT_EQ_DOUBLE(expect, json_get_number(&value));             \
    } while(0)

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


static void test_parse_invalid_value() {
    /* invalid number */
    # if 0
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "+0");  /* cannot use '+' as the first char of number */
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "+1");
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, ".123");/* at least one digit before '.' */
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "1.");  /* at least one digit after '.' */
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "INF");
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "inf");
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "NAN");
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "nan");
    TEST_ERROR(JSON_PARSE_INVALID_VALUE);
    /* other invalid value */
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "nul");
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "?");
    #endif
}

static void test_parse_expect_value() {
    /* parse failed and return null */
    TEST_ERROR(JSON_PARSE_EXPECT_VALUE, "");
    TEST_ERROR(JSON_PARSE_EXPECT_VALUE, " ");
}

static void test_parse_root_not_singular() {
    json_value value;
    value.type = json_type::JSON_FALSE;

    EXPECT_EQ_INT(JSON_PARSE_ROOT_NOT_SINGULAR, json_parse(&value, "null x"));
    EXPECT_EQ_INT(json_type::JSON_NULL, json_get_type(&value));

    #if 0
    TEST_ERROR(JSON_PARSE_ROOT_NOT_SINGULAR, "0123");
    TEST_ERROR(JSON_PARSE_ROOT_NOT_SINGULAR, "0x0");
    TEST_ERROR(JSON_PARSE_ROOT_NOT_SINGULAR, "0x123");
    #endif
}

static void test_parse_number() {
    TEST_NUMBER(0.0, "0");
    TEST_NUMBER(0.0, "-0");
    TEST_NUMBER(0.0, "-0.0");
    TEST_NUMBER(1.0, "1");
    TEST_NUMBER(-1.0, "-1");
    TEST_NUMBER(1.5, "1.5");
    TEST_NUMBER(-1.5, "-1.5");
    TEST_NUMBER(3.1416, "3.1416");
    TEST_NUMBER(1E10, "1E10");
    TEST_NUMBER(1e10, "1e10");
    TEST_NUMBER(1E+10, "1E+10");
    TEST_NUMBER(1E-10, "1E-10");
    TEST_NUMBER(1e-10, "1e-10");
    TEST_NUMBER(-1E10, "-1E10");
    TEST_NUMBER(-1e10, "-1e10");
    TEST_NUMBER(-1E+10, "-1E+10");
    TEST_NUMBER(-1E-10, "-1E-10");
    TEST_NUMBER(1.234E+10, "1.234E+10");
    TEST_NUMBER(1.234E-10, "1.234E-10");
    TEST_NUMBER(0.0, "1e-10000"); /* underflow */
    TEST_NUMBER(1.0000000000000002, "1.0000000000000002"); /* smallest number > 1 */
    TEST_NUMBER( 4.9406564584124654e-324, "4.9406564584124654e-324"); /* minimum denormal */
    TEST_NUMBER(-4.9406564584124654e-324, "-4.9406564584124654e-324");
    TEST_NUMBER( 2.2250738585072009e-308, "2.2250738585072009e-308");  /* Max subnormal double */
    TEST_NUMBER(-2.2250738585072009e-308, "-2.2250738585072009e-308");
    TEST_NUMBER( 2.2250738585072014e-308, "2.2250738585072014e-308");  /* Min normal positive double */
    TEST_NUMBER(-2.2250738585072014e-308, "-2.2250738585072014e-308");
    TEST_NUMBER( 1.7976931348623157e+308, "1.7976931348623157e+308");  /* Max double */
    TEST_NUMBER(-1.7976931348623157e+308, "-1.7976931348623157e+308");
}

static void test_parse_number_too_big()
{
    #if 0
    TEST_ERROR(JSON_PARSE_NUMBER_TOO_BIG, "1e309");
    TEST_ERROR(JSON_PARSE_NUMBER_TOO_BIG, "-1e309");
    #endif
}

static void test_parse() {
    test_parse_null();
    test_parse_true();
    test_parse_false();
    test_parse_invalid_value();
    test_parse_expect_value();
    test_parse_root_not_singular();
    test_parse_number_too_big();
    /* ... */
}

int main() {
    test_parse();
    std::cout << test_pass << "/" << test_count << " (" << test_pass * 100.0 / test_count << ") " << "passed" << std::endl;
    return main_ret;
}