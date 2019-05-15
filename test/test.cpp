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


/** EXPECT_EQ_INT
 * this macro get the EXPECT_EQ_BASE to show the int result 
 **/
#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

/** EXPECT_EQ_DOUBLE
 * this macro get the EXPECT_EQ_BASE to show the double result 
**/
#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%.17g");

/**
 * EXPECT_TRUE
 * this macro expect boolean true
**/
#define EXPECT_TRUE(actual) EXPECT_EQ_BASE((actual) != 0, "true", "false", "%s");

/**
 * EXPECT_FALSE
 * this macro expect boolean false
 **/
#define EXPECT_FALSE(actual) EXPECT_EQ_BASE((actual) == 0, "false", "true", "%s");

/**
 * EXPECT_EQ_SIZE_T
 * this macro test parse array API
 **/
#if defined(_MSC_VER)
#define EXPECT_EQ_SIZE_T(expect, actual) EXPECT_EQ_BASE((expect) == (actual), (size_t)expect, (size_t)actual, "%Iu")
#else
#define EXPECT_EQ_SIZE_T(expect, actual) EXPECT_EQ_BASE((expect) == (actual), (size_t)expect, (size_t)actual, "%zu")
#endif
/**
 * TEST_ERROR
 * simplify code to test error 
**/
#define TEST_ERROR(error, json)                                        \
    do                                                                 \
    {                                                                  \
        json_value value;                                              \
        value.type = json_type::JSON_FALSE;                            \
        EXPECT_EQ_INT(error, json_parse(&value, json));                \
        EXPECT_EQ_INT(json_type::JSON_NULL, json_get_type(&value));    \
    } while(0)


/**
 * EXPECT_EQ_DOUBLE
 * test double numbers
 **/
#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%.17g");

/**
 * TEST_NUMBER
 * test number
 **/
#define TEST_NUMBER(expect, json)                                      \
    do                                                                 \
    {                                                                  \
        json_value value;                                              \
        EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&value, json));        \
        EXPECT_EQ_INT(json_type::JSON_NUMBER, json_get_type(&value));  \
        EXPECT_EQ_DOUBLE(expect, json_get_number(&value));             \
    } while(0)

#define EXPECT_EQ_STRING(expect, actual, alength) \
        EXPECT_EQ_BASE(sizeof(expect) - 1 == alength, expect, actual, "%s")

#define TEST_STRING(expect, json)\
    do {\
        json_value v;\
        JSON_INIT(&v);\
        EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, json));\
        EXPECT_EQ_INT(json_type::JSON_STRING, json_get_type(&v));\
        EXPECT_EQ_STRING(expect, json_get_string(&v), json_get_string_length(&v));\
        json_free(&v);\
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
    # if 1
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "+0");  /* cannot use '+' as the first char of number */
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "+1");
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, ".123");/* at least one digit before '.' */
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "1.");  /* at least one digit after '.' */
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "INF");
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "inf");
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "NAN");
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "nan");
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

    TEST_ERROR(JSON_PARSE_ROOT_NOT_SINGULAR, "null x");

    #if 1
    TEST_ERROR(JSON_PARSE_ROOT_NOT_SINGULAR, "0123");
    TEST_ERROR(JSON_PARSE_ROOT_NOT_SINGULAR, "0x0");
    TEST_ERROR(JSON_PARSE_ROOT_NOT_SINGULAR, "0x123");
    #endif
}

static void test_parse_string() 
{
    TEST_STRING("", "\"\"");
    TEST_STRING("Hello", "\"Hello\"");
#if 1
    TEST_STRING("Hello\nWorld", "\"Hello\\nWorld\"");
    TEST_STRING("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");
#endif
    TEST_STRING("\x24", "\"\\u0024\"");         /* Dollar sign U+0024 */
    TEST_STRING("\xC2\xA2", "\"\\u00A2\"");     /* Cents sign U+00A2 */
    TEST_STRING("\xE2\x82\xAC", "\"\\u20AC\""); /* Euro sign U+20AC */
    TEST_STRING("\xF0\x9D\x84\x9E", "\"\\uD834\\uDD1E\"");  /* G clef sign U+1D11E */
    TEST_STRING("\xF0\x9D\x84\x9E", "\"\\ud834\\udd1e\"");  /* G clef sign U+1D11E */
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
    #if 1
    TEST_ERROR(JSON_PARSE_NUMBER_TOO_BIG, "1e309");
    TEST_ERROR(JSON_PARSE_NUMBER_TOO_BIG, "-1e309");
    #endif
}

static void test_acess_string() 
{
    json_value value;
    JSON_INIT(&value);
    json_set_string(&value, "", 0);
    EXPECT_EQ_STRING("", json_get_string(&value), json_get_string_length(&value));
    json_set_string(&value, "Hello", 5);
    EXPECT_EQ_STRING("Hello", json_get_string(&value), json_get_string_length(&value));
    json_free(&value);
}

