//
// Created by fszhuangb on 2019-05-28.
//

#pragma once

#include <stdexcept>

namespace rafaJSON
{
    class JsonException: public std::runtime_error
    {
        // ctor
    public:
        explicit JsonException(const std::string& errMsg) : runtime_error(errMsg) {}

    public:
        const char* what() const noexcept override
        {
            return runtime_error::what();
        }
    };
}