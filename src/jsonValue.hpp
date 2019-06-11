//
// Created by fszhuangb on 2019-05-27.
//

#pragma once

#include "json.hpp"
#include "jsonException.hpp"
#include <variant>

namespace rafaJSON
{
    enum class json_type;
    class json_value
    {
        /**
         * ctor
         * */
    public:
        explicit json_value(std::nullptr_t) : _val(nullptr) {}
        explicit json_value(bool val) : _val(val) {}
        explicit json_value(double val) : _val(val) {}
        explicit json_value(const std::string& val) : _val(val) {}

        /**
         * move ctor for string
         * */
    public:
        explicit json_value(std::string&& val) : _val(std::move(val)) {}
        /**
         * dtor
         * */
    public:
        ~json_value() = default;

        /**
         * type interface
         * */
    public:
        json_type json_get_type() const noexcept;

        /**
         * \TODO: interface for array and object
         * */
    public:
        size_t size() const;

        /** random acess */
        const Json& operator[](size_t) const;
        Json& operator[](size_t);


        /**
         * convert json_value into value instance
         * */
    public:
        std::nullptr_t json_value_to_null() const;
        bool json_value_to_bool() const;
        double json_value_to_double() const;
        const std::string& json_value_to_string() const;
        const Json::_array& json_value_to_array() const;

    /**
     * std::variant is a C++17 features,like union in C language
     * */
    private:
        std::variant<std::nullptr_t, bool, double, std::string, Json::_array> _val;
    };

}