static void test_access_boolean()
{
     json_value value;
     JSON_INIT(&value);
     json_set_string(&value, "a", 1);
     json_set_boolean(&value, 1);
     EXPECT_TRUE(json_get_boolean(&value));
     json_set_boolean(&value, 0);
     EXPECT_FALSE(json_get_boolean(&value));
     json_free(&value);
}

static void test_access_number()
{
    json_value value;
    JSON_INIT(&value);
    json_set_string(&value, "a", 1);
    json_set_number(&value, 1234.5);
    EXPECT_EQ_DOUBLE(1234.5, json_get_number(&value));
    json_free(&value);
}
static void test_parse_invalid_string_escape() {
    TEST_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE, "\"\\v\"");
    TEST_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE, "\"\\'\"");
    TEST_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE, "\"\\0\"");
    TEST_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE, "\"\\x12\"");
}

static void test_parse_invalid_string_char() {
    TEST_ERROR(JSON_PARSE_INVALID_STRING_CHAR, "\"\x01\"");
    TEST_ERROR(JSON_PARSE_INVALID_STRING_CHAR, "\"\x1F\"");
}

static void test_parse_missing_quotation_mark()
{
    TEST_ERROR(JSON_PARSE_MISS_QUOTATION_MARK, "\"");
    TEST_ERROR(JSON_PARSE_MISS_QUOTATION_MARK, "\"abc");
}

static void test_parse_invalid_unicode_hex() {
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u0\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u01\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u012\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u/000\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\uG000\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u0/00\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u0G00\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u00/0\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u00G0\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u000/\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u000G\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u 123\"");
}

static void test_parse_invalid_unicode_surrogate() {
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD800\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE, "\"\\uDBFF\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD800\\\\\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD800\\uDBFF\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD800\\uE000\"");
}

static void test_parse_miss_comma_or_square_bracket() {
#if 1
    TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[1");
    TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[1}");
    TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[1 2");
    TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[[]");
#endif
}

static void test_parse_array()
{
        size_t i, j;
        json_value value;

        JSON_INIT(&value);
        EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&value, "[ ]"));
        EXPECT_EQ_INT(json_type::JSON_ARRAY, json_get_type(&value));
        EXPECT_EQ_SIZE_T(0, json_get_array_size(&value));
        json_free(&value);

        JSON_INIT(&value);
        EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&value, "[ null, false, true, 123, \"abc\"]"));
        EXPECT_EQ_INT(json_type::JSON_ARRAY, json_get_type(&value));
        EXPECT_EQ_SIZE_T(5, json_get_array_size(&value));
        EXPECT_EQ_INT(json_type::JSON_NULL, json_get_type(json_get_array_element(&value, 0)));
        EXPECT_EQ_INT(json_type::JSON_FALSE, json_get_type(json_get_array_element(&value, 1)));
        EXPECT_EQ_INT(json_type::JSON_TRUE, json_get_type(json_get_array_element(&value, 2)));
        EXPECT_EQ_INT(json_type::JSON_NUMBER, json_get_type(json_get_array_element(&value, 3)));
        EXPECT_EQ_INT(json_type::JSON_STRING, json_get_type(json_get_array_element(&value, 4)));
        EXPECT_EQ_DOUBLE(123.0, json_get_number(json_get_array_element(&value, 3)));
        EXPECT_EQ_STRING("abc", json_get_string(json_get_array_element(&value, 4)), json_get_string_length(json_get_array_element(&value, 4)));
        json_free(&value);

        JSON_INIT(&value);
        EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&value, "[[ ], [0], [0, 1], [0, 1, 2]]"));
        EXPECT_EQ_INT(json_type::JSON_ARRAY, json_get_type(&value));
        EXPECT_EQ_SIZE_T(4, json_get_array_size(&value));
        for (i = 0; i < 4; i++)
        {
            json_value* a = json_get_array_element(&value, i);
            EXPECT_EQ_INT(json_type::JSON_ARRAY, json_get_type(a));
            EXPECT_EQ_SIZE_T(i, json_get_array_size(a));
            for (j = 0; j < i; j++)
            {
                json_value* e = json_get_array_element(a, j);
                EXPECT_EQ_INT(json_type::JSON_NUMBER, json_get_type(e));
                EXPECT_EQ_DOUBLE((double)j, json_get_number(e));
            }
        }
        json_free(&value);

}
static void test_parse() {
     test_parse_null();
     test_parse_true();
     test_parse_false();
     test_access_boolean();
     test_parse_invalid_value();
     test_parse_expect_value();
     test_parse_root_not_singular();
     test_parse_number_too_big();
     test_parse_number();
     test_access_number();
     test_parse_string();
     test_acess_string();
     test_parse_invalid_string_char();
     test_parse_invalid_string_escape();
     test_parse_missing_quotation_mark();
     test_parse_invalid_unicode_hex();
     test_parse_invalid_unicode_surrogate();
     test_parse_array();
    test_parse_miss_comma_or_square_bracket();
    /* ... */
}

int main() {
    test_parse();
    std::cout << test_pass << "/" << test_count << " (" << test_pass * 100.0 / test_count << ") " << "passed" << std::endl;
    return main_ret;
}