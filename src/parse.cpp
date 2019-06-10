//
// Created by fszhuangb on 2019-05-28.
//
#include "parse.hpp"
#include <cassert>    // assert
#include <stdexcept>  // runtime_error
#include <cstring>    // strncmp
#include <cmath>      // Huge_val
#include <cstdlib>    // strtod

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
             default:
                 return json_parse_number();
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

     /**
      * The syntax of JSON number, [] means not necessary
      * number = [ "-" ] int [ frac ] [ exp ]
      * int = "0" / digit1-9 *digit
      * frac = "." 1*digit
      * exp = ("e" / "E") ["-" / "+"] 1*digit
      * */
     Json Parser::json_parse_number()
     {
         /** make true the number is valid */
         if (*_curr == '-') ++_curr;
         if (*_curr == '0')
             ++_curr;
         else
         {
             if (!is_1_to_9(*_curr))
                 error("INVALID VALUE!");
             while (is_0_to_9(*++_curr)); // parse all number here
         }

         if (*_curr == '.')
         {
             /** There must be a number after '.' */
             if (!is_0_to_9(*++_curr))
                 error("INVALID VALUE");
             while (is_0_to_9(*++_curr));
         }

         if (toupper(*_curr) == 'E')
         {
             ++_curr;
             if (*_curr == '-' || *_curr == '+')
                 ++_curr;
             if (!is_0_to_9(*_curr))
                 error("INVALID VALUE");
             while (is_0_to_9(*++_curr));
         }

         /** we use function strtod() in cstdlib.h
          * convert the string to double
          * */
          double val = strtod(_start, nullptr);
          if (fabs(val) == HUGE_VAL)
              error("NUMBER TOO BIG");
          _start = _curr;
          return Json(val); // reconstruct the json instance
     }


     /**
      * @param: void
      * ROOT NOT SINGULAR means some character still exists after the end whitespace
      * */
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

