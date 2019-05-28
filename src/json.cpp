//
// Created by fszhuangb on 2019-05-27.
//

#include "json.hpp"
#include "jsonException.hpp"
#include "jsonValue.hpp"

namespace rafaJSON
{
    /**
     * Json's ctor && dtor
     * */
     Json::Json(std::nullptr_t) : _jsonValue(std::make_unique<json_value>(nullptr)) {}
     Json::Json(bool val) : _jsonValue(std::make_unique<json_value>(val)) {}

     Json::~Json() = default;

    /**
     * json's copy constructor && copy assignment
     * */
    Json::Json(const Json& rhs)
    {
        switch (rhs.json_get_type())
        {
            case json_type::JSON_NULL: _jsonValue = std::make_unique<json_value>(nullptr);
                break;
            case json_type::JSON_BOOL: _jsonValue = std::make_unique<json_value>(rhs.json_value_to_Bool());
                break;
        }
    }

//    Json& Json::operator=(Json& rhs) noexcept
//    {
//        Json temp(rhs);
//        swap(temp);
//        return *this;
//
//    }
    /***
     *  Json's move operation=default
     */
    Json::Json(Json&& rhs) noexcept = default;

     /**
      * type interface
      * */
     json_type Json::json_get_type() const noexcept
     {
         /**
          * package a new method to get the type
          * */
         return _jsonValue->json_get_type();
     }

     bool Json::json_value_is_Null() const noexcept
     {
         return json_get_type() == json_type::JSON_NULL;
     }

     bool Json::json_value_is_Bool() const noexcept
     {
         return json_get_type() == json_type::JSON_BOOL;
     }

     /**
      * parse interface:
      * convert json object into value instance
      * */
      bool Json::json_value_to_Bool() const
      {
          return _jsonValue->json_value_to_bool();
      }
}