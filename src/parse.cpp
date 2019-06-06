//
// Created by fszhuangb on 2019-05-28.
//
#include "parse.hpp"
#include <cassert>    // assert
#include <stdexcept>  // runtime_error
#include <cstring>    // strncmp
#include <cmath>      // Huge_val

namespace rafaJSON
{
    /**
     * parse whitespace:skip all
     * */
     void Parser::json_parse_whitespace() noexcept
     {
         while (*_curr == ' ' || *_curr == '\t' || *_curr == '\r' || *_curr == '\n')
             ++_curr;
         // replace the start position
         _start = _curr;
     }

     Json Parser::error(const std::string &msg) const
     {
         throw JsonException(msg + ": " + _start);
     }

     Json Parser::json_parse_value()
     {
         switch (*_curr)
         {
             case 'n':  return json_parse_literal("null");
             case 't':  return json_parse_literal("true");
             case 'f':  return json_parse_literal("false");
             case '\0': return error("EXPECT VALUE");
         }
     }
     Json Parser::json_parse_literal(const std::string &literal)
     {
         if (strncmp(_curr, literal.c_str(), literal.size()))
             error("INVALID VALUE");
         _curr += literal.size();
         _start = _curr;
         switch (literal[0])
         {
             case 't': return Json(true);
             case 'f': return Json(false);
             default: return Json(nullptr);
         }
     }


     Json Parser::parse()
     {
         json_parse_whitespace();
         Json json = json_parse_value();
         json_parse_whitespace();
         if (*_curr)
             error("ROOT NOT SINGULAR");
         return json;
     }
}

