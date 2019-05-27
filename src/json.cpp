//
// Created by fszhuangb on 2019-05-27.
//

#include "json.hpp"

namespace rafaJSON
{
    /**
     * Json's ctor && dtor
     * */
     Json::Json(std::nullptr_t) : _jsonValue(std::make_unique<json_value>(nullptr)) {}
     Json::Json(bool val) : _jsonValue(std::make_unique<json_value>(val)) {}


}