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
             case '\"': return json_parse_string();
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


     Json Parser::json_parse_string()
     {
         return Json(json_parse_raw_string());
     }

     /**
      * @param: void
      * The syntax of string:
      * string = quotation-mark *char quotation-mark
      * char = unescaped /
      * escape (
      *    %x22 /          ; "    quotation mark  U+0022
      *    %x5C /          ; \    reverse solidus U+005C
      *    %x2F /          ; /    solidus         U+002F
      *    %x62 /          ; b    backspace       U+0008
      *    %x66 /          ; f    form feed       U+000C
      *    %x6E /          ; n    line feed       U+000A
      *    %x72 /          ; r    carriage return U+000D
      *    %x74 /          ; t    tab             U+0009
      *    %x75 4HEXDIG )  ; uXXXX                U+XXXX
      *    escape = %x5C          ; \
      *    quotation-mark = %x22  ; "
      *    unescaped = %x20-21 / %x23-5B / %x5D-10FFFF
      * */
     std::string Parser::json_parse_raw_string()
     {
         /** we need a new string str to store the character we parse */
         std::string str;
         while (true)
         {
             switch (*++_curr)
             {
                 case '\"' : _start = ++_curr; return str;
                 case '\0' :
                     error("MISS QUOTATION MARK");
                 default:
                     if (static_cast<unsigned char>(*_curr) < 0x20)
                         error("INVALID STRING CHAR");
                     str.push_back(*_curr);
                     break;

                     /** when encounter escape character */
                 case '\\':
                     switch (*++_curr)
                     {
                         case '\"': str.push_back('\"');
                             break;
                         case '\\': str.push_back('\\');
                             break;
                         case '/': str.push_back('/');
                             break;
                         case 'b': str.push_back('\b');
                             break;
                         case 'n': str.push_back('\n');
                             break;
                         case 'f': str.push_back('\f');
                             break;
                         case 'r': str.push_back('\r');
                             break;
                         case 't': str.push_back('\t');
                             break;
                         case 'u':
                         {
                             unsigned u1 = json_parse_hex4();
                             if (u1 >= 0xd800 && u1 <= 0xdbff)
                             {
                                 if (*++_curr != '\\')
                                     error("INVALID UNICODE SURROGATE");
                                 if (*++_curr != 'u')
                                     error("INVALID UNICODE SURROGATE");
                                 unsigned u2 = json_parse_hex4();
                                 if (u2 < 0xdc00 || u2 > 0xdfff)
                                     error("INVALID UNICODE SURROGATE");
                                 u1 = (((u1 - 0xd800) << 10) | (u2 - 0xdc00)) + 0x10000;
                             }
                             str += json_encode_utf8(u1);
                         }
                             break;
                         default:
                             error("INVALID STRING ESCAPE");
                     }
                     break;
             }
         }


     }


     std::string Parser::json_encode_utf8(unsigned u) noexcept
     {
         std::string utf8;
         if (u <= 0x7F)  // 0111,1111
             utf8.push_back(static_cast<char>(u & 0xff));
         else if (u <= 0x7FF) {
             utf8.push_back(static_cast<char>(0xc0 | ((u >> 6) & 0xff)));
             utf8.push_back(static_cast<char>(0x80 | (u & 0x3f)));
         }
         else if (u <= 0xFFFF) {
             utf8.push_back(static_cast<char>(0xe0 | ((u >> 12) & 0xff)));
             utf8.push_back(static_cast<char>(0x80 | ((u >> 6) & 0x3f)));
             utf8.push_back(static_cast<char>(0x80 | (u & 0x3f)));
         }
         else {
             assert(u <= 0x10FFFF);
             utf8.push_back(static_cast<char>(0xf0 | ((u >> 18) & 0xff)));
             utf8.push_back(static_cast<char>(0x80 | ((u >> 12) & 0x3f)));
             utf8.push_back(static_cast<char>(0x80 | ((u >> 6) & 0x3f)));
             utf8.push_back(static_cast<char>(0x80 | (u & 0x3f)));
         }
         return utf8;

     }

     unsigned Parser::json_parse_hex4()
     {
         unsigned u;
         int i;
         for (i = 0; i != 4; ++i)
         {
             unsigned ch = static_cast<unsigned>(toupper(*++_curr));
             u <<= 4;
             if (ch >= '0' && ch <= '9')
                 u |= (ch - '0');
             else if (ch >= 'A' && ch <= 'F')
                 u |= ch - 'A' + 10;
             else
                 error("INVALID UNICODE HEX");
         }
         return u;
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

