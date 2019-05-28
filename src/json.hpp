//
// Created by fszhuangb on 2019-05-24.
//

#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <variant>
#include "jsonValue.hpp"

namespace rafaJSON
{
    enum class json_type
    {
        JSON_NULL,
        JSON_BOOL,
        JSON_NUMBER,
        JSON_STRING,
        JSON_ARRAY,
        JSON_OBJECT
    };

    /**
     * forward declaration for std::unique_ptr
     * */
class json_value;

    class Json final
    {
    public:
        /**
         * alias declarations
         * */
        using _array = std::vector<Json>;
        using _object = std::unordered_map<std::string, Json>;

    public:
        Json() :Json(nullptr) {};
        Json(std::nullptr_t);
        Json(bool);

        /**
         * this prevent Json(some_pointer) from accidentally producing a bool
         * */
        Json(void *) = delete;

    public:
        ~Json();

        /**
         * json's copy constructor && copy assignment
         * */
    public:
        Json(const Json& rhs);
        Json& operator=(Json& rhs) noexcept;

        /**
         * move constructor && assignment
         * */
    public:
        Json(Json&&) noexcept;


        /**
         * type interface
         * */
    public:
        json_type json_get_type() const noexcept;
        bool json_value_is_Null() const noexcept;
        bool json_value_is_Bool() const noexcept;


        /**
         * convert json object into value instance
         * */
    public:
        bool json_value_to_Bool() const;

        /**
         * parse && serialize interface
         * */
    public:
        static Json parse(const std::string& content, std::string& errMsg) noexcept;
        // \TODO
        std::string serialize() const noexcept;

        /**
         * data member
         * */
    private:
        // use _jsonValue as a pointer to json_value
        std::unique_ptr<json_value> _jsonValue;

    };
}