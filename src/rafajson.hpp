#include <iostream>
#include <variant>
#include <string>
#include <assert.h>
#include <errno.h>
#include <cmath>
#include <string>
#include <variant>
#include <stack>

#ifndef RAFAJSON_H__
#define RAFAJSON_H__
#define ISDIGIT(ch) ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch) ((ch) >= '1' && (ch) <= '9')
#define EXPECT(c, ch) do { assert(*c->json == (ch)); c->json++; } while (0)
#define JSON_INIT(value) do {(value)->type = json_type::JSON_NULL;} while(0)
#define JSON_SET_NULL(value) json_free(value)
#define PUTC(c, ch) do { *(char*) json_context_push(c, sizeof(char)) = (ch); } while(0)
#define STRING_ERROR(ret) do { c->top = head; return ret; } while(0)// TODO
#ifndef JSON_PARSE_STACK_INIT_SIZE
#define JSON_PARSE_STACK_INIT_SIZE 256
#endif


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

class json_member;
// define json's data structure
class json_value {
    public:
    /**
     * object
     * */
     std::variant<json_member* , size_t>m, size_of_object;
    /**
     * string
     * */
    std::variant<char *, size_t>s, len;

    /**
     * array
     * */
    std::variant<json_value* , size_t> e, size;

    /**
     * number
     * */
    double n;
    json_type type;
};

class json_member
{
public:
    char * k;
    size_t klen;
    json_value value;
};

/* store the context of JSON text */
class json_context {
    public:
    const char* json;
    char* stack;
    size_t size, top;
};

// the return value of result
enum {
    JSON_PARSE_OK = 0,
    JSON_PARSE_EXPECT_VALUE,
    JSON_PARSE_INVALID_VALUE,
    JSON_PARSE_ROOT_NOT_SINGULAR,
    JSON_PARSE_NUMBER_TOO_BIG,
    JSON_PARSE_MISS_QUOTATION_MARK,
    JSON_PARSE_INVALID_STRING_ESCAPE,
    JSON_PARSE_INVALID_STRING_CHAR,
    JSON_PARSE_INVALID_UNICODE_HEX,
    JSON_PARSE_INVALID_UNICODE_SURROGATE,
    JSON_PARSE_MISS_COMMA_OR_SQUARE_BRACKET,
    JSON_PARSE_MISS_KEY,
    JSON_PARSE_MISS_COLON,
    JSON_PARSE_MISS_COMMA_OR_CURLY_BRACKET
};

/* this function parse the json*/
int json_parse(json_value* value, const char* json);

/* this function get the type of result */
json_type json_get_type(const json_value* value);

/* ws = *(%x20 / %x09 / %x0A / %x0D) */
static void json_parse_whiteSpace(json_context* c);

/* value == null / false / true */
static int json_parse_value(json_context* c, json_value* value);

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

/*
 * this function set a value as string
 */
void json_set_string(json_value* value, const char* s, size_t len);

/*
 * this function free the memory of the value
 */
void json_free(json_value* value);

int json_get_boolean(const json_value* value);
void json_set_boolean(json_value* value, int b);

double json_get_number(const json_value* value);
void json_set_number(json_value* value, double n);

const char* json_get_string(const json_value* value);
size_t json_get_string_length(const json_value* value);
// void json_set_string(json_value* value, const char* s, size_t len);

static void* json_context_push(json_context* c, size_t size);

static void* json_context_pop(json_context* c, size_t size);

static int json_parse_string(json_context* c, json_value* value);

static const char* json_parse_hex4(const char *p, unsigned* u);

static void json_encode_utf8(json_context* c, unsigned u);

size_t json_get_array_size(const json_value * value);

json_value* json_get_array_element(const json_value* value, size_t index);

static int json_parse_array(json_context* c, json_value* value);

size_t json_get_object_size(const json_value* value);

const char* json_get_object_key(const json_value* value, size_t index);

size_t json_get_object_key_length(const json_value* value, size_t index);

json_value* json_get_object_value(const json_value* value, size_t index);

/**
 * parse JSON string, write the result into str and len
 * \TODO
 */
 static int json_parse_string_raw(json_context* c, char** str, size_t* len);

static int json_parse_object(json_context* c, json_value* value);

#endif