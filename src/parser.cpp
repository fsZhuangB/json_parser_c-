#include "rafajson.hpp"

/* this function use the function below to parse JSON */
/* ws value ws */
int json_parse(json_value * value, std::string json)
{
    json_context c;
    // int ret;
    assert(value != nullptr);
    c.json = json;
    c.stack = nullptr;
    c.size = c.top = 0;
    JSON_INIT(value);

    value->type = json_type::JSON_NULL;
    json_parse_whiteSpace(&c);

    int retValue = json_parse_value(&c, value);
    if (retValue == JSON_PARSE_OK)
        {
            json_parse_whiteSpace(&c);
                // iterator of string
            std::string::const_iterator p = (c.json).begin();
            std::string::const_iterator p_end = (c.json).end();
            if (p != p_end)
            {
                value->type = json_type::JSON_NULL;
                retValue = JSON_PARSE_ROOT_NOT_SINGULAR;
            }
        }
    assert(c.top == 0);
    free(c.stack);
    return retValue;
}

/* this function parse the whitespace of JSON */
/* ws == *(%x20 / %x09 / %x0A / %x0D) */
static void json_parse_whiteSpace(json_context* c)
{
    // std::string p = c->json;
    std::string::const_iterator p = (c->json).begin();
    std::string::const_iterator e = (c->json).end();
    // std::string space = " ";
    // std::string tab = "\t";
    // std::string next = "\n";
    // std::string ret = "\r";
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = (c->json).assign(p, e);
}

/*
 * this function is used to parse null / false / true
 */
static int json_parse_iteral(json_context* c, json_value* value, std::string literal, json_type type)
{
    size_t i;
    std::string::const_iterator beg = (c->json).begin();
    std::string::const_iterator end = (c->json).end();
    std::string::const_iterator literalIterator = literal.begin();
    EXPECT(beg, literal[0]);
    // size_t i = 0;
    for (i = 0; literal[i + 1]; i++)
        if (*beg++ != literal[i + 1])
            return JSON_PARSE_INVALID_VALUE;


    // for (; beg != end; beg++)
    //         if (*beg != *++literalIterator)
    //                 return JSON_PARSE_INVALID_VALUE;
    c->json = (c->json).assign(beg, end);
    // c->json += i;  // the pointer point to the end of string
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
    std::string::const_iterator e = (c->json).end();
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

    value->n = strtod((c->json).c_str(), nullptr);
    if (errno == ERANGE && (value->n == HUGE_VAL || value->n == -HUGE_VALL))
        return JSON_PARSE_NUMBER_TOO_BIG;
    value->type =  json_type::JSON_NUMBER;
    c->json = (c->json).assign(p, e);
    return JSON_PARSE_OK;
}

void json_set_string(json_value* value, const char* s, size_t len)
{
    assert(value != nullptr && (s != nullptr || len == 0));
    json_free(value);
    value->s = new char;
    memcpy(std::get<char*>(value->s), s, len);  // use std::get<T> to get value from variant
    std::get<char*>(value->s)[len] = '\0';
    value->len = len;
    value->type = json_type::JSON_STRING;
}

void json_free(json_value* value)
{
    assert(value != nullptr);
    if (value->type == json_type::JSON_STRING)
        delete(std::get<char*>(value->s));
    value->type = json_type::JSON_NULL;
}

static void* json_context_push(json_context* c, size_t size)
{
    void* ret;
    assert(size > 0);
    if (c->top + size >= c->size)
    {
        if (c->size == 0)
            c->size = JSON_PARSE_STACK_INIT_SIZE;
        while (c->top + size >= c->size)
            c->size += c->size >> 1;  /* c->size * 1.5 */
        c->stack = (char*)realloc(c->stack, c->size);
    }
    ret = c->stack + c->top;
    c->top += size;
    return ret;
}

static void* json_context_pop(json_context* c, size_t size)
{
    assert(c->top >= size);
    return c->stack + (c->top -= size);
}

static int json_parse_string(json_context* c, json_value* value)
{
    size_t head = c->top, len;
    // const char* p;
    std::string::const_iterator p = (c->json).begin();
    std::string::const_iterator e = (c->json).end();
    EXPECT(p, '\"');
    c->json = (c->json).assign(p, e);
    for (;;)
    {
        char ch = *p++;
        switch (ch) 
        {
            case '\"':
                len = c->top - head;
                json_set_string(value, (const char*)json_context_pop(c, len), len);
                (c->json) = (c->json).assign(p, e);
                return JSON_PARSE_OK;
            case '\0':
                c->top = head;
                return JSON_PARSE_MISS_QUOTATION_MARK;
            default:
                PUTC(c, ch);

        }
        
    }
}