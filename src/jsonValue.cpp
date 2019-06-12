//
// Created by fszhuangb on 2019-05-28.
//

#include "jsonValue.hpp"
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
            return json_type ::JSON_NULL;
        else if (std::holds_alternative<bool>(_val))
            return json_type ::JSON_BOOL;
        else if (std::holds_alternative<double>(_val))
            return json_type ::JSON_NUMBER;
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
             throw JsonException("NOT A NULL");
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
             throw JsonException("NOT A BOOL");
         }
     }

     double json_value::json_value_to_double() const
     {
         try
         {
             return std::get<double>(_val);
         }
         catch (const std::bad_variant_access&)
         {
             throw JsonException("NOT A DOUBLE");
         }
     }

     const std::string& json_value::json_value_to_string() const
     {
         try
         {
             return std::get<std::string>(_val);
         }
         catch (const std::bad_variant_access&)
         {
             throw JsonException("NOT A STRING");
         }
     }

     const Json::_array& json_value::json_value_to_array() const
     {
         try
         {
             return std::get<Json::_array>(_val);
         }
         catch (const std::bad_variant_access&)
         {
             throw JsonException("NOT AN ARRAY");
         }
     }

     const Json::_object& json_value::json_value_to_object() const
     {
         try
         {
             return std::get<Json::_object>(_val);
         }
         catch (const std::bad_variant_access&)
         {
             throw JsonException("NOT AN OBJECT");
         }

     }

     size_t json_value::size() const
     {
         if (std::holds_alternative<Json::_array>(_val))
             return std::get<Json::_array>(_val).size();
         else if (std::holds_alternative<Json::_object>(_val))
             return std::get<Json::_object>(_val).size();
         else
             throw JsonException("NOT AN ARRAY OR OBJECT");
     }

     /** overload operator[] for array */
     Json& json_value::operator[](size_t pos)
     {
         return const_cast<Json&>(static_cast<const json_value&>(*this)[pos]);
     }

     const Json& json_value::operator[](size_t pos) const
     {
         if (std::holds_alternative<Json::_array>(_val))
             return std::get<Json::_array>(_val)[pos];
         else
             throw JsonException("NOT AN ARRAY");
     }

     /** overload operator[] for object */
     Json& json_value::operator[](const std::string &key)
     {
         return const_cast<Json&>(static_cast<const json_value&>(*this)[key]);
     }

     const Json& json_value::operator[](const std::string &key) const
     {
         if (std::holds_alternative<Json::_object>(_val))
             return std::get<Json::_object>(_val).at(key);
         else
             throw JsonException("NOT AN OBJECT");
     }

}