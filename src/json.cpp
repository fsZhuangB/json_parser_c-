//
// Created by fszhuangb on 2019-05-27.
//

#include "json.hpp"
#include "jsonException.hpp"
#include "jsonValue.hpp"
#include "parse.hpp"

namespace rafaJSON
{
    /**
     * Json's ctor && dtor
     * */
     Json::Json(std::nullptr_t) : _jsonValue(std::make_unique<json_value>(nullptr)) {}
     Json::Json(bool val) : _jsonValue(std::make_unique<json_value>(val)) {}
     Json::Json(double val) : _jsonValue(std::make_unique<json_value>(val)) {}
     Json::Json(const std::string& val) : _jsonValue(std::make_unique<json_value>(val)) {}
     Json::Json(std::string&& val) : _jsonValue(std::make_unique<json_value>(std::move(val))) {}

     Json::~Json() = default;

    /**
     * json's copy constructor && copy assignment
     * */
    Json::Json(const Json& rhs)
    {
        switch (rhs.json_get_type())
        {
            case json_type ::JSON_NULL: _jsonValue = std::make_unique<json_value>(nullptr);
                break;
            case json_type ::JSON_BOOL: _jsonValue = std::make_unique<json_value>(rhs.json_value_to_Bool());
                break;
            case json_type ::JSON_NUMBER: _jsonValue = std::make_unique<json_value>(rhs.json_value_to_Double());
                break;
            case json_type ::JSON_STRING: _jsonValue = std::make_unique<json_value>(rhs.json_value_to_String());
                break;
        }
    }

    Json& Json::operator=(Json& rhs) noexcept
    {
        Json temp(rhs);
        swap(temp);
        return *this;

    }
    /***
     *  Json's move operation=default
     */
    Json::Json(Json&& rhs) noexcept = default;
    Json & Json::operator=(Json &&rhs) noexcept = default;

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
         return json_get_type() == json_type ::JSON_NULL;
     }

     bool Json::json_value_is_Bool() const noexcept
     {
         return json_get_type() == json_type ::JSON_BOOL;
     }

     bool Json::json_value_is_Number() const noexcept
     {
         return json_get_type() == json_type ::JSON_NUMBER;
     }

     bool Json::json_value_is_String() const noexcept
     {
         return json_get_type() == json_type ::JSON_STRING;
     }

     /**
      * parse interface:
      * convert json object into value instance
      * */
      bool Json::json_value_to_Bool() const
      {
          return _jsonValue->json_value_to_bool();
      }

      double Json::json_value_to_Double() const
      {
          return _jsonValue->json_value_to_double();
      }

      const std::string& Json::json_value_to_String() const
      {
          return _jsonValue->json_value_to_string();
      }

      /** aux interface copy && swap */
      void Json::swap(rafaJSON::Json &rhs) noexcept
      {
          using std::swap;
          swap(_jsonValue, rhs._jsonValue);
      }

      /** parse interface */
      Json Json::parse(const std::string &content, std::string &errMsg) noexcept
      {
          try
          {
              Parser p(content);
              return p.parse();
          }
          catch (JsonException& e)
          {
              errMsg = e.what();
              return Json(nullptr);
          }

      }
}