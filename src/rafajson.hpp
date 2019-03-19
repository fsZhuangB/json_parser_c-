#ifndef RAFAJSON_H__
#define RAFAJSON_H__
// #include <string>
#include <assert.h>

// enumerate six kind of json type
enum class json_type { 
                       JSON_NULL,
                       JSON_FALSE,
                       JSON_TRUE,
                       JSON_NUMBER,
                       JSON_STRING,
                       JSON_ARRAY,
                       JSON_OBJECT 
                    };


// define json's data structure
class json_value {
    public:
    double n;
    json_type type;
};

/* store the context of JSON text */
struct json_context {
    const char* json;
};

// the return value of result
enum {
    JSON_PARSE_OK = 0,
    JSON_PARSE_EXPECT_VALUE,
    JSON_PARSE_INVALID_VALUE,
    JSON_PARSE_ROOT_NOT_SINGULAR,
    JSON_PARSE_NUMBER_TOO_BIG
};

/* this function parse the json*/
int json_parse(json_value* value, const char* json);

/* this function get the type of result */
json_type json_get_type(const json_value* value);

/* ws = *(%x20 / %x09 / %x0A / %x0D) */
static void json_parse_whiteSpace(json_context* c);

/* value == null / false / true */
static int json_parse_value(json_context* c, json_value* value);

/* null == "null" */
// static int json_parse_null(json_context* c, json_value* value);

// /* true == "true" */
// static int json_parse_true(json_context* c, json_value* value);

// /* false == "false" */
// static int json_parse_false(json_context* c, json_value* value);

/* this function get the number of json */
double json_get_number(const json_value* value);

/* 
 * this function parse the number of json 
 * use c library function strtod()
 */
static int json_parse_number(json_context* c, json_value* value);

/*
 * this function is used to parse null / false / true
 */
static int json_parse_iteral(json_context* c, json_value* value, const char* literal, json_type type);

#endif