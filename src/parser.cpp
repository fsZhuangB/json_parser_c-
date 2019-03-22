#include "rafajson.hpp"

#define EXPECT(c, ch) do { assert(*c == (ch)); c++; } while (0)

/* this function use the function below to parse JSON */
/* ws value ws */
int json_parse(json_value * value, std::string json)
{
    json_context c;
    // iterator of json
    std::string::const_iterator p = (c.json).begin();
    assert(value != nullptr);
    c.json = json;
    value->type = json_type::JSON_NULL;
    json_parse_whiteSpace(&c);

    int retValue = json_parse_value(&c, value);
    if (retValue == JSON_PARSE_OK)
        {
            json_parse_whiteSpace(&c);
            if (*p != '\0')
            {
                value->type = json_type::JSON_NULL;
                retValue = JSON_PARSE_ROOT_NOT_SINGULAR;
            }
        }
    return retValue;
}

/* this function parse the whitespace of JSON */
/* ws == *(%x20 / %x09 / %x0A / %x0D) */
static void json_parse_whiteSpace(json_context* c)
{
    // std::string p = c->json;
    std::string::const_iterator p = (c->json).begin();
    // std::string space = " ";
    // std::string tab = "\t";
    // std::string next = "\n";
    // std::string ret = "\r";
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = *p;
}

/*
 * this function is used to parse null / false / true
 */
static int json_parse_iteral(json_context* c, json_value* value, std::string literal, json_type type)
{
    size_t i;
    std::string::const_iterator beg = (c->json).begin();
    EXPECT(beg, literal[0]);
    for (i = 0; literal[i + 1];i++)
        if (c->json[i] != literal[i+1])
            return JSON_PARSE_INVALID_VALUE;
    c->json += i;  // the pointer point to the end of string
    value->type = type;
    return JSON_PARSE_OK;
}


/* this function parse the 3 value of json */
/* value = null / false / true / number */
static int json_parse_value(json_context* c, json_value* value)
{
    std::string::const_iterator beg = (c->json).begin();
    switch (*beg)
    {
        case 'n':  return json_parse_iteral(c, value, "null", json_type::JSON_NULL);
        case 't':  return json_parse_iteral(c, value, "true", json_type::JSON_TRUE);
        case 'f':  return json_parse_iteral(c, value, "false", json_type::JSON_FALSE);
        case '\0': return JSON_PARSE_EXPECT_VALUE;
        default:   return json_parse_number(c, value);
    }
}

/* this function get the type of result */
json_type json_get_type(const json_value* value)
{
    assert(value != nullptr);
    return value->type;
}

/* this function get the number of json */
double json_get_number(const json_value* value)
{
    assert(value != nullptr && value->type == json_type::JSON_NUMBER);
    return value->n;
}

/* this function parse the number of json 
 * use c library function strtod()
*/

static int json_parse_number(json_context* c, json_value* value)
{
    char * end;
    std::string::const_iterator p = (c->json).begin();
    /* parse '-' */
    if (*p == '-')
        p++;

    /* parse int */
    if (*p == '0')
        p++;
    else 
    {
        if (!ISDIGIT1TO9(*p))
            return JSON_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++)
            ;
    }

    /* parse frac */
    if (*p == '.')
    {
        p++;
        if (!ISDIGIT(*p))
            return JSON_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++)
            ;
    }

    /* parse exp */
    if (*p == 'e' || *p == 'E')
    {
        p++;
        if (*p == '+' || *p == '-') 
            p++;
        if (!ISDIGIT(*p)) 
            return JSON_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }

    value->n = strtod(c->json, nullptr);
    if (errno == ERANGE && (value->n == HUGE_VAL || value->n == -HUGE_VALL))
        return JSON_PARSE_NUMBER_TOO_BIG;
    value->type =  json_type::JSON_NUMBER;
    c->json = p;
    return JSON_PARSE_OK;
}