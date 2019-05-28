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
    };
}