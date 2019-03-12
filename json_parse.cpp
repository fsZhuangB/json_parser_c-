#include "rafajson.hpp"

int json_parse(json_value * value, const std::string json)
{
    json_context c;
    assert(value != nullptr);
    c.json = json;
    value->type = json_type::JSON_NULL;
    json_parse_whiteSpace(&c);

    return json_parse_value(&c, value);
}