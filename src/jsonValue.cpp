//
// Created by fszhuangb on 2019-05-28.
//

#include "jsonValue.hpp"
#include "json.hpp"

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
         /**
          * \TODO:catch exception
          * */
         return std::get<std::nullptr_t>(_val);
     }

     bool json_value::json_value_to_bool() const
     {
         /**
          * \TODO:catch exception
          * */
          return std::get<bool>(_val);

     }

}