//
// Created by fszhuangb on 2019-05-28.
//

#pragma once

#include "json.hpp"
#include "jsonException.hpp"

namespace rafaJSON
{
    // constexpr function to determine a char is a specified number
    constexpr bool is_1_to_9(char ch) { return ch >= '1' && ch <= '9'; }
    constexpr bool is_0_to_9(char ch) { return ch >= '0' && ch <= '9'; }

    class Parser
    {
    public:
        // ctor
        explicit Parser(const char* cstr) noexcept : _start(cstr), _curr(cstr) {}
        explicit Parser(const std::string& content) noexcept : _start(content.c_str()), _curr(content.c_str()) {}

    public:
        // uncopyable
        Parser(const Parser&) = delete;
        Parser& operator=(const Parser&) = delete;

        /**
         * parse_aux interface
         * */
    private:
        void json_parse_whitespace() noexcept;
        std::string json_parse_raw_string();
        /** parse utf8 */
        unsigned json_parse_hex4();
        std::string json_encode_utf8(unsigned) noexcept;

        // indicate the error position
        Json error(const std::string& msg) const;

        /**
         * parse interface
         * */
    private:
        Json json_parse_value();
        Json json_parse_literal(const std::string& literal);
        Json json_parse_number();
        Json json_parse_string();

        /**
         * public parse interface
         * */
    public:
        Json parse();

    private:
        /**
         * private data member
         * two const char* pointers point to the context's current position and start position
         * */
         const char* _start;
         const char* _curr;
    };
}