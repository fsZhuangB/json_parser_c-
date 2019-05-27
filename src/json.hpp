//
// Created by fszhuangb on 2019-05-24.
//

#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <variant>
#include "jsonValue.cpp"

namespace rafaJSON
{
    enum class json_type
    {
        JSON_NULL,
        JSON_FALSE,
        JSON_TRUE,
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


    public:
        ~Json();

        /**
         * type interface
         * */
    public:
        json_type json_get_type() const noexcept;
        bool isNull() const noexcept;
        bool isBool() const noexcept;


        /**
         * convert json object into value
         * */
    public:
        bool toBool() const;

        /**
         * data member
         * */
    private:
        std::unique_ptr<json_value> _jsonValue;

    };
}