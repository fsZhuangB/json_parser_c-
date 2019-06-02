//
// Created by fszhuangb on 2019-05-28.
//

#include "jsonValue.hpp"
#include "json.hpp"
#include "jsonException.hpp"

namespace rafaJSON
{
    /**
     * type interface
     * std::holds_alternative checks if the variant v holds the alternative T
     * */
    json_type json_value::json_get_type() const noexcept
    {
        if (std::holds_alternative<std::nullptr_t>(_val))
            return json_type::JSON_NULL;
        else if (std::holds_alternative<bool>(_val))
            return json_type::JSON_BOOL;
        else if (std::holds_alternative<double>(_val))
            return json_type::JSON_NUMBER;
        else if (std::holds_alternative<std::string>(_val))
            return json_type::JSON_STRING;
        else if (std::holds_alternative<Json::_array>(_val))
            return json_type::JSON_ARRAY;
        else
            return json_type::JSON_OBJECT;
    }

    /**
     * convert interface: convert json_value into value instance
     * */
     std::nullptr_t json_value::json_value_to_null() const
     {
         try
         {
             return std::get<std::nullptr_t >(_val);
         }
         catch (const std::bad_variant_access&)
         {
             throw JsonException("Not a null");
         }
     }

     bool json_value::json_value_to_bool() const
     {
         try
         {
             return std::get<bool>(_val);
         }
         catch (const std::bad_variant_access&)
         {
             throw JsonException("Not a bool");
         }

     }

}