#include "rafajson.hpp"

/* this function use the function below to parse JSON */
/* ws value ws */
int json_parse(json_value * value, const char* json)
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
         if (*c.json != '\0')
            {
                value->type = json_type::JSON_NULL;
                retValue = JSON_PARSE_ROOT_NOT_SINGULAR;
            }
        }
    assert(c.top == 0);
    delete(c.stack);
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

/*
 * this function is used to parse null / false / true
 */
static int json_parse_iteral(json_context* c, json_value* value, const char* literal, json_type type)
{
    size_t i;
    EXPECT(c, literal[0]);
    // size_t i = 0;
    for (i = 0; literal[i + 1]; i++)
        if (c->json[i] != literal[i + 1])
            return JSON_PARSE_INVALID_VALUE;

    c->json += i;
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
        case '"':  return json_parse_string(c, value);
        case '[':  return json_parse_array(c, value);
        case '{':  return json_parse_object(c, value);
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
    const char* p = c->json;

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

/**
 * this function set a value as string
**/
void json_set_string(json_value* value, const char* s, size_t len)
{
    assert(value != nullptr && (s != nullptr || len == 0));
    json_free(value);
    value->s = new char;
    memcpy(std::get<char *>(value->s), s, len);
    (std::get<char *>(value->s))[len] = '\0';
    value->len = len;
    value->type = json_type::JSON_STRING;
}

// clear the string
void json_free(json_value* value)
{
    size_t i;
    assert(value != nullptr);
    switch (value->type)
    {
        case json_type::JSON_STRING :
            delete(std::get<char *>(value->s));
            break;

            /**
             * First we should free the object in the array recursively
             * */
        case json_type::JSON_ARRAY :
            for (i = 0; i < std::get<size_t>(value->size); i++)
                json_free(&std::get<json_value*>(value->e)[i]);
            delete(std::get<json_value*>(value->e));
            break;
        case json_type::JSON_OBJECT:
            for (i = 0; i < std::get<size_t >(value->size); i++)
            {
                delete(std::get<json_member*>(value->m)[i].k);
                json_free(&std::get<json_member*>(value->m)[i].value);
            }
            delete(std::get<json_member*>(value->m));
            break;

        default:
            break;
    }
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

static void* json_context_pop(json_context *c, size_t size)
{
    assert(c->top >= size);
    return c->stack + (c->top -= size); 
}

static int json_parse_string(json_context* c, json_value* value)
{
    int ret;
    char* s;
    size_t len;

    if ((ret = json_parse_string_raw(c, &s, &len)) == JSON_PARSE_OK)
        json_set_string(value, s, len);
    return ret;
}

const char* json_get_string(const json_value* value)
{
    assert(value != nullptr && value->type == json_type::JSON_STRING);
    return std::get<char *>(value->s);
}

size_t json_get_string_length(const json_value* value)
{
    assert(value != nullptr && value->type == json_type::JSON_STRING);
    return std::get<size_t>(value->len);
}

int json_get_boolean(const json_value* value)
{
    assert(value != nullptr && (value->type == json_type::JSON_TRUE) || (value->type == json_type::JSON_FALSE));
    return value->type == json_type::JSON_TRUE;
}

void json_set_boolean(json_value* value, int b)
{
    json_free(value);
    value->type = b ? json_type::JSON_TRUE : json_type::JSON_FALSE;
}


void json_set_number(json_value* value, double n)
{
    json_free(value);
    value->n = n;
    value->type = json_type::JSON_NUMBER;
}

static const char* json_parse_hex4(const char* p, unsigned* u)
{
    int i;
    *u = 0;
    for (i = 0; i < 4; i++)
    {
        char ch = *p++;
        *u <<= 4;
        if (ch >= '0' && ch <= '9')
            *u |= ch - '0';
        else if (ch >= 'A' && ch <= 'F')
            *u |= ch - ('A' - 10);
        else if (ch >= 'a' && ch <= 'f')
            *u |= ch - ('a' - 10);
        else
            return nullptr;
    }
    return p;
}

static int json_parse_value(json_context* c, json_value * value);

static int json_parse_array(json_context* c, json_value* value)
{
    size_t i, size = 0;
    int ret;
    EXPECT(c, '[');
    json_parse_whiteSpace(c);

    if (*c->json == ']')
    {
        c->json++;
        value->type = json_type::JSON_ARRAY;
        value->size = 0;
        value->e = nullptr;
        return JSON_PARSE_OK;
    }

    for (;;)
    {
        json_value e;
        JSON_INIT(&e);

        /**
         * call json_parse_value() parse the value
         * json_value e is a temporary value
         * */
        ret = json_parse_value(c, &e);
        if (ret != JSON_PARSE_OK)
            break;
        json_parse_whiteSpace(c);
        memcpy(json_context_push(c, sizeof(json_value)), &e, sizeof(json_value));
        size++;
        if (*c->json == ',')
        {
            c->json++;
            json_parse_whiteSpace(c);
        }
        else if (*c->json == ']')
        {
            c->json++;
            value->type = json_type::JSON_ARRAY;
            value->size = size;
            size *= sizeof(json_value);

            /**
             * copy the value from stack
             * */
            memcpy(std::get<json_value*>(value->e) = (json_value*)malloc(size), json_context_pop(c, size), size);
            return JSON_PARSE_OK;
        }
        else
        {
            ret = JSON_PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
            break;
        }
    }
    /**
     * Pop and free values on the stack
     * */
     // size_t i;
     for (i = 0; i < size; i++)
         json_free((json_value*)json_context_pop(c, sizeof(json_value)));
     return ret;
}


static void json_encode_utf8(json_context* c, unsigned u)
{
    if (u <= 0x7F)
        PUTC(c, u & 0x7F);
    else if (u <= 0x7FF)
    {
        PUTC(c, 0xC0 | ((u >> 6) & 0xFF));
        PUTC(c, 0x80 | (u        & 0x3F));
    }
    else if (u < 0xFFFF)
    {
        PUTC(c, 0xE0 | ((u >> 12) & 0xFF));
        PUTC(c, 0x80 | ((u >> 6)  & 0x3F));
        PUTC(c, 0x80 | (u         & 0x3F));
    }
    else
    {
        assert(u <= 0x10FFFF);
        PUTC(c, 0xF0 | ((u >> 18) & 0xFF));
        PUTC(c, 0x80 | ((u >> 12) & 0x3F));
        PUTC(c, 0x80 | ((u >>  6) & 0x3F));
        PUTC(c, 0x80 | ( u        & 0x3F));
    }
}

size_t json_get_array_size(const json_value* value)
{
    assert(value != nullptr && value->type == json_type::JSON_ARRAY);
    return std::get<size_t>(value->size);
}

json_value* json_get_array_element(const json_value* value, size_t index)
{
    assert(value != nullptr && value->type == json_type::JSON_ARRAY);
    assert(index < std::get<size_t>(value->size));
    return &std::get<json_value*>(value->e)[index];
}

static int json_parse_string_raw(json_context* c, char** str, size_t* len)
{
    /**
     * \TODO
     */
    unsigned u;
    unsigned u2;
    size_t head = c->top;
    const char * p;
    EXPECT(c, '\"');
    p = c->json;
    for (;;)
    {
        char ch = *p++;
        switch (ch)
        {
            case '\"':
                *len = c->top - head;
                *str = (char *)json_context_pop(c, *len);
                c->json = p;
                return JSON_PARSE_OK;
            case '\0':
                // c->top = head;
                STRING_ERROR(JSON_PARSE_MISS_QUOTATION_MARK);
                /* parse the sequence of escape character */
            case '\\':
                switch (*p++)
                {
                    case '\"': PUTC(c, '\"');
                        break;
                    case '\\': PUTC(c, '\\');
                        break;
                    case '/':  PUTC(c, '/');
                        break;
                    case 'b':  PUTC(c, '\b');
                        break;
                    case 'f':  PUTC(c, '\f');
                        break;
                    case 'n':  PUTC(c, '\n');
                        break;
                    case 'r':  PUTC(c, '\r');
                        break;
                    case 't':  PUTC(c, '\t');
                        break;
                    case 'u':
                        if (!(p = json_parse_hex4(p, &u)))
                            STRING_ERROR(JSON_PARSE_INVALID_UNICODE_HEX);
                        if (u >= 0xD800 && u <= 0xDBFF)
                        { /* surrogate pair */
                            if (*p++ != '\\')
                                STRING_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE);
                            if (*p++ != 'u')
                                STRING_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE);
                            if (!(p = json_parse_hex4(p, &u2)))
                                STRING_ERROR(JSON_PARSE_INVALID_UNICODE_HEX);
                            if (u2 < 0xDC00 || u2 > 0xDFFF)
                                STRING_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE);
                            u = (((u - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
                        }
                        json_encode_utf8(c, u);
                        break;
                    default:
                        // c->top = head;
                        STRING_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE);
                }
                /* Don't forget break! */
                break;
            default:
                /* Parse invalid string char */
                if ((unsigned char)ch < 0x20)
                    STRING_ERROR(JSON_PARSE_INVALID_STRING_CHAR);
                PUTC(c, ch);
        }
    }

}

static int json_parse_object(json_context* c, json_value* value)
{
    size_t size, i;
    json_member m;
    int ret;
    EXPECT(c, '}');
    json_parse_whiteSpace(c);
    if (*c->json == '}')
    {
        c->json++;
        value->type = json_type::JSON_OBJECT;
        value->m = 0;
        value->size = 0;
        return JSON_PARSE_OK;
    }
    m.k = nullptr;
    size = 0;
    for (;;)
    {
        char* str;
        JSON_INIT(&m.value);
        /** parse key */
        if (*c->json != '"')
        {
            ret = JSON_PARSE_MISS_KEY;
            break;
        }

        if ((ret = json_parse_string_raw(c, &str, &m.klen)) != JSON_PARSE_OK)
            break;
        memcpy(m.k = new char, str, m.klen);
        m.k[m.klen] = '\0';

        /** parse colon ws */
        json_parse_whiteSpace(c);
        if (*c->json != ':')
        {
            ret = JSON_PARSE_MISS_COLON;
            break;
        }
        c->json++;
        json_parse_whiteSpace(c);

        if ((ret = json_parse_value(c, &m.value)) != JSON_PARSE_OK)
            break;
        memcpy(json_context_push(c, sizeof(json_member)), &m, sizeof(json_member));
        size++;
        m.k = nullptr;
        /** parse ws [comma | right-curly-brace] ws */
        json_parse_whiteSpace(c);
        if (*c->json == ',')
        {
            c->json++;
            json_parse_whiteSpace(c);
        }
        else if (*c->json == '}')
        {
            size_t s = sizeof(json_member) * size;
            c->json++;
            value->type = json_type::JSON_OBJECT;
            value->size = size;
            memcpy(std::get<json_member*>(value->m), json_context_pop(c, s), s);
            return JSON_PARSE_OK;
        } else
        {
            ret = JSON_PARSE_MISS_COMMA_OR_CURLY_BRACKET;
            break;
        }
    }
    /** Pop and free members on the stack */
    delete(m.k);
    for (i = 0; i < size; i++)
    {
        auto m = (json_member*)json_context_pop(c, sizeof(json_member));
        delete(m->k);
        json_free(&m->value);
    }
    value->type = json_type ::JSON_NULL;
    return ret;
}


