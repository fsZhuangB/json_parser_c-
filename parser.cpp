#include "rafajson.hpp"
#define EXPECT(c, ch) do { assert(*c->json == (ch)); c->json++; } while (0)

/* this function use the function below to parse JSON */
int json_parse(json_value * value, const char* json)
{
    json_context c;
    assert(value != nullptr);
    c.json = json;
    value->type = json_type::JSON_NULL;
    json_parse_whiteSpace(&c);

    int retValue = json_parse_value(&c, value);
    if (retValue == JSON_PARSE_OK)
        {
            json_parse_whiteSpace(&c);
            if (*c.json != '\0')
                retValue = JSON_PARSE_ROOT_NOT_SINGULAR;
        }
    return retValue;
}

/* this function parse the whitespace of JSON */
/* ws = *(%x20 / %x09 / %x0A / %x0D) */
static void json_parse_whiteSpace(json_context* c)
{
    const char* p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

/* this function parse null of JSON */
/* null = "null" */
static int json_parse_null(json_context* c, json_value* value)
{
    EXPECT(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return JSON_PARSE_INVALID_VALUE;
    c->json += 3;  // move 3 characters 
    value->type = json_type::JSON_NULL;
    return JSON_PARSE_OK;
}

/* this function parse the 3 value of json */
/* value = null / false / true */
static int json_parse_value(json_context* c, json_value* value)
{
    switch (*c->json)
    {
        case 'n':  return json_parse_null(c, value);
        case '\0': return JSON_PARSE_EXPECT_VALUE;
        default:   return JSON_PARSE_INVALID_VALUE;
    }
}

/* this function get the type of result */
json_type json_get_type(const json_value* value)
{
    assert(value != nullptr);
    return value->type;
}

