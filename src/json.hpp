//
// Created by fszhuangb on 2019-05-24.
//

#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <variant>

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

        /** convert int into double */
        Json(int val) : Json(1.0 * val) {}
        Json(double);

        /** witnout this ctor, Json("xx") will call Json(bool) */
        Json(const char* cstr) : Json(std::string(cstr)) {}
        Json(const std::string&);  // copy constructor， use std::string's ctor
        Json(std::string&&);       // move constructor
        Json(const _array&);
        Json(_array&&);
        Json(const _object&);
        Json(_object&&);

    public:
        /** implicit ctor for objects */
        /** std::is_constructible: If T is an object or reference type and the variable
         *  definition T obj(std::declval<Args>()...); is well-formed,
         *  provides the member constant value equal to true.
         *  In all other cases, value is false.
         * */

         /** Implicit constructor: map-like objects (std::map, std::unordered_map, etc) */
        template <class M, typename std::enable_if<
                std::is_constructible<std::string, decltype(std::declval<M>().begin()->first)>::value
                && std::is_constructible<Json, decltype(std::declval<M>().begin()->second)>::value, int>::type = 0>
        Json(const M& m) : Json(_object(m.begin(), m.end())) {}

        /** Implicit constructor: vector-like objects (std::list, std::vector, std::set, etc)*/
        template <class V, typename std::enable_if<
                std::is_constructible<Json, decltype(*std::declval<V>().begin())>::value, int>::type = 0>
        Json(const V& v) : Json(_object(v.begin(), v.end())) {}
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
        Json(const Json&);
        Json& operator=(Json&) noexcept;

        /**
         * move constructor && assignment
         * */
    public:
        Json(Json&&) noexcept;
        Json& operator=(Json&&) noexcept;


        /**
         * type interface
         * */
    public:
        json_type json_get_type() const noexcept;
        bool json_value_is_Null() const noexcept;
        bool json_value_is_Bool() const noexcept;
        bool json_value_is_Number() const noexcept;
        bool json_value_is_String() const noexcept;
        bool json_value_is_Array() const noexcept;
        bool json_value_is_Object() const noexcept;


        /**
         * convert json object into value instance
         * */
    public:
        bool json_value_to_Bool() const;
        double json_value_to_Double() const;
        const std::string& json_value_to_String() const;
        const Json::_array& json_value_to_Array() const;
        const Json::_object& json_value_to_Object() const;

        /**
         * parse && serialize interface
         * */
    public:
        static Json parse(const std::string& content, std::string& errMsg) noexcept;
        // \TODO
        std::string serialize() const noexcept;

    public:
        void swap(Json&) noexcept; // make copy && swap

        /**
         * interface for array and object
         * The first kind: not only we can access the value, but also we can change the value
         * 1. return_value & operator[](parameter)
         * The second kind: we can only access the value
         * 2. const return_value & operator[](parameter) const;
         * */
        size_t json_get_size() const;
        /** overload [] for array */
        Json& operator[](size_t);
        const Json& operator[](size_t) const;
        /** for object */
        Json& operator[](const std::string&);
        const Json& operator[](const std::string&) const;

        /**
         * data member
         * */
    private:
        /** use _jsonValue as a pointer to json_value */
        std::unique_ptr<json_value> _jsonValue;
    };

    /** non-member function */
    bool operator==(const Json&, const Json&);
}