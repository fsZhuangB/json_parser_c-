#ifndef RAFAJSON_H__
#define RAFAJSON_H__
#include <string>

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
    json_type type;
};

// the return value
enum {
    JSON_PARSE_OK = 0,
    JSON_PARSE_EXPECT_VALUE,
    JSON_PARSE_INVALID_VALUE,
    JSON_PARSE_ROOT_NOT_SINGULAR
};

/* this function parse the json*/
int json_parse(json_value* value, const std::string json);

/* this function get the type of result */
json_type json_get_type(const json_value* value);

#endif