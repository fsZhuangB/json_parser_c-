#include "rafajson.hpp"
#define EXPECT(c, ch) do { assert(*c->json == (ch)); c->json++; } while (0)

/* this function use the function below to parse JSON */
/* ws value ws */
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
/* ws == *(%x20 / %x09 / %x0A / %x0D) */
static void json_parse_whiteSpace(json_context* c)
{
    const char* p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

/* this function parse null of JSON */
/* null == "null" */
// static int json_parse_null(json_context* c, json_value* value)
// {
//     EXPECT(c, 'n');
//     if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
//         return JSON_PARSE_INVALID_VALUE;
//     c->json += 3;  // move 3 characters 
//     value->type = json_type::JSON_NULL;
//     return JSON_PARSE_OK;
// }

// /* this function parse true of JSON */
// /* true == "true" */
// static int json_parse_true(json_context* c, json_value* value)
// {
//     EXPECT(c, 't');
//     if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
//         return JSON_PARSE_INVALID_VALUE;
//     c->json += 3;
//     value->type = json_type::JSON_TRUE;
//     return JSON_PARSE_OK;
// }

// /* this function parse false of JSON */
// /* false == "false" */
// static int json_parse_false(json_context* c, json_value* value)
// {
//     EXPECT(c, 'f');
//     if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e')
//         return JSON_PARSE_INVALID_VALUE;
//     c->json += 4;
//     value->type = json_type::JSON_FALSE;
//     return JSON_PARSE_OK;
// }

/*
 * this function is used to parse null / false / true
 */
static int json_parse_iteral(json_context* c, json_value* value, const char* literal, json_type type)
{
    size_t i;
    EXPECT(c, literal[0]);
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
    switch (*c->json)
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
    /* TODO validate number */
    value->n = strtod(c->json, &end);

    /* if json value == end */
    if (c->json == end)
        return JSON_PARSE_INVALID_VALUE;
    c->json = end;
    value->type =  json_type::JSON_NUMBER;
    return JSON_PARSE_OK;
}