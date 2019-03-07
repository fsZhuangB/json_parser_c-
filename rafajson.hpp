#ifndef RAFAJSON_H__
#define RAFAJSON_H__
#include <string>

// enumerate six kind of json type
enum class json_type { JSON_NULL, JSON_FALSE, JSON_TRUE, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_OBJECT };

// define json's data structure
class json_value {
    json_type type;
};

/* this function parse the json*/
int json_parse(json_value* v, const std::string json);

#endif